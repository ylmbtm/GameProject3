#include "stdafx.h"
#include "CommandDef.h"
#include "DBMsgHandler.h"
#include "Log.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "CommonSocket.h"
#include "DataBuffer.h"
#include "../Message/Msg_ID.pb.h"


CDBMsgHandler::CDBMsgHandler()
{

}

CDBMsgHandler::~CDBMsgHandler()
{

}

BOOL CDBMsgHandler::Init(UINT32 dwReserved)
{
	ERROR_RETURN_FALSE(m_DBManager.Init());

	return TRUE;
}

BOOL CDBMsgHandler::Uninit()
{
	m_DBManager.Uninit();

	return TRUE;
}


BOOL CDBMsgHandler::DispatchPacket(NetPacket* pNetPacket)
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

BOOL CDBMsgHandler::OnMsgRoleListReq(NetPacket* pPacket)
{
	RoleListReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	RoleListAck Ack;
	m_DBManager.GetRoleList(Req.accountid(), Ack);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID,  MSG_ROLE_LIST_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
}

BOOL CDBMsgHandler::OnMsgRoleLoginReq(NetPacket* pPacket)
{
	RoleLoginReq  Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	DBRoleLoginAck Ack;
	m_DBManager.GetRoleData(Req.roleid(), Ack);
	m_DBManager.GetBagData(Req.roleid(), Ack);
	m_DBManager.GetCopyData(Req.roleid(), Ack);
	m_DBManager.GetEquipData(Req.roleid(), Ack);
	m_DBManager.GetPetData(Req.roleid(), Ack);
	m_DBManager.GetPartnerData(Req.roleid(), Ack);
	m_DBManager.GetTaskData(Req.roleid(), Ack);
	m_DBManager.GetMountData(Req.roleid(), Ack);
	m_DBManager.GetActivtyData(Req.roleid(), Ack);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID,  MSG_ROLE_LOGIN_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
}



