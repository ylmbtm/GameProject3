#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "Error.h"
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
		PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_REQ,	OnMsgSelectServerReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_REQ,		OnMsgRoleListReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_ACK,		OnMsgRoleListAck);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_CREATE_REQ,	OnMsgRoleCreateReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_DELETE_REQ,	OnMsgRoleDeleteReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_REQ,	OnMsgRoleLoginReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_ACK,	OnMsgRoleLoginAck);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGOUT_REQ,	OnMsgRoleLogoutReq);


	default:
		{

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

	ASSERT(pHeader->u64TargetID != 0);

	SelectServerAck Ack;
	Ack.set_retcode(E_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_pConnect->GetConnectionID(), MSG_SELECT_SERVER_ACK, pHeader->u64TargetID, 0, Ack);
}

BOOL CWorldMsgHandler::OnMsgRoleListReq(NetPacket *pNetPacket)
{
	RoleListReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->m_pDBServerConn->GetConnectionID(),  MSG_ROLE_LIST_REQ, pHeader->u64TargetID, 0, Req);
}

BOOL CWorldMsgHandler::OnMsgRoleListAck(NetPacket *pNetPacket)
{
	RoleListAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->m_pProxySvrConn->GetConnectionID(),  MSG_ROLE_LIST_ACK, pHeader->u64TargetID, 0, Ack);
}



BOOL CWorldMsgHandler::OnMsgRoleCreateReq(NetPacket *pNetPacket)
{
	RoleCreateReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->CreatePlayer(Req.accountid(), Req.roletype(), Req.name());

	RoleListAck Ack;
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->m_pDBServerConn->GetConnectionID(),  MSG_ROLE_CREATE_REQ, pHeader->u64TargetID, 0, Ack);
}

BOOL CWorldMsgHandler::OnMsgRoleDeleteReq(NetPacket *pNetPacket)
{
	RoleDeleteReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);

	CPlayerManager::GetInstancePtr()->DeletePlayer(Req.roleid());

	RoleDeleteAck Ack;
	Ack.set_retcode(E_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_pConnect->GetConnectionID(), MSG_ROLE_DELETE_REQ, pHeader->u64TargetID, 0, Ack);
}

BOOL CWorldMsgHandler::OnMsgRoleLoginReq(NetPacket *pNetPacket)
{
	RoleLoginReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	if(pPlayer == NULL)
	{
		return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->m_pDBServerConn->GetConnectionID(), MSG_ROLE_LOGIN_REQ, pHeader->u64TargetID, 0, Req);
	}

	RoleLoginAck Ack;
	Ack.set_retcode(E_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_pConnect->GetConnectionID(), MSG_ROLE_LOGIN_ACK, pHeader->u64TargetID, 0, Ack);

	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleLoginAck(NetPacket *pNetPacket)
{
	RoleLoginAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);


	TransRoleDataReq Req;
	Req.set_roleid(pHeader->u64TargetID);

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_pConnect->GetConnectionID(), MSG_TRANS_ROLE_DATA_REQ, pHeader->u64TargetID, 0, Req);

	Ack.set_retcode(E_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_pConnect->GetConnectionID(), MSG_ROLE_LOGIN_ACK, pHeader->u64TargetID, 0, Ack);

}

BOOL CWorldMsgHandler::OnMsgRoleLogoutReq(NetPacket *pNetPacket)
{
	RoleLogoutReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);

	CPlayerManager::GetInstancePtr()->DeletePlayer(Req.roleid());

	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	if(pPlayer == NULL)
	{

	}

	RoleLogoutAck Ack;
	Ack.set_retcode(E_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_pConnect->GetConnectionID(), MSG_ROLE_DELETE_REQ, pHeader->u64TargetID, 0, Ack);
	return TRUE;
}
