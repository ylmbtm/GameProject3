#include "stdafx.h"
#include "LoginMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "HttpParameter.h"
#include "LoginClientMgr.h"
#include "WebCommandMgr.h"
#include "../Message/Msg_Account.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_ID.pb.h"

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
			PROCESS_MESSAGE_ITEM(MSG_CHECK_VERSION_REQ,     OnMsgCheckVersionReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_REG_REQ,       OnMsgAccountRegReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGIN_REQ,     OnMsgAccountLoginReq);
			PROCESS_MESSAGE_ITEM(MSG_SERVER_LIST_REQ,       OnMsgServerListReq);
			PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_REQ,     OnMsgSelectServerReq);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_REG_ACK,       OnMsgAccountRegAck);
			PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGIN_ACK,     OnMsgAccountLoginAck);
			PROCESS_MESSAGE_ITEM(MSG_LOGIC_REGTO_LOGIN_REQ, OnMsgLogicSvrRegReq);
			PROCESS_MESSAGE_ITEMEX(MSG_LOGIC_UPDATE_REQ,    OnMsgLogicUpdateReq);
			PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_ACK,     OnMsgSelectServerAck);
			PROCESS_MESSAGE_ITEM(MSG_SEAL_ACCOUNT_ACK,      OnMsgSealAccountAck);

	}

	return FALSE;
}

BOOL CLoginMsgHandler::OnCloseConnect(UINT32 dwConnID)
{
	m_LogicSvrMgr.OnCloseConnect(dwConnID);

	return TRUE;
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
		Ack.set_retcode(MRC_ACCOUNT_WRONG_VERSION);
	}
	Ack.set_clientverion("1.0.1");
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

	CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(nConnID);
	ERROR_RETURN_TRUE(pConnection != 0);

	Req.mutable_reglog()->set_ipaddr(pConnection->GetIpAddr());

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

	CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(nConnID);
	ERROR_RETURN_TRUE(pConnection != 0);

	Req.mutable_loginlog()->set_ipaddr(pConnection->GetIpAddr());

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
	UINT32 dwIpAddr = pConn->GetIpAddr();

	ClientServerListAck Ack;

	//是否是评审服包
	BOOL bReviewClient = Req.review();

	for(auto itor = m_LogicSvrMgr.begin(); itor != m_LogicSvrMgr.end(); itor++)
	{
		LogicServerNode* pTempNode = itor->second;
		//如果是评审包
		if(bReviewClient)
		{
			if(pTempNode->m_ServerFlag != ESF_REVIEW)
			{
				continue;
			}
		}
		else
		{
			if (pTempNode->m_ServerFlag == ESF_REVIEW || pTempNode->m_ServerFlag == ESF_SHUTDOWN)
			{
				continue;
			}

			if (!pTempNode->CheckIP(dwIpAddr))
			{
				//需要检测IP
				continue;
			}

			if (!pTempNode->CheckChannel(Req.channel()))
			{
				//需要检测渠道
				continue;
			}

			if (!pTempNode->CheckVersion(Req.version()))
			{
				//需要检测版本
				continue;
			}
		}

		ClientServerNode* pClientNode =  Ack.add_svrnode();
		pClientNode->set_svrid(pTempNode->m_dwServerID);
		pClientNode->set_svrname(pTempNode->m_strSvrName);
		pClientNode->set_svrflag(pTempNode->m_ServerFlag);
		pClientNode->set_cornermark(pTempNode->m_CornerMark);
		pClientNode->set_svropentime(pTempNode->m_uSvrOpenTime);
		pClientNode->set_svrstatus(pTempNode->m_ServerStatus);

		if (pTempNode->m_ServerStatus != ESS_SVR_ONLINE)
		{
			if (pTempNode->m_ServerFlag >= ESF_GOOD && pTempNode->m_ServerFlag <= ESF_FULL)
			{
				pClientNode->set_svrflag(ESF_MAINTAIN);
			}
		}
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
	ERROR_RETURN_TRUE(Req.serverid() != 0);

	if (!CLoginClientMgr::GetInstancePtr()->CheckClientMessage(nConnID, pPacket->m_dwMsgID))
	{
		CLog::GetInstancePtr()->LogError("非法的消息请求SelectServer!!!");
		return TRUE;
	}

	CLoginClientMgr::GetInstancePtr()->RemoveByConnID(nConnID);

	LogicServerNode* pServerNode = m_LogicSvrMgr.GetLogicServerInfo(Req.serverid());
	if (pServerNode == NULL || pServerNode->m_ServerStatus != ESS_SVR_ONLINE)
	{
		CLog::GetInstancePtr()->LogError("选择服务器错误 服务器:%d, 不可用", Req.serverid());
		SelectServerAck Ack;
		Ack.set_serveraddr("0.0.0.0");
		Ack.set_serverport(0);
		Ack.set_retcode(MRC_INVALID_SERVER_ID);
		ERROR_RETURN_TRUE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_SELECT_SERVER_ACK, 0, 0, Ack));
		return TRUE;
	}

	if (pServerNode->m_ServerFlag == ESF_MAINTAIN)
	{
		CLog::GetInstancePtr()->LogError("服务器:%d 维护中", Req.serverid());
		SelectServerAck Ack;
		Ack.set_serveraddr("0.0.0.0");
		Ack.set_serverport(0);
		Ack.set_retcode(MRC_SERVER_MAINTAIN);
		ERROR_RETURN_TRUE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_SELECT_SERVER_ACK, 0, 0, Ack));
		return TRUE;
	}

	if (pServerNode->m_ServerStatus != ESS_SVR_ONLINE)
	{
		CLog::GetInstancePtr()->LogError("选择服务器错误 服务器:%d 不在线", Req.serverid());
		SelectServerAck Ack;
		Ack.set_serveraddr("0.0.0.0");
		Ack.set_serverport(0);
		Ack.set_retcode(MRC_SERVER_NOT_AVAILABLE);
		ERROR_RETURN_TRUE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_SELECT_SERVER_ACK, 0, 0, Ack));
		return TRUE;
	}

	if (pServerNode->m_uSvrOpenTime > CommonFunc::GetCurrTime())
	{
		SelectServerAck Ack;
		Ack.set_serveraddr("0.0.0.0");
		Ack.set_serverport(0);
		Ack.set_retcode(MRC_SERVER_NOT_OPENTIME);
		ERROR_RETURN_TRUE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_SELECT_SERVER_ACK, 0, 0, Ack));
		return TRUE;
	}

	Req.set_checkrole(pServerNode->m_ServerFlag == ESF_BUSY);
	ERROR_RETURN_TRUE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pServerNode->m_dwConnID, MSG_SELECT_SERVER_REQ, 0, nConnID, Req));

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
		pNode = m_LogicSvrMgr.GetSuggestServer(Ack.review(), Ack.channel(), 0);
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
	m_LogicSvrMgr.RegisterLogicServer(pPacket->m_dwConnID, Req.serverid(), Req.serverport(), Req.httpport(), Req.watchport(), Req.servername(), Req.svrinnerip());
	LogicRegToLoginAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);

	LogicServerNode* pServerNode = m_LogicSvrMgr.GetLogicServerInfo(Req.serverid());
	if (pServerNode != NULL)
	{
		Ack.set_svropentime(pServerNode->m_uSvrOpenTime);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_LOGIC_REGTO_LOGIN_ACK, 0, 0, Ack);

	CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacket->m_dwConnID);
	ERROR_RETURN_TRUE(pConnection != NULL);
	pConnection->SetConnectionData(1);

	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgLogicUpdateReq(NetPacket* pPacket)
{
	LogicUpdateInfoReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	ERROR_RETURN_TRUE(Req.serverid() > 0);
	m_LogicSvrMgr.UpdateLogicServerInfo(Req.serverid(), Req.maxonline(), Req.curonline(), Req.totalnum(), Req.cachenum(), Req.status(), Req.dberrcnt(), Req.servername());

	LogicServerNode* pServerNode = m_LogicSvrMgr.GetLogicServerInfo(Req.serverid());
	ERROR_RETURN_TRUE(pServerNode != NULL);

	LogicUpdateInfoAck Ack;
	Ack.set_svropentime(pServerNode->m_uSvrOpenTime);
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_LOGIC_UPDATE_ACK, 0, 0, Ack);
	return TRUE;
}

BOOL CLoginMsgHandler::OnMsgSelectServerAck(NetPacket* pPacket)
{
	SelectServerAck Ack;
	Ack.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	if (Ack.retcode() != MRC_SUCCESSED)
	{
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pHeader->dwUserData, MSG_SELECT_SERVER_ACK, pHeader->u64TargetID, 0, Ack);
		return TRUE;
	}

	LogicServerNode* pNode = m_LogicSvrMgr.GetLogicServerInfo(Ack.serverid());
	ERROR_RETURN_TRUE(pNode != NULL);
	Ack.set_serveraddr(pNode->m_strOuterAddr);
	Ack.set_serverport(pNode->m_dwPort);
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pHeader->dwUserData, MSG_SELECT_SERVER_ACK, pHeader->u64TargetID, 0, Ack);

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

	std::string strResult = CommonConvert::IntToString((INT64)EWR_SUCCESSED);

	ServiceBase::GetInstancePtr()->SendMsgRawData(nConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, strResult.c_str(), (UINT32)strResult.size());


	if (Ack.retcode() != MRC_SUCCESSED)
	{
		return TRUE;
	}

	if (Ack.accountid() > 0)
	{
		SealAccountNtf Ntf;
		Ntf.set_accountid(Ack.accountid());
		UINT32 dwLogicConnID = m_LogicSvrMgr.GetLogicConnID(Ack.serverid());
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwLogicConnID, MSG_SEAL_ACCOUNT_NTY, 0, 0, Ntf);
	}

	return TRUE;
}