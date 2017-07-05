#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "WorldMsgHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "Utility/CommonSocket.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "RoleModule.h"
#include "SimpleMananger.h"

CWorldMsgHandler::CWorldMsgHandler()
{

}

CWorldMsgHandler::~CWorldMsgHandler()
{

}

BOOL CWorldMsgHandler::Init(UINT32 dwReserved)
{
	

	return TRUE;
}

BOOL CWorldMsgHandler::Uninit()
{
	return TRUE;
}

BOOL CWorldMsgHandler::DispatchPacket(NetPacket *pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
		PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_REQ,		OnMsgSelectServerReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_REQ,			OnMsgRoleListReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_ACK,			OnMsgRoleListAck);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_CREATE_REQ,		OnMsgRoleCreateReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_DELETE_REQ,		OnMsgRoleDeleteReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_REQ,		OnMsgRoleLoginReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_ACK,		OnMsgRoleLoginAck);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGOUT_REQ,		OnMsgRoleLogoutReq);
		PROCESS_MESSAGE_ITEM(MSG_TRANS_ROLE_DATA_ACK,	OnMsgTransRoleDataAck);
	case MSG_GMSVR_REGTO_LOGIC_REQ:
		CGameSvrMgr::GetInstancePtr()->OnMsgGameSvrRegister(pNetPacket);
		break;
	case MSG_LOGIC_REGTO_LOGIN_ACK:
		{
			break;
		}
	default:
		{
			PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
			
			CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
			if(pPlayer == NULL)
			{
				ASSERT_FAIELD;
			}

			pPlayer->DispatchPacket(pNetPacket);
		}
		break;
	}

	return TRUE;
}



BOOL CWorldMsgHandler::OnMsgSelectServerReq(NetPacket *pNetPacket)
{
	SelectServerReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->dwUserData != 0);
	SelectServerAck Ack;
	Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
	Ack.set_logincode(12345678);
	Ack.set_retcode(MRC_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_SELECT_SERVER_ACK, 0, pHeader->dwUserData, Ack);
}

BOOL CWorldMsgHandler::OnMsgRoleListReq(NetPacket *pNetPacket)
{
	RoleListReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->dwUserData != 0);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetDBConnID(),  MSG_ROLE_LIST_REQ, pNetPacket->m_dwConnID, pHeader->dwUserData, Req);
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleListAck(NetPacket *pNetPacket)
{
	RoleListAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->dwUserData != 0);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pHeader->u64TargetID,  MSG_ROLE_LIST_ACK, 0, pHeader->dwUserData, Ack);
}



BOOL CWorldMsgHandler::OnMsgRoleCreateReq(NetPacket *pNetPacket)
{
	RoleCreateReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->dwUserData != 0);
	//检验名字是否可用

	UINT64 u64RoleID = CSimpleManager::GetInstancePtr()->MakeNewRoleID();
	CSimpleInfo *pSimpleInfo = CSimpleManager::GetInstancePtr()->CreateSimpleInfo(u64RoleID, Req.accountid(), Req.name(), Req.roletype());
	if(pSimpleInfo == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}
	CPlayerObject *pPlayer  = CPlayerManager::GetInstancePtr()->CreatePlayer(u64RoleID);
	ASSERT(pPlayer->Init());
	CRoleModule *pRoleModule = (CRoleModule *)pPlayer->GetModuleByType(MT_ROLE);
	pRoleModule->SetBaseData(u64RoleID, Req.name(), Req.roletype(), Req.accountid(), 1);
	pPlayer->OnCreate(u64RoleID);
	pPlayer->SetAllModuleOK();
	pPlayer->SetConnectID(pNetPacket->m_dwConnID, pHeader->dwUserData);
	RoleCreateAck Ack;
	Ack.set_accountid(Req.accountid());
	Ack.set_roleid(u64RoleID);
	Ack.set_roletype(Req.roletype());
	Ack.set_name(Req.name());


	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID,  MSG_ROLE_CREATE_ACK, 0, pHeader->dwUserData, Req);
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleDeleteReq(NetPacket *pNetPacket)
{
	RoleDeleteReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);

	CPlayerManager::GetInstancePtr()->DeletePlayer(Req.roleid());

	RoleDeleteAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_ROLE_DELETE_REQ, pHeader->u64TargetID, 0, Ack);
}

BOOL CWorldMsgHandler::OnMsgRoleLoginReq(NetPacket *pNetPacket)
{
	RoleLoginReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->dwUserData != 0);
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	if(pPlayer == NULL)
	{
		return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetDBConnID(), MSG_ROLE_LOGIN_REQ, 0, pHeader->dwUserData, Req);
	}

	pPlayer->SetConnectID(pNetPacket->m_dwConnID, pHeader->dwUserData);

	RoleLoginAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_ROLE_LOGIN_ACK, pNetPacket->m_dwConnID, pHeader->dwUserData, Ack);

	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleLoginAck(NetPacket *pNetPacket)
{
	RoleLoginAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->dwUserData != 0);
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->CreatePlayer(Ack.roleid());
	pPlayer->Init();
	pPlayer->SetConnectID(pHeader->u64TargetID, pHeader->dwUserData);
	CRoleModule *pRoleModule = (CRoleModule *)pPlayer->GetModuleByType(MT_ROLE);
	pRoleModule->SetBaseData(Ack.roleid(), Ack.name(), Ack.roletype(), Ack.accountid(), 1);
	pPlayer->OnLoadData(Ack.roleid());
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleLogoutReq(NetPacket *pNetPacket)
{
	RoleLogoutReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->dwUserData != 0);
	CPlayerManager::GetInstancePtr()->DeletePlayer(Req.roleid());
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	if(pPlayer == NULL)
	{
	}

	RoleLogoutAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_ROLE_DELETE_REQ, 0, pHeader->dwUserData, Ack);
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgTransRoleDataAck(NetPacket *pNetPacket)
{
	TransRoleDataAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);

	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Ack.roleid());
	if(pPlayer == NULL)
	{
		return FALSE;
	}
	
	NotifyIntoScene Nty;
	Nty.set_copytype(Ack.copytype());
	Nty.set_copyid(Ack.copyid());
	pPlayer->SendProtoBuf(MSG_NOTIFY_INTO_SCENE, Nty);

	return TRUE;
}
