#include "stdafx.h"
#include "../Message/Msg_ID.pb.h"
#include "LoginMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "HttpParameter.h"
#include "LoginClientMgr.h"

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
	m_LogicSvrMgr.Uninit();

	return TRUE;
}



BOOL CLoginMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			//客户端连接上的第一个消息，验证版本号
			PROCESS_MESSAGE_ITEM(MSG_CHECK_VERSION_REQ,		OnMsgCheckVersionReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_REG_REQ,		OnMsgAccountRegReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGIN_REQ,		OnMsgAccountLoginReq);
			PROCESS_MESSAGE_ITEM(MSG_SERVER_LIST_REQ,		OnMsgServerListReq);
			PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_REQ,		OnMsgSelectServerReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_REG_ACK,		OnMsgAccountRegAck);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGIN_ACK,		OnMsgAccountLoginAck);
			PROCESS_MESSAGE_ITEM(MSG_LOGIC_REGTO_LOGIN_REQ,	OnMsgLogicSvrRegReq);
			PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_ACK,		OnMsgSelectServerAck);
			PROCESS_MESSAGE_ITEM(MSG_SEAL_ACCOUNT_ACK,		OnMsgSealAccountAck);
			PROCESS_MESSAGE_ITEM(MSG_PHP_GM_COMMAND_REQ,	OnMsgWebCommandReq);
	}

	return FALSE;
}

BOOL CLoginMsgHandler::OnMsgCheckVersionReq(NetPacket* pPacket)
{
	CheckVersionReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pPacket->m_dwConnID;
	ERROR_RETURN_TRUE(nConnID != 0);

	CheckVersionAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	if (!CLoginClientMgr::GetInstancePtr()->CheckClientMessage(nConnID, pPacket->m_dwMsgID))
	{
		Ack.set_retcode(MRC_BAD_CLIENT_VER);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_CHECK_VERSION_ACK, 0, 0, Ack);
	return TRUE;
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

BOOL CLoginMsgHandler::OnMsgServerListReq(NetPacket* pPacket)
{
	ClientServerListReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	CConnection* pConn = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacket->m_dwConnID);
	ERROR_RETURN_TRUE(pConn != NULL);
	UINT32 dwIpAddr = pConn->m_dwIpAddr;

	ClientServerListAck Ack;

	for(auto itor = m_LogicSvrMgr.begin(); itor != m_LogicSvrMgr.end(); itor++)
	{
		LogicServerNode* pTempNode = itor->second;
		//如果是评审包
		if(m_LogicSvrMgr.IsReviewVersion(Req.clientversion()))
		{
			if(pTempNode->m_Statue != ESS_REVIEW)
			{
				continue;
			}
		}
		else
		{
			if (pTempNode->m_Statue == ESS_REVIEW)
			{
				continue;
			}
		}

		if(!pTempNode->CheckIP(dwIpAddr))
		{
			//需要检测IP
			continue;
		}

		if(!pTempNode->CheckChannel(Req.channel()))
		{
			//需要检测渠道
			continue;
		}

		if(!pTempNode->CheckVersion(Req.clientversion()))
		{
			//需要检测版本
			continue;
		}

		ClientServerNode* pClientNode =  Ack.add_svrnode();
		pClientNode->set_svrid(pTempNode->m_dwServerID);
		pClientNode->set_svrname(pTempNode->m_strSvrName);
		pClientNode->set_svrstate(pTempNode->m_Statue);
		pClientNode->set_svrflag(pTempNode->m_Flag);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_SERVER_LIST_ACK, 0, 0, Ack);

	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgSelectServerReq(NetPacket* pPacket)
{
	SelectServerReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pPacket->m_dwConnID;
	ERROR_RETURN_TRUE(nConnID != 0);

	if (!CLoginClientMgr::GetInstancePtr()->CheckClientMessage(nConnID, pPacket->m_dwMsgID))
	{
		CLog::GetInstancePtr()->LogError("非法的消息请求SelectServer!!!");
		return TRUE;
	}

	CLoginClientMgr::GetInstancePtr()->RemoveByConnID(nConnID);

	UINT32 SvrConnID = m_LogicSvrMgr.GetLogicConnID(Req.serverid());
	if (SvrConnID == 0)
	{
		CLog::GetInstancePtr()->LogError("选择服务器错误 服务器:%d, 不可用。", Req.serverid());
		return TRUE;
	}

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
		Ack.set_lastsvrid(pNode->m_dwServerID);
		Ack.set_lastsvrname(pNode->m_strSvrName);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_ACCOUNT_LOGIN_ACK, 0, 0, Ack);

	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgLogicSvrRegReq(NetPacket* pPacket)
{
	LogicRegToLoginReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	m_LogicSvrMgr.RegisterLogicServer(pPacket->m_dwConnID, Req.serverid(), Req.serverport(), Req.httpport(), Req.watchport(), Req.servername());
	LogicRegToLoginAck Ack;
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
	Ack.set_serveraddr(pNode->m_strIpAddr);
	Ack.set_serverport(pNode->m_dwPort);
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pHeader->dwUserData, MSG_SELECT_SERVER_ACK, 0, 0, Ack);

	SetLastServerNty Nty;
	Nty.set_accountid(Ack.accountid());
	Nty.set_serverid(Ack.serverid());
	CGameService::GetInstancePtr()->SendCmdToAccountConnection(MSG_SET_LAST_SERVER_NTY, 0, 1, Nty);

	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgSealAccountAck(NetPacket* pPacket)
{
	SealAccountAck Ack;
	Ack.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	UINT32 nConnID = pHeader->dwUserData;
	ERROR_RETURN_TRUE(nConnID != 0);

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_SEAL_ACCOUNT_ACK, 0, 0, Ack);

	return TRUE;
}
