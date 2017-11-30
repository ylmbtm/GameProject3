#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "CommonFunc.h"
#include "Position.h"
#include "Log.h"
#include "CommonThreadFunc.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"
CGameService::CGameService(void)
{
	m_dwLogicConnID = 0;
}

CGameService::~CGameService(void)
{
}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
}


BOOL CGameService::Init()
{
	CommonFunc::SetCurrentWorkDir("");

	if(!CLog::GetInstancePtr()->StartLog("ProxyServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogError("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("proxy_log_level"));

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	ERROR_RETURN_FALSE(m_ProxyMsgHandler.Init(0));

	ConnectToLogicSvr();

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

	return TRUE;
}

BOOL CGameService::OnNewConnect(CConnection* pConn)
{
	m_ProxyMsgHandler.OnNewConnect(pConn);

	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection* pConn)
{
	if(pConn->GetConnectionID() == m_dwLogicConnID)
	{
		m_dwLogicConnID = 0;
		ConnectToLogicSvr();
	}

	m_ProxyMsgHandler.OnCloseConnect(pConn);

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_REQ, OnMsgWatchHeartBeatReq)
		default:
		{
			m_ProxyMsgHandler.DispatchPacket(pNetPacket);
		}
		break;
	}

	return TRUE;
}

UINT32 CGameService::GetLogicConnID()
{
	return m_dwLogicConnID;
}

BOOL CGameService::ConnectToLogicSvr()
{
	UINT32 nLogicPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	std::string strLogicIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
	CConnection* pConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strLogicIp, nLogicPort);
	ERROR_RETURN_FALSE(pConn != NULL);


	m_dwLogicConnID = pConn->GetConnectionID();

	return TRUE;
}

BOOL CGameService::Uninit()
{
	ServiceBase::GetInstancePtr()->StopNetwork();
	google::protobuf::ShutdownProtobufLibrary();

	return TRUE;
}

BOOL CGameService::Run()
{
	while(TRUE)
	{
		ServiceBase::GetInstancePtr()->Update();

		CommonThreadFunc::Sleep(1);
	}

	return TRUE;
}

BOOL CGameService::OnMsgWatchHeartBeatReq(NetPacket* pNetPacket)
{
	WatchHeartBeatReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	WatchHeartBeatAck Ack;

	Ack.set_data(Req.data());
	Ack.set_retcode(MRC_SUCCESSED);
	Ack.set_processid(CommonFunc::GetCurProcessID());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_WATCH_HEART_BEAT_ACK, 0, 0, Ack);

	return TRUE;
}




