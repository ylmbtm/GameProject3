#include "stdafx.h"
#include "CommandDef.h"
#include "DBMsgHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "Utility/CommonSocket.h"
#include "DataBuffer.h"
#include "DBStoredProcMgr.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"


CDBMsgHandler::CDBMsgHandler()
{

}

CDBMsgHandler::~CDBMsgHandler()
{

}

BOOL CDBMsgHandler::Init(UINT32 dwReserved)
{
	if(!m_DBManager.Init())
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

BOOL CDBMsgHandler::Uninit()
{
	m_DBManager.Uninit();

	return TRUE;
}


BOOL CDBMsgHandler::DispatchPacket(NetPacket *pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_REQ,			OnMsgRoleListReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_REQ,		OnMsgRoleLoginReq);
	default:
		{

		}
		break;
	}


	return TRUE;
}

BOOL CDBMsgHandler::OnUpdate(UINT32 dwTick)
{
	return TRUE;
}

BOOL CDBMsgHandler::OnMsgRoleListReq(NetPacket *pPacket)
{
	RoleListReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	RoleListAck Ack;
	m_DBManager.GetRoleList(Req.accountid(), Ack);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID,  MSG_ROLE_LIST_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
}

BOOL CDBMsgHandler::OnMsgRoleLoginReq(NetPacket *pPacket)
{
	RoleLoginReq  Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	DBRoleLoginAck Ack;
	m_DBManager.GetRoleData(Req.roleid(), Ack);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID,  MSG_ROLE_LOGIN_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
}



