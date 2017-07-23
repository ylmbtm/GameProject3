#include "stdafx.h"
#include "CommandDef.h"
#include "GameService.h"
#include "Scene.h"
#include "PacketHeader.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"
#include "SceneLogic/SceneLogic_Type.h"
#include "SceneLogic/SceneLogic_Normal.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Game_Define.pb.h"
#include "../Message/Msg_Move.pb.h"
#include "MonsterCreator.h"
#include "SceneLogic/SceneLogic_None.h"
#include "SceneLogic/SceneLogic_City.h"
#include "Utility/RapidXml//rapidxml.h"
#include "SceneXmlMgr.h"
#include "Utility/CommonConvert.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

BOOL CScene::Init(UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwLogicType,UINT32 dwPlayerNum)
{
	m_dwCopyID		= dwCopyID;
	m_dwCopyType	= dwCopyType;
	m_dwLogicType	= dwLogicType;
	m_dwPlayerNum	= dwPlayerNum;
	m_dwLoginNum	= 0;
	m_dwStartTime	= 0;
    m_dwCreateTime	= CommonFunc::GetCurrTime();
	m_pMonsterCreator = new MonsterCreator(this);
	m_dwMaxGuid = 1;

	ERROR_RETURN_FALSE(CreateSceneLogic(dwLogicType));
	ERROR_RETURN_FALSE(ReadSceneXml());

	return TRUE;
}

BOOL CScene::Uninit()
{
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pObj = itor->second;
        delete pObj;
    }

    m_PlayerMap.clear();
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); itor++)
    {
        CSceneObject *pObj = itor->second;
        delete pObj;
    }
    m_MonsterMap.clear();

	delete m_pMonsterCreator;

	ERROR_RETURN_FALSE(DestroySceneLogic(m_dwLogicType));

	return TRUE;
}

BOOL CScene::DispatchPacket(NetPacket *pNetPacket)
{
	PacketHeader *pPacketHeader = (PacketHeader *)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);
	
	switch(pPacketHeader->dwMsgID)
	{
		PROCESS_MESSAGE_ITEM(MSG_TRANS_ROLE_DATA_REQ,   OnMsgTransRoleDataReq);
		PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,		OnMsgEnterSceneReq);
		PROCESS_MESSAGE_ITEM(MSG_LEAVE_SCENE_REQ,		OnMsgLeaveSceneReq);
		PROCESS_MESSAGE_ITEM(MSG_DISCONNECT_NTY,		OnMsgRoleDisconnect);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_MOVE_REQ,			OnMsgRoleMoveReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_SKILL_REQ,		OnMsgRoleSkillReq);
		
		default:
		{
			return FALSE;
		}
		break;
	}

	return TRUE;
}


BOOL CScene::OnMsgRoleMoveReq(NetPacket *pNetPacket)
{
	ObjectMoveReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	
	for(int i = 0; i < Req.movelist_size(); i++)
	{
		const MoveItem &Item = Req.movelist(i);
		CSceneObject *pSceneObj = GetPlayer(Item.objectid());
		ERROR_RETURN_TRUE(pSceneObj != NULL);

		pSceneObj->m_dwObjState =  Item.movestate();
		pSceneObj->x = Item.x();
		pSceneObj->z = Item.z();
		pSceneObj->vx = Item.vx();
		pSceneObj->vz = Item.vz();
		pSceneObj->SetChanged();
	}

	return TRUE;
}

BOOL CScene::OnMsgRoleSkillReq(NetPacket *pNetPacket)
{
	RoleSkillReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();


    //



	return TRUE;
}


BOOL CScene::OnMsgRoleDisconnect(NetPacket *pNetPacket)
{
	RoleDisconnectReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject *pPlayer = GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pPlayer != NULL);
	pPlayer->SetConnectID(0, 0);

	return TRUE;
}

BOOL CScene::BroadNewObject(CSceneObject *pSceneObject)
{
    //先把玩家的完整包组装好
    ObjectNewNty Nty;
    pSceneObject->SaveNewObject(Nty);

	char szBuff[10240] = {0};
	Nty.SerializePartialToArray(szBuff, Nty.ByteSize());

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL)
        
        if(!pOther->IsConnected())
        {
            continue;
        }

        if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
        {
            continue;
        }

		pOther->SendMsgRawData(MSG_OBJECT_NEW_NTY, szBuff, Nty.ByteSize());
    }

	return TRUE;
}

BOOL CScene::OnMsgLeaveSceneReq(NetPacket *pNetPacket)
{
	LeaveSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject *pSceneObject = GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pSceneObject != NULL);

	if(m_pSceneLogic->OnPlayerLeave(pSceneObject))
	{
		BroadRemoveObject(pSceneObject);
		DeletePlayer(Req.roleid());
	}
	return TRUE;
}



BOOL CScene::OnUpdate( UINT32 dwTick )
{
	if((m_dwLastTick > dwTick)&&(m_dwLastTick - dwTick < 20))
	{
		return TRUE;
	}
	
	m_dwLastTick = dwTick;

	if(m_pSceneLogic->IsFinished()) //己经结束不再处理
	{
		return TRUE;
	}

    SyncObjectState(); //同步所有对象的状态

	//如果playerNum 不等于0， 表示有人数要求, 0表示无人数要求
	/*if(m_dwPlayerNum != 0) 
	{
		if(m_dwPlayerNum == m_PlayerMap.size())
	}*/


	if(IsAllDataReady())
	{
		m_pMonsterCreator->OnUpdate(dwTick);
	}

    m_pSceneLogic->Update(dwTick);

	return TRUE;
}

BOOL CScene::CreateSceneLogic(UINT32 dwLogicType)
{
	ERROR_RETURN_FALSE(dwLogicType != 0);
	switch (dwLogicType)
	{
	case SLT_NORMAL:
		m_pSceneLogic = new SceneLogic_Normal(this);
		break;

	case SLT_CITY:
		m_pSceneLogic = new SceneLogic_City(this);
		break;
	default:
		{
			m_pSceneLogic = new SceneLogic_None(this);
		}
	}

	return (m_pSceneLogic != NULL);
}

BOOL CScene::DestroySceneLogic(UINT32 dwLogicType)
{
	switch (dwLogicType)
	{
	case SLT_NORMAL:
		{
			SceneLogic_Normal* pTemp = (SceneLogic_Normal*)m_pSceneLogic;
			delete pTemp;
		}
		break;
	case SLT_CITY:
		{
			SceneLogic_City* pTemp = (SceneLogic_City*)m_pSceneLogic;
			delete pTemp;
		}
		break;
	default:
		{
			SceneLogic_None* pTemp = (SceneLogic_None*)m_pSceneLogic;
			delete pTemp;
		}
	}

	return (m_pSceneLogic != NULL);
}

BOOL CScene::OnMsgTransRoleDataReq(NetPacket *pNetPacket)
{
	TransRoleDataReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);
	ERROR_RETURN_TRUE(pHeader->u64TargetID == Req.roleid());
	CSceneObject *pObject = GetPlayer(pHeader->u64TargetID);
	if(pObject == NULL)
	{
		pObject = new CSceneObject(pHeader->u64TargetID, Req.actorid(), OT_PLAYER, 2, (std::string &)Req.rolename());
		AddPlayer(pObject);
	}
	//根据数据创建宠物，英雄
	pObject->m_dwObjType = OT_PLAYER;
	pObject->m_dwActorID = Req.actorid();
	pObject->m_strName = Req.rolename();
	pObject->m_uGuid = pHeader->u64TargetID;

	m_pSceneLogic->OnObjectCreate(pObject);

    //检查人齐没齐，如果齐了，就全部发准备好了的消息
    //有的副本不需要等人齐，有人就可以进

	TransRoleDataAck Ack;
	Ack.set_copyid(m_dwCopyID);
	Ack.set_copytype(m_dwCopyType);
	Ack.set_roleid(pHeader->u64TargetID);
	Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
	Ack.set_retcode(MRC_SUCCESSED);

	ERROR_RETURN_FALSE(m_dwCopyID != 0);
	ERROR_RETURN_FALSE(m_dwCopyType != 0);
	ERROR_RETURN_FALSE(pHeader->u64TargetID != 0);
	ERROR_RETURN_FALSE(CGameService::GetInstancePtr()->GetServerID() != 0);

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_TRANS_ROLE_DATA_ACK, pHeader->u64TargetID, 0, Ack);
	return TRUE;
}

BOOL CScene::OnMsgEnterSceneReq(NetPacket *pNetPacket)
{
	EnterSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject *pSceneObj = GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pSceneObj != NULL);

	pSceneObj->SetConnectID(pNetPacket->m_dwConnID, (UINT32)pHeader->u64TargetID);
	pSceneObj->SetEnterCopy();
	m_pSceneLogic->OnPlayerEnter(pSceneObj);

	m_dwLoginNum ++;

	//发比较全的自己的信息
	EnterSceneAck Ack;
	Ack.set_copyid(m_dwCopyID);
	Ack.set_copytype(m_dwCopyType);
	Ack.set_roleid(Req.roleid());
	Ack.set_rolename(pSceneObj->m_strName);
	Ack.set_actorid(pSceneObj->m_dwActorID);
	Ack.set_retcode(MRC_SUCCESSED);

	pSceneObj->SendMsgProtoBuf(MSG_ENTER_SCENE_ACK, Ack);
    SendAllNewObjectToPlayer(pSceneObj);
    BroadNewObject(pSceneObj);

	m_dwStartTime = CommonFunc::GetCurrTime();
	return TRUE;
}

UINT32 CScene::GetCopyID()
{
    return m_dwCopyID;
}

UINT32 CScene::GetCopyType()
{
    return m_dwCopyType;
}

BOOL CScene::SendAllNewObjectToPlayer( CSceneObject *pSceneObject )
{
    //先把玩家的完整包组装好
    ObjectNewNty Nty;

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
        {
            continue;
        }

        pOther->SaveNewObject(Nty);
    }

    pSceneObject->SendMsgProtoBuf(MSG_OBJECT_NEW_NTY, Nty);

    return TRUE;
}

BOOL CScene::GetPlayerCount()
{
	return m_PlayerMap.size();
}

BOOL CScene::BroadRemoveObject( CSceneObject *pSceneObject )
{
    ObjectRemoveNty Nty;
    Nty.add_removelist(pSceneObject->GetObjectGUID());

	char szBuff[10240] = {0};
	Nty.SerializePartialToArray(szBuff, Nty.ByteSize());

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
        {
            continue;
        }

        if(!pOther->IsConnected())
        {
            continue;
        }

		pOther->SendMsgRawData(MSG_OBJECT_REMOVE_NTY, szBuff, Nty.ByteSize());
    }

    return TRUE;
}

CSceneObject* CScene::GetPlayer( UINT64 uID )
{
    std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.find(uID);
    if(itor != m_PlayerMap.end())
    {
        return itor->second;
    }

    return NULL;
}

BOOL CScene::AddPlayer( CSceneObject *pSceneObject )
{
    ERROR_RETURN_FALSE(pSceneObject != NULL);

	ERROR_RETURN_FALSE(pSceneObject->GetObjectGUID() != 0);

    m_PlayerMap.insert(std::make_pair(pSceneObject->GetObjectGUID(), pSceneObject));

    return TRUE;
}





VOID CScene::DeletePlayer(UINT64 uID)
{
	std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.find(uID);
	if(itor != m_PlayerMap.end())
	{
		m_PlayerMap.erase(itor);
	}
	else
	{
		CLog::GetInstancePtr()->LogError("Error CScene::DeletePlayer cannot find player id:%d", uID);
	}
	
	return ;
}

CSceneObject* CScene::GetSceneObject(UINT64 uID)
{
   std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.find(uID);
   if(itor != m_PlayerMap.end())
   {
       return itor->second;
   }
    
   itor = m_MonsterMap.find(uID);
   if(itor != m_MonsterMap.end())
   {
       return itor->second;
   }
	return NULL;
}

BOOL CScene::IsFinished()
{
	return m_pSceneLogic->IsFinished();
}

BOOL CScene::IsAllDataReady()
{
	if(m_PlayerMap.size() == m_dwPlayerNum)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CScene::IsAllLoginReady()
{
	if(!IsAllDataReady())
	{
		return FALSE;
	}

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject *pObj = itor->second;
		if(!pObj->IsEnterCopy())
		{
			return FALSE;
		}
	}

	return TRUE;
}

UINT32 CScene::GetStartTime()
{
	return m_dwStartTime;
}

UINT32 CScene::GetCreateTime()
{
	return m_dwCreateTime;
}

BOOL CScene::SyncObjectState()
{
    ObjectUpdateNty Nty;
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pObj = itor->second;
        ASSERT(pObj != NULL);

        pObj->SaveUpdateObject(Nty);
    }

	char szBuff[102400] = {0};
	Nty.SerializePartialToArray(szBuff, Nty.ByteSize());

    if(Nty.updatelist_size() > 0)
    {
        for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
        {
            CSceneObject *pObj = itor->second;
            ASSERT(pObj != NULL);

            if(!pObj->IsConnected())
            {
                continue;
            }

            pObj->SendMsgRawData(MSG_OBJECT_UPDATE_NTY, szBuff, Nty.ByteSize());
        }
    }

    return TRUE;
}

BOOL CScene::GenMonster( UINT32 dwActorID, UINT32 dwCamp, FLOAT x, FLOAT y)
{
	//StActorInfo *pActorInf = NULL;
	CSceneObject *pObject = new CSceneObject(m_dwMaxGuid++, dwActorID, OT_MONSTER, dwCamp, "");
	AddPlayer(pObject);
	BroadNewObject(pObject);
    return TRUE;
}

BOOL CScene::ReadSceneXml()
{
	return TRUE;
	rapidxml::xml_document<char> *pXmlDoc = CSceneXmlManager::GetInstancePtr()->GetXmlDocument("xxxx");
	ERROR_RETURN_FALSE(pXmlDoc != NULL);

	rapidxml::xml_node<char>* pXmlRoot = pXmlDoc->first_node("Scene");
	ERROR_RETURN_FALSE(pXmlRoot != NULL);
	
	auto pLogicNode = pXmlRoot->first_node("SceneLogic");
	ERROR_RETURN_FALSE(pLogicNode != NULL);
	ERROR_RETURN_FALSE(m_pSceneLogic != NULL);
	ERROR_RETURN_FALSE(m_pSceneLogic->ReadFromXml(pLogicNode));


	auto pCreatorNode = pXmlRoot->first_node("SceneCreator");
	ERROR_RETURN_FALSE(pCreatorNode != NULL);
	ERROR_RETURN_FALSE(m_pMonsterCreator != NULL);
	ERROR_RETURN_FALSE(m_pMonsterCreator->ReadFromXml(pCreatorNode));

	return TRUE;
}

CSceneObject* CScene::GetOwnPlayer()
{
    if((m_PlayerMap.size() < 1) ||(m_PlayerMap.size() > 1))
    {
        ASSERT_FAIELD;
    }

    std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin();
    if(itor == m_PlayerMap.end())
    {
        return NULL;
    }

    return itor->second;
}

BOOL CScene::SkillFight( CSceneObject *pAttacker, UINT32 dwSkillID, CSceneObject *pDefender )
{
    ERROR_RETURN_FALSE(pAttacker != NULL);
    ERROR_RETURN_FALSE(pDefender != NULL);
    ERROR_RETURN_FALSE(dwSkillID != 0);

    UINT32 dwRandValue = CommonFunc::GetRandNum(1);
    //先判断是否命中
    if (dwRandValue > (800+pAttacker->m_dwProperty[8]-pDefender->m_dwProperty[7]) && dwRandValue > 500) 
    {
        //未命中
        return TRUE;
    }

    //判断是否爆击
    dwRandValue = CommonFunc::GetRandNum(1);
    BOOL bCriticalHit = FALSE;
    if (dwRandValue < (pAttacker->m_dwProperty[9]-pAttacker->m_dwProperty[10]) || dwRandValue < 10) 
    {
        bCriticalHit = TRUE;
    } 

    //var pSkillInfo = new(gamedata.ST_SkillInfo)
    //    pSkillInfo.Hurts = make([]gamedata.ST_Hurts, 1, 1)
    //    pSkillInfo.Hurts[0].Percent = 100
    //   pSkillInfo.Hurts[0].Fixed = 0

    //最终伤害加成
    UINT32 dwFinalAdd = pAttacker->m_dwProperty[6] - pDefender->m_dwProperty[5] + 1000;

    //伤害随机
    UINT32 dwFightRand = 900 + CommonFunc::GetRandNum(1)%200;
    //hurt := (pSkillInfo.Hurts[0].Percent*(pAttacker.CurProperty[pAttacker.AttackPID-1]-pDefender.CurProperty[pAttacker.AttackPID]) + pSkillInfo.Hurts[0].Fixed)
    //UINT32 dwHurt := pAttacker->m_dwProperty[pAttacker.AttackPID-1] - pDefender->m_dwProperty[pAttacker.AttackPID]
    UINT32 dwHurt = pAttacker->m_dwProperty[1] - pDefender->m_dwProperty[1];
    if (dwHurt <= 0)
    {
        dwHurt = 1;
    } 
    else 
    {
        dwHurt = dwHurt * dwFightRand / 1000;
        dwHurt = dwHurt * dwFinalAdd / 1000;
        if (bCriticalHit) 
        {
            dwHurt = dwHurt * 15 / 10;
        }
    }

    pDefender->m_dwHp -= dwHurt;
    if (pDefender->m_dwHp <= 0) 
    {
        pDefender->m_dwHp = 0;
    }

    return TRUE;
}

