#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "Error.h"
#include "AccountMsgHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "Utility/CommonSocket.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"




CAccountMsgHandler::CAccountMsgHandler()
{

}

CAccountMsgHandler::~CAccountMsgHandler()
{

}

BOOL CAccountMsgHandler::Init(UINT32 dwReserved)
{
	m_DBProcManager.Init();

	return TRUE;
}

BOOL CAccountMsgHandler::Uninit()
{
	return TRUE;
}


BOOL CAccountMsgHandler::DispatchPacket(NetPacket *pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
	default:
		{
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_REG_REQ,	    OnMsgAccountRegReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGIN_REQ,		OnMsgAccontLoginReq);
		}
		break;
	}


	return TRUE;
}

BOOL CAccountMsgHandler::OnUpdate(UINT32 dwTick)
{
	return TRUE;
}

BOOL CAccountMsgHandler::OnMsgAccountRegReq(NetPacket *pPacket)
{
	AccountRegReq Req;

	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	PacketHeader *pHeader = (PacketHeader *) pPacket->m_pDataBuffer->GetBuffer();

	ASSERT(pHeader->u64TargetID != 0);

	AccountRegAck Ack;

	if(m_DBProcManager.CreateAccount(Req.accountname().c_str(), Req.password().c_str()))
	{ 
		Ack.set_retcode(E_SUCCESSED);
	}
	else
	{
		Ack.set_retcode(E_FAILED);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_pConnect->GetConnectionID(), MSG_ACCOUNT_REG_ACK, pHeader->u64TargetID, 0, Ack);
	return TRUE;
}

BOOL CAccountMsgHandler::OnMsgAccontLoginReq(NetPacket *pPacket)
{	
	AccountLoginReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	PacketHeader *pHeader = (PacketHeader *) pPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);


	AccountLoginAck Ack;
	
	UINT32 dwAccountID = m_DBProcManager.VerifyAccount(Req.accountname().c_str(), Req.password().c_str());
	if(dwAccountID == 0)
	{
		Ack.set_retcode(E_FAILED);
	}
	else
	{
		Ack.set_retcode(E_SUCCESSED);
	}


	Ack.set_lastsvrid(1);
	Ack.set_accountid(dwAccountID);

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_pConnect->GetConnectionID(), MSG_ACCOUNT_LOGIN_ACK, pHeader->u64TargetID, 0, Ack);

	return TRUE;
}
