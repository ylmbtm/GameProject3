#include "stdafx.h"
#include "../Message/Msg_ID.pb.h"
#include "CommandDef.h"
#include "LoginMsgHandler.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "GameService.h"
#include "CommonSocket.h"
#include "PacketHeader.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "Log.h"

CLoginMsgHandler::CLoginMsgHandler()
{

}

CLoginMsgHandler::~CLoginMsgHandler()
{

}

BOOL CLoginMsgHandler::Init()
{
	m_LogicSvrMgr.Init();

	return TRUE;
}

BOOL CLoginMsgHandler::Uninit()
{


	return TRUE;
}



BOOL CLoginMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_CHECK_VERSION_REQ,	OnMsgCheckVersionReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_REG_REQ,	OnMsgAccountRegReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGIN_REQ,	OnMsgAccountLoginReq);

			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGINREG_REQ,	OnMsgAccountLoginRegReq);
			PROCESS_MESSAGE_ITEM(MSG_SERVER_LIST_REQ,	OnMsgServerListReq);
			PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_REQ,	OnMsgSelectServerReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_REG_ACK,	OnMsgAccountRegAck);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGIN_ACK,	OnMsgAccountLoginAck);
			PROCESS_MESSAGE_ITEM(MSG_LOGIC_REGTO_LOGIN_REQ,	OnMsgLogicSvrRegReq);
			PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_ACK,	OnMsgSelectServerAck);



		default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgCheckVersionReq(NetPacket* pPacket)
{
	CheckVersionReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	CheckVersionAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_CHECK_VERSION_ACK, 0, 0, Ack);
}

BOOL CLoginMsgHandler::OnMsgAccountRegReq(NetPacket* pPacket )
{
	AccountRegReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pPacket->m_dwConnID;
	ERROR_RETURN_TRUE(nConnID != 0);

	CGameService::GetInstancePtr()->SendCmdToAccountConnection(MSG_ACCOUNT_REG_REQ, 0, nConnID, Req);
	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgAccountLoginReq(NetPacket* pPacket)
{
	AccountLoginReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pPacket->m_dwConnID;
	ERROR_RETURN_TRUE(nConnID != 0);

	CGameService::GetInstancePtr()->SendCmdToAccountConnection(MSG_ACCOUNT_LOGIN_REQ, 0, nConnID, Req);

	return TRUE;
}


BOOL CLoginMsgHandler::OnMsgAccountLoginRegReq(NetPacket* pPacket)
{
	AccountLoginReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pPacket->m_dwConnID;
	ERROR_RETURN_TRUE(nConnID != 0);

	CGameService::GetInstancePtr()->SendCmdToAccountConnection(MSG_ACCOUNT_LOGINREG_REQ, nConnID, 0, Req);

	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgServerListReq(NetPacket* pPacket)
{
	ClientServerListReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	ClientServerListAck Ack;

	for(auto itor = m_LogicSvrMgr.begin(); itor != m_LogicSvrMgr.end(); itor++)
	{
		LogicServerNode& tempNode = itor->second;
		ClientServerNode* pClientNode =  Ack.add_svrnode();
		pClientNode->set_svrid(tempNode.dwServerID);
		pClientNode->set_svrname(tempNode.strServerName);
	}

	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_SERVER_LIST_ACK, 0, 0, Ack);
}

BOOL CLoginMsgHandler::OnMsgSelectServerReq(NetPacket* pPacket)
{
	SelectServerReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pPacket->m_dwConnID;
	ERROR_RETURN_TRUE(nConnID != 0);

	UINT32 SvrConnID = m_LogicSvrMgr.GetLogicConnID(Req.serverid());
	ERROR_RETURN_TRUE(SvrConnID != 0);

	ERROR_RETURN_TRUE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(SvrConnID, MSG_SELECT_SERVER_REQ, 0, nConnID, Req));

	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgAccountRegAck( NetPacket* pPacket )
{
	AccountRegAck Ack;
	Ack.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pHeader->dwUserData;
	ERROR_RETURN_TRUE(nConnID != 0);

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_ACCOUNT_REG_ACK, 0, 0, Ack);

	return TRUE;
}



BOOL CLoginMsgHandler::OnMsgAccountLoginAck( NetPacket* pPacket )
{
	AccountLoginAck Ack;
	Ack.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pHeader->dwUserData;
	ERROR_RETURN_TRUE(nConnID != 0);

	LogicServerNode* pNode = m_LogicSvrMgr.GetLogicServerInfo(Ack.lastsvrid());
	if(pNode == NULL)
	{
		pNode = m_LogicSvrMgr.GetRecommendServerInfo();
	}

	if(pNode == NULL)
	{
		Ack.set_lastsvrid(0);
		Ack.set_lastsvrname("No Server");
	}
	else
	{
		Ack.set_lastsvrid(pNode->dwServerID);
		Ack.set_lastsvrname(pNode->strServerName);
	}

	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_ACCOUNT_LOGIN_ACK, 0, 0, Ack);
}

BOOL CLoginMsgHandler::OnMsgLogicSvrRegReq(NetPacket* pPacket)
{
	SvrRegToSvrReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	m_LogicSvrMgr.RegisterLogicServer(pPacket->m_dwConnID, Req.serverid(), Req.serverport());
	SvrRegToSvrAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_LOGIC_REGTO_LOGIN_ACK, 0, 0, Ack);
	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgSelectServerAck(NetPacket* pPacket)
{
	SelectServerAck Ack;
	Ack.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pPacket->m_dwConnID;
	ERROR_RETURN_TRUE(nConnID != 0);

	LogicServerNode* pNode = m_LogicSvrMgr.GetLogicServerInfo(Ack.serverid());
	ERROR_RETURN_TRUE(pNode != NULL);
	Ack.set_serveraddr(pNode->strIpAddr);
	Ack.set_serverport(pNode->dwPort);
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pHeader->dwUserData, MSG_SELECT_SERVER_ACK, 0, 0, Ack);
	return TRUE;
}
