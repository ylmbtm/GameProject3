#include "stdafx.h"
#include "CommandDef.h"
#include "AccountMsgHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "Utility/CommonSocket.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"
#include "../Message/Msg_RetCode.pb.h"




CAccountMsgHandler::CAccountMsgHandler()
{

}

CAccountMsgHandler::~CAccountMsgHandler()
{

}

BOOL CAccountMsgHandler::Init(UINT32 dwReserved)
{
	m_DBManager.Init();

    m_AccountManager.InitManager();

	return TRUE;
}

BOOL CAccountMsgHandler::Uninit()
{
	m_DBManager.Uninit();

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

	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);

	AccountRegAck Ack;

	CAccountObject *pAccount = m_AccountManager.GetAccountObjectByName(Req.accountname());
	if(pAccount != NULL)
	{
		Ack.set_retcode(MRC_FAILED);
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_ACCOUNT_REG_ACK, 0, pHeader->dwUserData, Ack);
		return TRUE;
	}

	UINT64 u64ID = m_DBManager.GetAccountID(Req.accountname().c_str());
	if (u64ID != 0)
	{
		Ack.set_retcode(MRC_FAILED);
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_ACCOUNT_REG_ACK, 0, pHeader->dwUserData, Ack);
		return TRUE;
	}
	
	pAccount = m_AccountManager.CreateAccountObject(Req.accountname().c_str(), Req.password().c_str(), Req.channel());
	if(pAccount == NULL)
	{
		Ack.set_retcode(MRC_FAILED);
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_ACCOUNT_REG_ACK, 0, pHeader->dwUserData, Ack);
		return FALSE;
	}

	if(m_DBManager.CreateAccount(pAccount->m_ID, Req.accountname().c_str(), Req.password().c_str(), pAccount->m_dwChannel, pAccount->m_dwCreateTime))
	{ 
		Ack.set_retcode(MRC_SUCCESSED);
	}
	else
	{
		Ack.set_retcode(MRC_FAILED);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_ACCOUNT_REG_ACK, 0, pHeader->dwUserData, Ack);
	
	return TRUE;
}

BOOL CAccountMsgHandler::OnMsgAccontLoginReq(NetPacket *pPacket)
{	
	AccountLoginReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	PacketHeader *pHeader = (PacketHeader *) pPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);


	AccountLoginAck Ack;
	CAccountObject *pAccObj = m_AccountManager.GetAccountObjectByName(Req.accountname());
	if(pAccObj != NULL)
	{
		if(Req.password() == pAccObj->m_strPassword)
		{
			Ack.set_lastsvrid(pAccObj->m_dwLastSvrID);
			Ack.set_accountid(pAccObj->m_ID);
			Ack.set_retcode(MRC_SUCCESSED);
			ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_ACCOUNT_LOGIN_ACK, 0, pHeader->dwUserData, Ack);
			return TRUE;
		}
	}

	UINT64 u64AccountID = m_DBManager.VerifyAccount(Req.accountname().c_str(), Req.password().c_str());
	if(u64AccountID == 0)
	{
		Ack.set_retcode(MRC_FAILED);
	}
	else
	{
		m_AccountManager.AddAccountObject(u64AccountID, Req.accountname().c_str(), Req.password().c_str(), 0);
		Ack.set_retcode(MRC_SUCCESSED);
	}


	Ack.set_lastsvrid(201);
	Ack.set_accountid(u64AccountID);
	Ack.set_lastsvrname("Test_Server_1");
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_ACCOUNT_LOGIN_ACK, 0, pHeader->dwUserData, Ack);
	return TRUE;
}
