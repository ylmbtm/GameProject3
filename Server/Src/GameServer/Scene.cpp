#include "stdafx.h"
#include "GameService.h"
#include "Scene.h"
#include "PacketHeader.h"
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
#include "../ConfigData/ConfigStruct.h"
#include "../ConfigData/ConfigData.h"
#include "GameObject/SkillObject.h"
#include "GameObject/BulletObject.h"


CScene::CScene()
{
	m_dwCopyGuid		= 0;
	m_dwCopyID			= 0;
	m_dwCopyType		= 0;
	m_dwPlayerNum		= 0;
	m_uCreateKey		= 0;
	m_dwLoginNum		= 0;
	m_uStartTime		= 0;
	m_uMaxGuid			= 0;
	m_uCreateTime		= 0;
	m_pMonsterCreator	= NULL;
	m_bFinished			= FALSE;
}

CScene::~CScene()
{
	m_dwCopyGuid		= 0;
	m_dwCopyID			= 0;
	m_dwCopyType		= 0;
	m_dwPlayerNum		= 0;
	m_uCreateKey		= 0;
	m_dwLoginNum		= 0;
	m_uStartTime		= 0;
	m_uMaxGuid			= 0;
	m_uCreateTime		= 0;
	m_pMonsterCreator	= NULL;
	m_bFinished			= FALSE;
}

BOOL CScene::Init(UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCopyType, UINT32 dwPlayerNum, UINT64 uCreateKey)
{
	m_dwCopyGuid		= dwCopyGuid;
	m_dwCopyID			= dwCopyID;
	m_dwCopyType		= dwCopyType;
	m_dwPlayerNum		= dwPlayerNum;
	m_uCreateKey		= uCreateKey;
	m_dwLoginNum		= 0;
	m_uStartTime		= 0;
	m_uMaxGuid			= 0;
	m_uCreateTime		= CommonFunc::GetCurrTime();
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
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_TRANSFER_DATA_REQ,     OnMsgTransRoleDataReq);
			PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,		OnMsgEnterSceneReq);
			PROCESS_MESSAGE_ITEM(MSG_LEAVE_SCENE_REQ,		OnMsgLeaveSceneReq);
			PROCESS_MESSAGE_ITEM(MSG_DISCONNECT_NTY,		OnMsgRoleDisconnect);
			PROCESS_MESSAGE_ITEMEX(MSG_SKILL_CAST_REQ,		OnMsgSkillCastReq);
			PROCESS_MESSAGE_ITEMEX(MSG_OBJECT_ACTION_REQ,	OnMsgObjectActionReq);
			PROCESS_MESSAGE_ITEM(MSG_HEART_BEAT_REQ,		OnMsgHeartBeatReq);
			PROCESS_MESSAGE_ITEM(MSG_USE_HP_BOOTTLE_REQ,	OnMsgUseHpBottleReq);
			PROCESS_MESSAGE_ITEM(MSG_USE_MP_BOOTTLE_REQ,	OnMsgUseMpBottleReq);
			PROCESS_MESSAGE_ITEM(MSG_BATTLE_CHAT_REQ,	    OnMsgBattleChatReq);
			PROCESS_MESSAGE_ITEM(MSG_SCENEOBJ_CHAGE_NTF,	OnMsgObjectChangeNtf);

	}

	return FALSE;
}

BOOL CScene::ProcessActionItem(const  ActionReqItem& Item)
{
	CSceneObject* pSceneObj = GetPlayer(Item.objectguid());
	ERROR_RETURN_TRUE(pSceneObj != NULL);
	pSceneObj->ProcessAction(Item);
	return TRUE;
}

BOOL CScene::OnMsgObjectActionReq( NetPacket* pNetPacket )
{
	ObjectActionReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	for(int i = 0; i < Req.actionlist_size(); i++)
	{
		const ActionReqItem& Item = Req.actionlist(i);
		ProcessActionItem(Item);
	}

	return TRUE;
}

BOOL CScene::OnMsgSkillCastReq(NetPacket* pNetPacket)
{
	SkillCastReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject* pSceneObj = GetPlayer(Req.objectguid());
	ERROR_RETURN_TRUE(pSceneObj != NULL);

	UINT32 dwRetCode = pSceneObj->ProcessSkill(Req);
	if (dwRetCode != MRC_SUCCESSED)
	{
		SkillCastAck Ack;
		Ack.set_retcode(dwRetCode);
		pSceneObj->SendMsgProtoBuf(MSG_SKILL_CAST_ACK, Ack);
	}

	for (int i = 0; i < Req.targetobjects_size(); i++)
	{
		UINT64 uTargetID = Req.targetobjects(i);
		CSceneObject *pTargetObj = GetSceneObject(uTargetID);
		if (pTargetObj == NULL)
		{
			continue;
		}
		pTargetObj->SubHp(1);
	}

	return TRUE;
}

BOOL CScene::OnMsgObjectChangeNtf(NetPacket* pNetPacket)
{
	ObjectChangeNotify Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject* pPlayer = GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pPlayer != NULL);

	if (Req.changetype() == 1)
	{
		pPlayer->ChangeEquip((INT32)Req.intvalue1(), (UINT32)Req.intvalue2());
	}

	return TRUE;
}

BOOL CScene::OnMsgRoleDisconnect(NetPacket* pNetPacket)
{
	RoleDisconnectReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CSceneObject* pPlayer = GetPlayer(Req.roleid());
	if (pPlayer == NULL)
	{
		return TRUE;
	}

	pPlayer->SetConnectID(0, 0);

	UpdateAiController(pPlayer->GetObjectGUID());

	//ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_DISCONNECT_NTY, pHeader->u64TargetID, 0, Req);

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
	pSceneObject->SaveNewData(Nty);

	char szBuff[10240] = {0};
	ERROR_RETURN_FALSE(Nty.ByteSize() < 10240);
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

		pOther->SendMsgRawData(MSG_OBJECT_NEW_NTF, szBuff, Nty.ByteSize());
	}

	return TRUE;
}

BOOL CScene::BroadNewBullet(CBulletObject* pBulletObject)
{
	BulletNewNtf Ntf;
	pBulletObject->SaveNewData(Ntf);

	BroadMessage(MSG_BULLET_NEW_NTF, Ntf);

	return TRUE;
}

BOOL CScene::BroadMessage(UINT32 dwMsgID, const google::protobuf::Message& pdata)
{
	char szBuff[10240] = { 0 };
	ERROR_RETURN_FALSE(pdata.ByteSize() < 10240);
	ERROR_RETURN_FALSE(pdata.SerializePartialToArray(szBuff, pdata.ByteSize()));

	BroadMessageNotify Nty;
	Nty.set_msgdata(szBuff, pdata.ByteSize());
	Nty.set_msgid(dwMsgID);

	for (std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		ERROR_RETURN_FALSE(pObj != NULL);

		if (!pObj->IsConnected())
		{
			continue;
		}

		Nty.add_connid(pObj->m_dwClientConnID);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetProxyConnID(), MSG_BROAD_MESSAGE_NOTIFY, 0, 0, Nty);

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

BOOL CScene::OnUpdate( UINT64 uTick )
{
	if(IsFinished())
	{
		//己经结束不再处理
		return TRUE;
	}

	for(auto itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); ++itor)
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_CONTINUE_EX(pSceneObject != NULL);
		pSceneObject->OnUpdate(uTick);
	}

	for (auto itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); ++itor)
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_CONTINUE_EX(pSceneObject != NULL);
		pSceneObject->OnUpdate(uTick);
	}

	for (auto itor = m_BulletMap.begin(); itor != m_BulletMap.end();)
	{
		CBulletObject* pBulletObject = itor->second;
		ERROR_CONTINUE_EX(pBulletObject != NULL);
		pBulletObject->OnUpdate(uTick);

		if (pBulletObject->IsFinished())
		{
			itor = m_BulletMap.erase(itor);
		}
		else
		{
			++itor;
		}
	}

	//同步所有对象的状态
	SyncObjectState();

	//把玩家死亡都同步一下
	for(auto itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); ++itor)
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_CONTINUE_EX(pSceneObject != NULL);
		if(pSceneObject->GetHp() <= 0 && !pSceneObject->IsDead())
		{
			pSceneObject->SetDead(TRUE);
			m_pSceneLogic->OnObjectDie(pSceneObject);
			BroadDieNotify(pSceneObject->GetObjectGUID());
		}
	}

	//把怪物死亡同步一下
	for (auto itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); ++itor)
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_CONTINUE_EX(pSceneObject != NULL);
		if(pSceneObject->GetHp() <= 0 && !pSceneObject->IsDead())
		{
			pSceneObject->SetDead(TRUE);
			m_pMonsterCreator->OnObjectDie(pSceneObject);
			m_pSceneLogic->OnObjectDie(pSceneObject);
			BroadDieNotify(pSceneObject->GetObjectGUID());
		}
	}

	RemoveDeadObject();

	m_pMonsterCreator->OnUpdate(uTick);

	m_pSceneLogic->Update(uTick);

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

	Ack.set_x(pSceneObj->m_Pos.m_x);
	Ack.set_y(pSceneObj->m_Pos.m_y);
	Ack.set_z(pSceneObj->m_Pos.m_z);
	Ack.set_ft(0);

	for (int i = 0; i < EQUIP_MAX_NUM; i++)
	{
		Ack.add_equips(pSceneObj->m_Equips[i]);
	}

	pSceneObj->SendMsgProtoBuf(MSG_ENTER_SCENE_ACK, Ack);
	SendAllNewObjectToPlayer(pSceneObj);
	m_uStartTime = CommonFunc::GetCurrTime();
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


		pOther->SaveNewData(Nty);
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

		pOther->SaveNewData(Nty);
	}

	if(Nty.newlist_size() <= 0)
	{
		return TRUE;
	}

	pSceneObject->SendMsgProtoBuf(MSG_OBJECT_NEW_NTF, Nty);

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
	ERROR_RETURN_FALSE(Nty.ByteSize() < 10240);
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

		pOther->SendMsgRawData(MSG_OBJECT_REMOVE_NTF, szBuff, Nty.ByteSize());
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
		CSceneObject *pObject = itor->second;
		delete pObject;
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
	std::map<UINT64, CSceneObject*>::iterator itor = m_MonsterMap.find(uID);
	if(itor != m_MonsterMap.end())
	{
		CSceneObject *pObject = itor->second;
		delete pObject;
		m_MonsterMap.erase(itor);
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

BOOL CScene::RemoveDeadObject()
{
	for (auto itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); )
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_CONTINUE_EX(pSceneObject != NULL);
		if (pSceneObject->IsDead())
		{
			delete pSceneObject;
			itor = m_PlayerMap.erase(itor);
		}
		else
		{
			itor++;
		}
	}

	for (auto itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); )
	{
		CSceneObject* pSceneObject = itor->second;
		ERROR_CONTINUE_EX(pSceneObject != NULL);
		if (pSceneObject->IsDead())
		{
			delete pSceneObject;
			itor = m_MonsterMap.erase(itor);
		}
		else
		{
			itor++;
		}
	}

	return TRUE;
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
	return m_bFinished;
}

VOID CScene::SetFinished()
{
	m_bFinished = TRUE;
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

UINT64 CScene::GetStartTime()
{
	return m_uStartTime;
}

UINT64 CScene::GetCreateTime()
{
	return m_uCreateTime;
}

UINT64 CScene::GetTotalTime()
{
	return m_uTotalTime;
}

UINT64 CScene::GetLastTick()
{
	return m_dwLastTick;
}

BOOL CScene::SetLastTick(UINT64 uTick)
{
	m_dwLastTick = uTick;
	return TRUE;
}

UINT64 CScene::GenNewGuid()
{
	m_uMaxGuid += 1;
	return m_uMaxGuid;
}


BOOL CScene::SyncObjectState()
{
	ObjectActionNty ActionNty;
	for (std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		ERROR_RETURN_FALSE(pObj != NULL);

		if (!pObj->IsChanged())
		{
			continue;
		}

		pObj->SaveUpdateData(ActionNty);
	}


	for (std::map<UINT64, CSceneObject*>::iterator itor = m_MonsterMap.begin(); itor != m_MonsterMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		ERROR_RETURN_FALSE(pObj != NULL);

		if (!pObj->IsChanged())
		{
			continue;
		}

		pObj->SaveUpdateData(ActionNty);
	}

	if (ActionNty.actionlist_size() <= 0)
	{
		return TRUE;
	}

	char szBuff[10240] = {0};
	ERROR_RETURN_FALSE(ActionNty.ByteSize() < 10240);
	ERROR_RETURN_FALSE(ActionNty.SerializePartialToArray(szBuff, ActionNty.ByteSize()));

	BroadMessageNotify Nty;
	Nty.set_msgdata(szBuff, ActionNty.ByteSize());
	Nty.set_msgid(MSG_OBJECT_CHANGE_NTF);
	ActionNty.Clear();

	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		ERROR_RETURN_FALSE(pObj != NULL);
		if(!pObj->IsConnected())
		{
			continue;
		}

		Nty.add_connid(pObj->m_dwClientConnID);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetProxyConnID(), MSG_BROAD_MESSAGE_NOTIFY, 0, 0, Nty);

	return TRUE;
}


CSceneObject* CScene::CreateMonster(UINT32 dwActorID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft)
{
	StActorInfo* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(dwActorID);
	ERROR_RETURN_NULL(pActorInfo != NULL);
	CSceneObject* pObject = new CSceneObject(GenNewGuid(), this);

	pObject->m_dwCamp = dwCamp;
	pObject->m_dwObjType = OT_MONSTER;
	pObject->m_dwActorID = dwActorID;
	pObject->m_strName = pActorInfo->strName;
	pObject->m_dwLevel =  pActorInfo->Level;
	for(int i = 0; i < PROPERTY_NUM; i++)
	{
		pObject->m_Propertys[i] = pActorInfo->Propertys[i];
	}

	pObject->m_Propertys[HP] = pObject->m_Propertys[HP_MAX];
	pObject->m_Propertys[MP] = pObject->m_Propertys[MP_MAX];
	pObject->SetPos(x, y, z, ft);

	m_pSceneLogic->OnObjectCreate(pObject);
	AddMonster(pObject);
	BroadNewObject(pObject);
	return pObject;
}

CSceneObject* CScene::CreatePlayer(const TransRoleData& roleData, UINT64 uHostID, UINT32 dwCamp )
{
	CSceneObject* pObject = GetPlayer(roleData.roleid());
	if(pObject == NULL)
	{
		pObject = new CSceneObject(roleData.roleid(), this);
		AddPlayer(pObject);
	}
	//根据数据创建宠物，英雄
	pObject->m_dwCamp = dwCamp;
	pObject->m_dwObjType = OT_PLAYER;
	pObject->m_dwActorID = roleData.actorid();
	pObject->m_strName = roleData.name();
	pObject->m_dwLevel = roleData.level();

	for (int i = 0; i < roleData.equips_size(); i++)
	{
		pObject->m_Equips[i] = roleData.equips(i);
	}

	for(int i = 0; i < roleData.propertys_size(); i++)
	{
		pObject->m_Propertys[i] = roleData.propertys(i);
	}
	pObject->m_Propertys[HP] = pObject->m_Propertys[HP_MAX];
	pObject->m_Propertys[MP] = pObject->m_Propertys[MP_MAX];

	m_pSceneLogic->OnObjectCreate(pObject);

	BroadNewObject(pObject);

	return pObject;
}

CSceneObject* CScene::CreatePet(const TransPetData& petData, UINT64 uHostID, UINT32 dwCamp )
{
	StActorInfo* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(petData.actorid());
	ERROR_RETURN_NULL(pActorInfo != NULL);
	CSceneObject* pObject = new CSceneObject(petData.petguid(), this);
	ERROR_RETURN_NULL(pObject != NULL);

	//根据数据创建宠物，英雄
	pObject->m_dwCamp = dwCamp;
	pObject->m_uHostGuid = uHostID;
	pObject->m_dwObjType = OT_PET;
	pObject->m_dwActorID = petData.actorid();
	pObject->m_strName = pActorInfo->strName;
	pObject->m_dwLevel = petData.level();
	for (int i = 0; i < petData.propertys_size(); i++)
	{
		pObject->m_Propertys[i] = petData.propertys(i);
	}
	pObject->m_Propertys[HP] = pObject->m_Propertys[HP_MAX];
	pObject->m_Propertys[MP] = pObject->m_Propertys[MP_MAX];

	m_pSceneLogic->OnObjectCreate(pObject);
	AddMonster(pObject);
	BroadNewObject(pObject);
	return pObject;
}

CSceneObject* CScene::CreatePartner(const TransPartnerData& partnerData, UINT64 uHostID, UINT32 dwCamp  )
{
	StActorInfo* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(partnerData.actorid());
	ERROR_RETURN_NULL(pActorInfo != NULL);
	CSceneObject* pObject = new CSceneObject(partnerData.partnerguid(), this);
	ERROR_RETURN_NULL(pObject != NULL);

	//根据数据创建宠物，英雄
	pObject->m_dwCamp = dwCamp;
	pObject->m_uHostGuid = uHostID;
	pObject->m_dwObjType = OT_PARTNER;
	pObject->m_dwActorID = partnerData.actorid();
	pObject->m_strName = pActorInfo->strName;
	pObject->m_dwLevel = partnerData.level();
	for (int i = 0; i < partnerData.propertys_size(); i++)
	{
		pObject->m_Propertys[i] = partnerData.propertys(i);
	}
	pObject->m_Propertys[HP] = pObject->m_Propertys[HP_MAX];
	pObject->m_Propertys[MP] = pObject->m_Propertys[MP_MAX];

	m_pSceneLogic->OnObjectCreate(pObject);
	AddMonster(pObject);
	BroadNewObject(pObject);
	return pObject;
}

CSceneObject* CScene::CreateSummon(UINT32 dwActorID, UINT64 uSummonerID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft)
{
	StActorInfo* pActorInfo = CConfigData::GetInstancePtr()->GetActorInfo(dwActorID);
	ERROR_RETURN_NULL(pActorInfo != NULL);
	CSceneObject* pObject = new CSceneObject(GenNewGuid(), this);

	//根据数据创建宠物，英雄
	pObject->m_dwCamp = dwCamp;
	pObject->m_uSummonerID = uSummonerID;
	pObject->m_dwObjType = OT_SUMMON;
	pObject->m_dwActorID = dwActorID;
	pObject->m_strName = pActorInfo->strName;
	pObject->m_dwLevel = 0;// partnerData.level();
	for (int i = 0; i < PROPERTY_NUM; i++)
	{
		pObject->m_Propertys[i] = pActorInfo->Propertys[i];
	}
	pObject->m_Propertys[HP] = pObject->m_Propertys[HP_MAX];
	pObject->m_Propertys[MP] = pObject->m_Propertys[MP_MAX];
	pObject->SetPos(x, y, z, ft);

	m_pSceneLogic->OnObjectCreate(pObject);
	AddMonster(pObject);
	BroadNewObject(pObject);
	return pObject;
}

CBulletObject* CScene::CreateBullet(UINT32 dwBulletID, UINT32 dwType, FLOAT Angle, FLOAT Fix, FLOAT Muti)
{
	CBulletObject* pBullet = new CBulletObject(GenNewGuid(), dwBulletID, dwType, Angle, Fix, Muti);

	m_BulletMap.insert(std::make_pair(pBullet->m_dwID, pBullet));

	BroadNewBullet(pBullet);

	return pBullet;
}

BOOL CScene::IsCampAllDie(UINT32 dwCamp)
{
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
	{
		CSceneObject* pObj = itor->second;
		if((pObj != NULL) && (pObj->GetCamp() == dwCamp))
		{
			if(!pObj->IsDead() && pObj->m_bIsCampCheck)
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
			if(!pObj->IsDead() && pObj->m_bIsCampCheck)
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
			if(!pObj->IsDead() && pObj->m_bIsMonsCheck)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CScene::ReadSceneXml()
{
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



