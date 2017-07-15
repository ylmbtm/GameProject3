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
CScene::CScene()
{
	m_dwCopyID = 0;		//当前副本实例ID
	m_dwCopyType = 0;   //当前副本TYPE
	m_dwLogicType = 0;  //逻辑类型
	m_pSceneLogic = NULL;
	m_dwPlayerNum = 0;
	m_dwLoginNum = 0;

}

CScene::~CScene()
{

}

BOOL CScene::Init(UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwLogicType,UINT32 dwPlayerNum)
{
	m_dwCopyID = dwCopyID;

	m_dwCopyType = dwCopyType;

	m_dwLogicType = dwLogicType;

	CreateSceneLogic(dwLogicType);

	m_dwPlayerNum  = dwPlayerNum;

	m_dwLoginNum = 0;

	m_pMonsterCreator = new MonsterCreator(this);


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

	DestroySceneLogic(m_dwLogicType);

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
		PROCESS_MESSAGE_ITEM(MSG_ROLE_MOVE_REQ,			OnMsgRoleMoveReq);
		PROCESS_MESSAGE_ITEM(MSG_DISCONNECT_NTY,		OnMsgRoleDisconnect);
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
	
	CSceneObject *pSceneObj = GetPlayer(Req.objectid());
	ERROR_RETURN_TRUE(pSceneObj != NULL);
	
    pSceneObj->m_dwObjState =  Req.objstate();
    pSceneObj->x = Req.x();
    pSceneObj->z = Req.z();
    pSceneObj->vx = Req.vx();
    pSceneObj->vz = Req.vz();
    pSceneObj->SetChanged();

	CLog::GetInstancePtr()->AddLog("响应客户端坐标请求[%lld], 坐标 x =%f, z=%f", pSceneObj->GetObjectID(), pSceneObj->x, pSceneObj->z);

	return TRUE;
}

BOOL CScene::OnMsgRoleAttack(NetPacket *pNetPacket)
{


	return TRUE;
}


BOOL CScene::OnMsgRoleDisconnect(NetPacket *pNetPacket)
{
	RoleDisconnectReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

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

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL)
        
        if(!pOther->IsConnected())
        {
            continue;
        }

        if(pOther->GetObjectID() == pSceneObject->GetObjectID())
        {
            continue;
        }
        
         pOther->SendProtoBuf(MSG_OBJECT_NEW_NTY, Nty);
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


	if(IsPlayerDataReady())
	{
		m_pMonsterCreator->OnUpdate(dwTick);
	}

    m_pSceneLogic->Update(dwTick);

	return TRUE;
}

BOOL CScene::CreateSceneLogic(UINT32 dwLogicType)
{
	switch (dwLogicType)
	{
	case SLT_NORMAL:
		m_pSceneLogic = new SceneLogic_Normal(this);
		break;

	default:
		{
			return false;
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
	default:
		{
			return false;
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
		pObject = new CSceneObject;
		pObject->m_dwType = OT_PLAYER;
		pObject->m_dwObjType = Req.roletype();
		pObject->m_strName = Req.rolename();
		pObject->m_uID = pHeader->u64TargetID;
		AddPlayer(pObject);
	}
	//根据数据创建宠物，英雄
	pObject->m_dwType = OT_PLAYER;
	pObject->m_dwObjType = Req.roletype();
	pObject->m_strName = Req.rolename();
	pObject->m_uID = pHeader->u64TargetID;

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

	pSceneObj->SetConnectID(pNetPacket->m_dwConnID, pHeader->u64TargetID);
	pSceneObj->SetEnterCopy();
	m_pSceneLogic->OnPlayerEnter(pSceneObj);

	m_dwLoginNum ++;

	//发比较全的自己的信息
	EnterSceneAck Ack;
	Ack.set_copyid(m_dwCopyID);
	Ack.set_copytype(m_dwCopyType);
	Ack.set_roleid(Req.roleid());
	Ack.set_rolename(pSceneObj->m_strName);
	Ack.set_roletype(pSceneObj->m_dwObjType);
	Ack.set_retcode(MRC_SUCCESSED);

	pSceneObj->SendProtoBuf(MSG_ENTER_SCENE_ACK, Ack);
    SendAllNewObjectToPlayer(pSceneObj);
    BroadNewObject(pSceneObj);
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

        if(pOther->GetObjectID() == pSceneObject->GetObjectID())
        {
            continue;
        }

        pOther->SaveNewObject(Nty);
    }

    pSceneObject->SendProtoBuf(MSG_OBJECT_NEW_NTY, Nty);

    return TRUE;
}

BOOL CScene::GetPlayerCount()
{
	return m_PlayerMap.size();
}

BOOL CScene::BroadRemoveObject( CSceneObject *pSceneObject )
{
    ObjectRemoveNty Nty;
    Nty.add_removelist(pSceneObject->GetObjectID());

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->GetObjectID() == pSceneObject->GetObjectID())
        {
            continue;
        }

        if(!pOther->IsConnected())
        {
            continue;
        }

        pOther->SendProtoBuf(MSG_OBJECT_REMOVE_NTY, Nty);
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

	ERROR_RETURN_FALSE(pSceneObject->GetObjectID() != 0);

    m_PlayerMap.insert(std::make_pair(pSceneObject->GetObjectID(), pSceneObject));

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

	return NULL;
}

BOOL CScene::IsFinished()
{
	return m_pSceneLogic->IsFinished();
}

BOOL CScene::IsPlayerDataReady()
{
	if(m_PlayerMap.size() == m_dwPlayerNum)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CScene::IsPlayerLoginReady()
{
	if(!IsPlayerDataReady())
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

BOOL CScene::SyncObjectState()
{
    ObjectUpdateNty Nty;
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pObj = itor->second;
        ASSERT(pObj != NULL);

        pObj->SaveUpdateObject(Nty);
    }

    if(Nty.updatelist_size() <= 0)
    {
        for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
        {
            CSceneObject *pObj = itor->second;
            ASSERT(pObj != NULL);

            if(!pObj->IsConnected())
            {
                continue;
            }

            pObj->SendProtoBuf(MSG_OBJECT_UPDATE_NTY, Nty);
        }
    }

    return TRUE;
}

