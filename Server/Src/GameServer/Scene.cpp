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
CScene::CScene()
{
	
}

CScene::~CScene()
{

}

BOOL CScene::Init(UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwLogicType)
{
	m_dwCopyID = dwCopyID;

	m_dwCopyType = dwCopyType;

	m_bOver = FALSE;

	CreateSceneLogic(dwLogicType);

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

    BroadRemoveObject(pSceneObject);

	return TRUE;
}



BOOL CScene::OnUpdate( UINT32 dwTick )
{
    ObjectUpdateNty Nty;
	for(std::map<UINT64, CSceneObject*>::iterator itor = m_PlayerMap.begin(); itor != m_PlayerMap.end(); itor++)
    {
        CSceneObject *pObj = itor->second;
        ASSERT(pObj != NULL);

        pObj->SaveUpdateObject(Nty);
    }

	//if(Nty.updatelist_size() <= 0)
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

BOOL CScene::CreateSceneLogic(UINT32 dwLogicType)
{
	switch (dwLogicType)
	{
	case SLT_NORMAL:
		m_pSceneLogic = new SceneLogic_Normal();
		break;

	default:
		{
			return false;
		}
	}

	return (m_pSceneLogic != NULL);
}




BOOL CScene::IsCopyOver()
{
	return m_bOver;
}

BOOL CScene::IsFinished()
{
	return m_bFinished;
}

BOOL CScene::OnMsgTransRoleDataReq(NetPacket *pNetPacket)
{
	TransRoleDataReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	CSceneObject *pObject = GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_FALSE(pObject == NULL);

	//根据数据创建宠物，英雄
	CSceneObject *pSceneObject = new CSceneObject;
	pSceneObject->m_dwType = OT_PLAYER;
	pSceneObject->m_dwObjType = Req.roletype();
	pSceneObject->m_strName = Req.rolename();
	AddPlayer(pSceneObject);

    //检查人齐没齐，如果齐了，就全部发准备好了的消息
    //有的副本不需要等人齐，有人就可以进

	TransRoleDataAck Ack;
	Ack.set_copyid(m_dwCopyID);
	Ack.set_copytype(m_dwCopyType);
	Ack.set_roleid(pHeader->u64TargetID);
	Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
	Ack.set_retcode(MRC_SUCCESSED);
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

    m_PlayerMap.insert(std::make_pair(pSceneObject->GetObjectID(), pSceneObject));

    return TRUE;
}





CSceneObject* CScene::GetSceneObject(UINT64 uID)
{

	return NULL;
}
