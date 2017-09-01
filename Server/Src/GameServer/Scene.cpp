#include "stdafx.h"
#include "CommandDef.h"
#include "GameService.h"
#include "Scene.h"
#include "PacketHeader.h"
#include "Log.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "DataBuffer.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Game_Define.pb.h"
#include "../Message/Msg_Move.pb.h"
#include "../Message/Msg_Copy.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "SceneLogic/SceneLogic_Normal.h"
#include "SceneLogic/SceneLogic_None.h"
#include "SceneLogic/SceneLogic_City.h"
#include "MonsterCreator.h"
#include "RapidXml.h"
#include "SceneXmlMgr.h"
#include "CommonConvert.h"
#include "../ConfigData/ConfigStruct.h"
#include "../ConfigData/ConfigData.h"
#include "../ServerData/ServerDefine.h"


CScene::CScene()
{
}

CScene::~CScene()
{
}

BOOL CScene::Init(UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCopyType, UINT32 dwPlayerNum)
{
	m_dwCopyGuid		= dwCopyGuid;
	m_dwCopyID			= dwCopyID;
	m_dwCopyType		= dwCopyType;
	m_dwPlayerNum		= dwPlayerNum;
	m_dwLoginNum		= 0;
	m_dwStartTime		= 0;
	m_uMaxGuid			= 0;
	m_dwCreateTime		= CommonFunc::GetCurrTime();
	m_pMonsterCreator	= new MonsterCreator(this);

	ERROR_RETURN_FALSE(CreateSceneLogic(dwCopyType));
	ERROR_RETURN_FALSE(ReadSceneXml());

	return TRUE;
}

BOOL CScene::Uninit()
{
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		delete pObj;
	}

	m_PlayerMap.clear();
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		delete pObj;
	}
	m_MonsterMap.clear();

	delete m_pMonsterCreator;

	ERROR_RETURN_FALSE(DestroySceneLogic(m_dwCopyType));

	return TRUE;
}

BOOL CScene::DispatchPacket(NetPacket* pNetPacket)
{
	PacketHeader* pPacketHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);

	switch(pPacketHeader->dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_TRANSFER_DATA_REQ,     OnMsgTransRoleDataReq);
			PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,		OnMsgEnterSceneReq);
			PROCESS_MESSAGE_ITEM(MSG_LEAVE_SCENE_REQ,		OnMsgLeaveSceneReq);
			PROCESS_MESSAGE_ITEM(MSG_DISCONNECT_NTY,		OnMsgRoleDisconnect);
			PROCESS_MESSAGE_ITEMEX(MSG_OBJECT_ACTION_REQ,	OnMsgObjectActionReq);
			PROCESS_MESSAGE_ITEM(MSG_HEART_BEAT_REQ,		OnMsgHeartBeatReq);
			PROCESS_MESSAGE_ITEM(MSG_USE_HP_BOOTTLE_REQ,	OnMsgUseHpBottleReq);
			PROCESS_MESSAGE_ITEM(MSG_USE_MP_BOOTTLE_REQ,	OnMsgUseMpBottleReq);
			PROCESS_MESSAGE_ITEM(MSG_BATTLE_CHAT_REQ,	    OnMsgBattleChatReq);
		default:
		{
			return FALSE;
		}
		break;
	}

	return TRUE;
}


BOOL CScene::OnMsgObjectActionReq( NetPacket* pNetPacket )
{
	ObjectActionReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	for(int i = 0; i < Req.actionlist_size(); i++)
	{
		const ActionItem& Item = Req.actionlist(i);
		ProcessActionItem(Item);
	}

	return TRUE;
}

BOOL CScene::OnMsgRoleDisconnect(NetPacket* pNetPacket)
{
	RoleDisconnectReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject* pPlayer = GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pPlayer != NULL);
	pPlayer->SetConnectID(0, 0);

	UpdateAiController(pPlayer->GetObjectGUID());

	return TRUE;
}

BOOL CScene::OnMsgHeartBeatReq(NetPacket* pNetPacket)
{
	HeartBeatReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject* pPlayer = GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_TRUE(pPlayer != NULL);

	HeartBeatAck Ack;
	Ack.set_timestamp(Req.timestamp());
	Ack.set_servertime(0);

	pPlayer->SendMsgProtoBuf(MSG_HEART_BEAT_ACK, Ack);

	return TRUE;
}

BOOL CScene::OnMsgUseHpBottleReq(NetPacket* pNetPacket)
{
	UseHpBottleReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject* pObject = GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_TRUE(pObject != NULL);



	return TRUE;
}

BOOL CScene::OnMsgUseMpBottleReq(NetPacket* pNetPacket)
{
	UseMpBottleReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject* pObject = GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_TRUE(pObject != NULL);


	return TRUE;
}

BOOL CScene::BroadNewObject(CSceneObject* pSceneObject)
{
	if(GetConnectCount() <= 0)
	{
		return TRUE;
	}

	//先把玩家的完整包组装好
	ObjectNewNty Nty;
	pSceneObject->SaveNewObject(Nty);

	char szBuff[10240] = {0};
	Nty.SerializePartialToArray(szBuff, Nty.ByteSize());

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pOther = itor->second;
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

//玩家主动中止副本
BOOL CScene::OnMsgLeaveSceneReq(NetPacket* pNetPacket)
{
	LeaveSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject* pSceneObject = GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pSceneObject != NULL);
	//只有主城和不结算的pvp副本需要允许删除玩家信息
	if(m_pSceneLogic->OnPlayerLeave(pSceneObject))
	{
		BroadRemoveObject(pSceneObject);
		DeletePlayer(Req.roleid());
	}
	return TRUE;
}



BOOL CScene::OnUpdate( UINT32 dwTick )
{
	if(m_pSceneLogic->IsFinished())
	{
		//己经结束不再处理
		return TRUE;
	}

	//同步所有对象的状态
	SyncObjectState();

	//把玩家死亡都同步一下
	for(auto itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); ++itor)
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_CONTINUE_EX(pSceneObject != NULL);
		if(pSceneObject->GetHp() <= 0 && pSceneObject->m_dwObjState != OS_DIE)
		{
			pSceneObject->m_dwObjState = OS_DIE;
			m_pSceneLogic->OnObjectDie(pSceneObject);
			BroadDieNotify(pSceneObject->GetObjectGUID());
		}
	}

	//把怪物死亡同步一下
	for (auto itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); )
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_CONTINUE_EX(pSceneObject != NULL);
		if(pSceneObject->GetHp() <= 0 && pSceneObject->m_dwObjState != OS_DIE)
		{
			pSceneObject->m_dwObjState = OS_DIE;
			m_pMonsterCreator->OnObjectDie(pSceneObject);
			m_pSceneLogic->OnObjectDie(pSceneObject);
			BroadDieNotify(pSceneObject->GetObjectGUID());
			itor = m_MonsterMap.erase(itor);
		}
		else
		{
			itor++;
		}
	}

	m_pMonsterCreator->OnUpdate(dwTick);

	m_pSceneLogic->Update(dwTick);

	return TRUE;

}

BOOL CScene::CreateSceneLogic(UINT32 dwCopyType)
{
	ERROR_RETURN_FALSE(dwCopyType != 0);
	switch (dwCopyType)
	{
		case CPT_MAIN:
			m_pSceneLogic = new SceneLogic_Normal(this);
			break;

		case CPT_CITY:
			m_pSceneLogic = new SceneLogic_City(this);
			break;
		default:
		{
			m_pSceneLogic = new SceneLogic_None(this);
		}
	}

	return (m_pSceneLogic != NULL);
}

BOOL CScene::DestroySceneLogic(UINT32 dwCopyType)
{
	switch (dwCopyType)
	{
		case CPT_MAIN:
		{
			SceneLogic_Normal* pTemp = (SceneLogic_Normal*)m_pSceneLogic;
			delete pTemp;
		}
		break;
		case CPT_CITY:
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

BOOL CScene::OnMsgTransRoleDataReq(NetPacket* pNetPacket)
{
	ERROR_RETURN_FALSE(m_dwCopyGuid != 0);
	ERROR_RETURN_FALSE(m_dwCopyID != 0);
	ERROR_RETURN_FALSE(m_dwCopyType != 0);
	ERROR_RETURN_FALSE(CGameService::GetInstancePtr()->GetServerID() != 0);

	TransferDataReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);
	ERROR_RETURN_TRUE(pHeader->u64TargetID == Req.roledata().roleid());

	CreatePlayer(Req.roledata(), pHeader->u64TargetID, Req.camp());

	//是否有宠物
	if(Req.has_petdata())
	{
		CreatePet(Req.petdata(), pHeader->u64TargetID, Req.camp());
	}

	//是否有伙伴
	if(Req.has_partnerdata())
	{
		CreatePartner(Req.partnerdata(), pHeader->u64TargetID, Req.camp());
	}

	//检查人齐没齐，如果齐了，就全部发准备好了的消息
	//有的副本不需要等人齐，有人就可以进

	TransferDataAck Ack;
	Ack.set_copyguid(m_dwCopyGuid);
	Ack.set_copyid(m_dwCopyID);
	Ack.set_roleid(pHeader->u64TargetID);
	Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_TRANSFER_DATA_ACK, pHeader->u64TargetID, 0, Ack);
	return TRUE;
}

BOOL CScene::OnMsgEnterSceneReq(NetPacket* pNetPacket)
{
	EnterSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	CSceneObject* pSceneObj = GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pSceneObj != NULL);

	pSceneObj->SetConnectID(pNetPacket->m_dwConnID, (UINT32)pHeader->u64TargetID);
	pSceneObj->SetEnterCopy();
	m_pSceneLogic->OnPlayerEnter(pSceneObj);

	m_dwLoginNum ++;

	//发比较全的自己的信息
	EnterSceneAck Ack;
	Ack.set_copyguid(m_dwCopyGuid);
	Ack.set_copyid(m_dwCopyID);
	Ack.set_roleid(Req.roleid());
	Ack.set_rolename(pSceneObj->m_strName);
	Ack.set_actorid(pSceneObj->m_dwActorID);
	Ack.set_retcode(MRC_SUCCESSED);

	Ack.set_hp(pSceneObj->m_Propertys[HP]);
	Ack.set_mp(pSceneObj->m_Propertys[MP]);
	Ack.set_hpmax(pSceneObj->m_Propertys[HP_MAX]);
	Ack.set_mpmax(pSceneObj->m_Propertys[MP_MAX]);

	Ack.set_camp(pSceneObj->m_dwCamp);

	Ack.set_x(pSceneObj->m_x);
	Ack.set_y(pSceneObj->m_y);
	Ack.set_z(pSceneObj->m_z);
	Ack.set_ft(0);

	pSceneObj->SendMsgProtoBuf(MSG_ENTER_SCENE_ACK, Ack);
	SendAllNewObjectToPlayer(pSceneObj);
	m_dwStartTime = CommonFunc::GetCurrTime();
	return TRUE;
}

UINT32 CScene::GetCopyGuid()
{
	return m_dwCopyGuid;
}

UINT32 CScene::GetCopyID()
{
	return m_dwCopyID;
}

UINT32 CScene::GetCopyType()
{
	return m_dwCopyType;
}

BOOL CScene::SendAllNewObjectToPlayer( CSceneObject* pSceneObject )
{
	//先把玩家的完整包组装好
	ObjectNewNty Nty;

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pOther = itor->second;
		ERROR_RETURN_FALSE(pOther != NULL);

		if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
		{
			continue;
		}

		if(pOther->GetObjType() == OT_ROBOT)
		{
			if(pOther->m_uControlerID == 0)
			{
				pOther->m_uControlerID = pSceneObject->GetObjectGUID();
			}
		}


		pOther->SaveNewObject(Nty);
	}

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); itor++)
	{
		CSceneObject* pOther = itor->second;
		ERROR_RETURN_FALSE(pOther != NULL);

		if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
		{
			continue;
		}

		if(pOther->m_uControlerID == 0)
		{
			pOther->m_uControlerID = pSceneObject->GetObjectGUID();
		}

		pOther->SaveNewObject(Nty);
	}

	if(Nty.newlist_size() <= 0)
	{
		return TRUE;
	}

	pSceneObject->SendMsgProtoBuf(MSG_OBJECT_NEW_NTY, Nty);

	return TRUE;
}

BOOL CScene::BroadDieNotify(UINT64 uObjectID)
{
	ObjectDieNotify Nty;
	Nty.set_objectguid(uObjectID);
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_RETURN_FALSE(pSceneObject != NULL);

		if(pSceneObject->IsConnected())
		{
			pSceneObject->SendMsgProtoBuf(MSG_OBJECT_DIE_NOTIFY, Nty);
		}
	}

	return TRUE;
}

INT32 CScene::GetPlayerCount()
{
	return (INT32)m_PlayerMap.size();
}

INT32 CScene::GetConnectCount()
{
	INT32 nCount = 0;
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pOther = itor->second;
		ERROR_RETURN_FALSE(pOther != NULL);

		if(pOther->IsConnected())
		{
			nCount += 1;
		}
	}

	return nCount;
}

BOOL CScene::BroadRemoveObject( CSceneObject* pSceneObject )
{
	ObjectRemoveNty Nty;
	Nty.add_removelist(pSceneObject->GetObjectGUID());

	char szBuff[10240] = {0};
	Nty.SerializePartialToArray(szBuff, Nty.ByteSize());

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pOther = itor->second;
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

BOOL CScene::AddPlayer( CSceneObject* pSceneObject )
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

BOOL CScene::AddMonster(CSceneObject* pSceneObject)
{
	ERROR_RETURN_FALSE(pSceneObject != NULL);

	ERROR_RETURN_FALSE(pSceneObject->GetObjectGUID() != 0);

	m_MonsterMap.insert(std::make_pair(pSceneObject->GetObjectGUID(), pSceneObject));

	return TRUE;

}

VOID CScene::DeleteMonster(UINT64 uID)
{
	std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.find(uID);
	if(itor != m_PlayerMap.end())
	{
		m_PlayerMap.erase(itor);
	}
	else
	{
		CLog::GetInstancePtr()->LogError("Error CScene::DeleteMonster cannot find player id:%d", uID);
	}
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

BOOL CScene::UpdateAiController(UINT64 uFilterID)
{
	UINT64 u64ControllerID = SelectController(uFilterID);
	if(u64ControllerID == 0)
	{
		return FALSE;
	}

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pOther = itor->second;
		ERROR_RETURN_FALSE(pOther != NULL);

		if(pOther->GetObjectGUID() == uFilterID)
		{
			continue;
		}

		if(pOther->GetObjType() == OT_ROBOT)
		{
			if(pOther->m_uControlerID == uFilterID)
			{
				pOther->m_uControlerID = u64ControllerID;
			}
		}
	}

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); itor++)
	{
		CSceneObject* pOther = itor->second;
		ERROR_RETURN_FALSE(pOther != NULL);

		if(pOther->m_uControlerID == uFilterID)
		{
			pOther->m_uControlerID = u64ControllerID;
		}
	}

	return TRUE;
}

UINT64 CScene::SelectController(UINT64 uFilterID)
{
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pOther = itor->second;
		ERROR_RETURN_FALSE(pOther != NULL);
		if(pOther->GetObjectGUID() == uFilterID)
		{
			continue;
		}

		if(pOther->GetObjType() == OT_ROBOT)
		{
			continue;
		}

		if(!pOther->IsConnected())
		{
			continue;
		}

		return pOther->GetObjectGUID();
	}

	return 0;
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
		CSceneObject* pObj = itor->second;
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

UINT32 CScene::GetLastTick()
{
	return m_dwLastTick;
}

BOOL CScene::SetLastTick(UINT32 dwTick)
{
	m_dwLastTick = dwTick;
	return TRUE;
}

UINT64 CScene::GenNewGuid()
{
	m_uMaxGuid += 1;
	return m_uMaxGuid;
}

BOOL CScene::SyncObjectState()
{
	if(m_ObjectActionNty.actionlist_size() <= 0)
	{
		return TRUE;
	}

	char szBuff[102400] = {0};
	m_ObjectActionNty.SerializePartialToArray(szBuff, m_ObjectActionNty.ByteSize());

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		ASSERT(pObj != NULL);

		if(!pObj->IsConnected())
		{
			continue;
		}

		pObj->SendMsgRawData(MSG_OBJECT_ACTION_NTY, szBuff, m_ObjectActionNty.ByteSize());
	}

	m_ObjectActionNty.Clear();

	return TRUE;
}

BOOL CScene::CreateMonster( UINT32 dwActorID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft)
{
	StActor* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(dwActorID);
	ERROR_RETURN_FALSE(pActorInfo != NULL);
	CSceneObject* pObject = new CSceneObject(GenNewGuid(), dwActorID, OT_MONSTER, dwCamp, pActorInfo->strName);
	for(int i = 0; i < PROPERTY_NUM; i++)
	{
		pObject->m_Propertys[i] = pActorInfo->Propertys[i];
	}

	pObject->m_Propertys[HP] = pObject->m_Propertys[HP_MAX];
	pObject->m_Propertys[MP] = pObject->m_Propertys[MP_MAX];

	m_pSceneLogic->OnObjectCreate(pObject);
	AddMonster(pObject);
	BroadNewObject(pObject);
	return TRUE;
}

BOOL CScene::CreatePlayer(const TransRoleData& roleData, UINT64 uHostID, UINT32 dwCamp )
{
	CSceneObject* pObject = GetPlayer(roleData.roleid());
	if(pObject == NULL)
	{
		pObject = new CSceneObject(roleData.roleid(), roleData.actorid(), OT_PLAYER, 2, (std::string&)roleData.name());
		AddPlayer(pObject);
	}
	//根据数据创建宠物，英雄
	pObject->m_dwObjType = OT_PLAYER;
	pObject->m_dwActorID = roleData.actorid();
	pObject->m_strName = roleData.name();
	pObject->m_uGuid = roleData.roleid();
	pObject->m_dwCamp = dwCamp;

	for(int i = 0; i < roleData.propertys_size(); i++)
	{
		pObject->m_Propertys[i] = roleData.propertys(i);
	}
	pObject->m_Propertys[HP] = pObject->m_Propertys[HP_MAX];
	pObject->m_Propertys[MP] = pObject->m_Propertys[MP_MAX];

	m_pSceneLogic->OnObjectCreate(pObject);
	BroadNewObject(pObject);
	return TRUE;
}

BOOL CScene::CreatePet(const TransPetData& petData, UINT64 uHostID, UINT32 dwCamp )
{
	StActor* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(petData.actorid());
	ERROR_RETURN_FALSE(pActorInfo != NULL);
	CSceneObject* pObject = new CSceneObject(petData.petguid(), petData.actorid(), OT_PET, dwCamp, pActorInfo->strName);
	pObject->m_uHostGuid = uHostID;
	m_pSceneLogic->OnObjectCreate(pObject);
	AddMonster(pObject);
	BroadNewObject(pObject);
	return TRUE;
}

BOOL CScene::CreatePartner(const TransPartnerData& partnerData, UINT64 uHostID, UINT32 dwCamp  )
{
	StActor* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(partnerData.actorid());
	ERROR_RETURN_FALSE(pActorInfo != NULL);
	CSceneObject* pObject = new CSceneObject(partnerData.partnerguid(), partnerData.actorid(), OT_PARTNER, dwCamp, pActorInfo->strName);
	pObject->m_uHostGuid = uHostID;
	m_pSceneLogic->OnObjectCreate(pObject);
	AddMonster(pObject);
	BroadNewObject(pObject);
	return TRUE;
}

BOOL CScene::CreateSummon(UINT32 dwActorID, UINT64 uSummonerID, UINT32 dwCamp, FLOAT x, FLOAT y)
{
	StActor* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(dwActorID);
	ERROR_RETURN_FALSE(pActorInfo != NULL);
	CSceneObject* pObject = new CSceneObject(GenNewGuid(), dwActorID, OT_SUMMON, dwCamp, pActorInfo->strName);
	pObject->m_uSummonerID = uSummonerID;
	m_pSceneLogic->OnObjectCreate(pObject);
	AddMonster(pObject);
	BroadNewObject(pObject);
	return TRUE;
}

BOOL CScene::IsCampAllDie(UINT32 dwCamp)
{
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		if((pObj != NULL) && (pObj->GetCamp() == dwCamp))
		{
			if(!pObj->IsDie() && pObj->m_bIsCampCheck)
			{
				return FALSE;
			}
		}
	}

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		if((pObj != NULL) && (pObj->GetCamp() == dwCamp))
		{
			if(!pObj->IsDie() && pObj->m_bIsCampCheck)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CScene::IsMonsterAllDie()
{
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		if((pObj != NULL) && (pObj->GetObjType() == OT_MONSTER))
		{
			if(!pObj->IsDie() && pObj->m_bIsMonsCheck)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CScene::ReadSceneXml()
{
	//return TRUE;
	StCopyInfo* pCopyInfo = CConfigData::GetInstancePtr()->GetCopyInfo(m_dwCopyID);
	ERROR_RETURN_FALSE(pCopyInfo != NULL);
	pCopyInfo->strXml = CommonConvert::IntToString(m_dwCopyID) + ".xml";
	rapidxml::xml_document<char>* pXmlDoc = CSceneXmlManager::GetInstancePtr()->GetXmlDocument(pCopyInfo->strXml);
	ERROR_RETURN_FALSE(pXmlDoc != NULL);

	rapidxml::xml_node<char>* pXmlRoot = pXmlDoc->first_node("Root");
	ERROR_RETURN_FALSE(pXmlRoot != NULL);

	//auto pLogicNode = pXmlRoot->first_node("MapLogic");
	//ERROR_RETURN_FALSE(pLogicNode != NULL);
	//ERROR_RETURN_FALSE(m_pSceneLogic != NULL);
	//ERROR_RETURN_FALSE(m_pSceneLogic->ReadFromXml(pLogicNode));

	auto pBornNode = pXmlRoot->first_node("MapBorns");
	ERROR_RETURN_FALSE(pBornNode != NULL);
	ERROR_RETURN_FALSE(m_pSceneLogic != NULL);
	ERROR_RETURN_FALSE(m_pSceneLogic->ReadFromXml(pBornNode));

	auto pCreatorNode = pXmlRoot->first_node("MapActions");
	ERROR_RETURN_FALSE(pCreatorNode != NULL);
	ERROR_RETURN_FALSE(m_pMonsterCreator != NULL);
	ERROR_RETURN_FALSE(m_pMonsterCreator->ReadFromXml(pCreatorNode));

	return TRUE;
}

CSceneObject* CScene::GetOwnPlayer()
{
	if((m_PlayerMap.size() < 1) || (m_PlayerMap.size() > 1))
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

BOOL CScene::SkillFight( CSceneObject* pAttacker, UINT32 dwSkillID, CSceneObject* pDefender )
{
	ERROR_RETURN_FALSE(pAttacker != NULL);
	ERROR_RETURN_FALSE(pDefender != NULL);
	ERROR_RETURN_FALSE(dwSkillID != 0);

	StSkillInfo* pSkillInfo = CConfigData::GetInstancePtr()->GetSkillInfo(dwSkillID, 1);
	ERROR_RETURN_FALSE(pSkillInfo != NULL);

	UINT32 dwCurTime = CommonFunc::GetTickCount();

	UINT32 dwTime = pAttacker->GetLastSkillTime(dwSkillID);
	if(dwCurTime - dwTime < pSkillInfo->CD * 1000)
	{
		CLog::GetInstancePtr()->LogError("玩家作弊!!!");

		return FALSE;
	}

	StBuffInfo* pBuffInfo = CConfigData::GetInstancePtr()->GetBuffInfo(pSkillInfo->SelfBuffID);
	ERROR_RETURN_FALSE(pBuffInfo != NULL);

	pAttacker->AddBuff(pSkillInfo->SelfBuffID);

	pDefender->AddBuff(pSkillInfo->TargetBuffID);

	UINT32 dwRandValue = CommonFunc::GetRandNum(1);
	//先判断是否命中
	if (dwRandValue > (800 + pAttacker->m_Propertys[8] - pDefender->m_Propertys[7]) && dwRandValue > 500)
	{
		//未命中
		return TRUE;
	}

	//判断是否爆击
	dwRandValue = CommonFunc::GetRandNum(1);
	BOOL bCriticalHit = FALSE;
	if (dwRandValue < (pAttacker->m_Propertys[9] - pAttacker->m_Propertys[10]) || dwRandValue < 10)
	{
		bCriticalHit = TRUE;
	}

	//最终伤害加成
	UINT32 dwFinalAdd = pAttacker->m_Propertys[6] - pDefender->m_Propertys[5] + 1000;

	//伤害随机
	UINT32 dwFightRand = 900 + CommonFunc::GetRandNum(1) % 200;
	INT32 hurt = (pSkillInfo->Percent * (pAttacker->m_Propertys[5] - pDefender->m_Propertys[6]) + pSkillInfo->Fix);
	UINT32 dwHurt = pAttacker->m_Propertys[1] - pDefender->m_Propertys[1];
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

	pDefender->m_Propertys[HP] -= dwHurt;
	if (pDefender->m_Propertys[HP] <= 0)
	{
		pDefender->m_Propertys[HP] = 0;
	}

	return TRUE;
}

BOOL CScene::ProcessActionItem( const  ActionItem& Item )
{
	CSceneObject* pSceneObj = GetPlayer(Item.objectguid());
	ERROR_RETURN_TRUE(pSceneObj != NULL);
	pSceneObj->m_x = Item.x();
	pSceneObj->m_y = Item.y();
	pSceneObj->m_z = Item.z();
	pSceneObj->m_ft = Item.ft();

	if(Item.actionstate()&AS_HURT)
	{
		for(int i = 0; i < Item.damagerlist_size(); i++)
		{
			const DamagerItem& damager  = Item.damagerlist(i);
			CSceneObject* pDamager = GetPlayer(damager.objectguid());
			if(pDamager == NULL)
			{
				CLog::GetInstancePtr()->LogError("Error: CScene::ProcessActionItem Can not find Damager id:%d", damager.objectguid());
				continue;
			}

			SkillFight(pSceneObj, Item.actionid(), pDamager);
		}
	}

	ActionItem* pSvrItem = m_ObjectActionNty.add_actionlist();
	ERROR_RETURN_TRUE(pSvrItem != NULL);
	pSvrItem->CopyFrom(Item);
	pSvrItem->set_hp(pSceneObj->GetHp());
	pSvrItem->set_mp(pSceneObj->GetMp());
	pSvrItem->set_hpmax(1000);
	pSvrItem->set_mpmax(1000);

	return TRUE;
}

BOOL CScene::SendBattleResult()
{
	BattleResultNty Nty;

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		ERROR_RETURN_FALSE(pObj != NULL);
		ResultPlayer* pPlayer = Nty.add_playerlist();
		pObj->SaveBattleResult(pPlayer);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_BATTLE_RESULT_NTY, 0, 0, Nty);

	return TRUE;
}

BOOL CScene::OnMsgBattleChatReq( NetPacket* pNetPacket )
{
	return TRUE;
}



