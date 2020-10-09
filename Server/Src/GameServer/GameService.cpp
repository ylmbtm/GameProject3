#include "stdafx.h"
#include "GameService.h"
#include "DataBuffer.h"
#include "../Message/Msg_ID.pb.h"
#include "StaticData.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "HttpParameter.h"
#include "CommonConvert.h"
#include "WebActionDef.h"
#include "WatcherClient.h"

CGameService::CGameService(void)
{
	m_dwLogicConnID		= 0;
	m_dwProxyConnID		= 0;
	m_dwServerID		= 0;
}

CGameService::~CGameService(void)
{
}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
}

BOOL CGameService::Init(UINT32 dwServerID, UINT32 dwPort)
{
	CommonFunc::SetCurrentWorkDir("");

	if(!CLog::GetInstancePtr()->Start("GameServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogInfo("---------服务器开始启动-ServerID:%d--Port:%d--------", dwServerID, dwPort);

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	CHAR szSignName[128] = { 0 };
	snprintf(szSignName, 128, "GameServer%d_%d", dwServerID, dwPort);
	if (CommonFunc::IsAlreadyRun(szSignName))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("game_log_level"));

	m_dwServerID = dwServerID;

	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("game_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(dwPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	CStaticData::GetInstancePtr()->LoadConfigData("Config.db");

	if(!m_SceneManager.Init(TRUE))
	{
		CLog::GetInstancePtr()->LogError("启动场景管理器失败!");
		return FALSE;
	}

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");
	return TRUE;
}

BOOL CGameService::OnNewConnect(UINT32 nConnID)
{
	if(nConnID == m_dwLogicConnID)
	{
		RegisterToLogicSvr();
		m_SceneManager.SendCityReport();
		return TRUE;
	}

	if(nConnID == m_dwProxyConnID)
	{
		RegisterToProxySvr();
		return TRUE;
	}

	CWatcherClient::GetInstancePtr()->OnNewConnect(nConnID);

	return TRUE;
}

BOOL CGameService::OnCloseConnect(UINT32 nConnID)
{
	if(m_dwLogicConnID == nConnID)
	{
		m_dwLogicConnID = 0;
	}

	if(m_dwProxyConnID == nConnID)
	{
		m_dwProxyConnID = 0;
	}

	CWatcherClient::GetInstancePtr()->OnCloseConnect(nConnID);

	return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
	ConnectToLogicSvr();

	ConnectToProxySvr();

	CWatcherClient::GetInstancePtr()->OnSecondTimer();

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_GASVR_REGTO_PROXY_ACK, OnMsgRegToProxyAck)
			PROCESS_MESSAGE_ITEM(MSG_PHP_GM_COMMAND_REQ,    OnMsgWebCommandReq)
	}

	if (CWatcherClient::GetInstancePtr()->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	if (m_SceneManager.DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CGameService::Uninit()
{
	ServiceBase::GetInstancePtr()->StopNetwork();

	m_SceneManager.Uninit();

	google::protobuf::ShutdownProtobufLibrary();

	return TRUE;
}

BOOL CGameService::Run()
{
	while(CWatcherClient::GetInstancePtr()->IsRun())
	{
		ServiceBase::GetInstancePtr()->Update();

		m_SceneManager.OnUpdate(CommonFunc::GetTickCount());

		CommonFunc::Sleep(1);
	}

	return TRUE;
}

BOOL CGameService::SetLogicConnID( UINT32 dwConnID )
{
	m_dwLogicConnID = dwConnID;

	return TRUE;
}

UINT32 CGameService::GetLogicConnID()
{
	return m_dwLogicConnID;
}

UINT32 CGameService::GetProxyConnID()
{
	return m_dwProxyConnID;
}

BOOL CGameService::ConnectToLogicSvr()
{
	if (m_dwLogicConnID != 0)
	{
		return TRUE;
	}
	UINT32 nLogicPort = CConfigFile::GetInstancePtr()->GetRealNetPort("logic_svr_port");
	ERROR_RETURN_FALSE(nLogicPort > 0);
	std::string strLogicIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
	CConnection* pConn = ServiceBase::GetInstancePtr()->ConnectTo(strLogicIp, nLogicPort);
	ERROR_RETURN_FALSE(pConn != NULL);
	m_dwLogicConnID = pConn->GetConnectionID();

	return TRUE;
}

BOOL CGameService::ConnectToProxySvr()
{
	if (m_dwProxyConnID != 0)
	{
		return TRUE;
	}
	UINT32 nProxyPort = CConfigFile::GetInstancePtr()->GetRealNetPort("proxy_svr_port");
	ERROR_RETURN_FALSE(nProxyPort > 0);
	std::string strProxyIp = CConfigFile::GetInstancePtr()->GetStringValue("proxy_svr_ip");
	CConnection* pConn = ServiceBase::GetInstancePtr()->ConnectTo(strProxyIp, nProxyPort);
	ERROR_RETURN_FALSE(pConn != NULL);
	m_dwProxyConnID = pConn->GetConnectionID();
	return TRUE;
}

BOOL CGameService::RegisterToLogicSvr()
{
	SvrRegToSvrReq Req;
	Req.set_serverid(m_dwServerID);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwLogicConnID, MSG_GAME_REGTO_LOGIC_REQ, 0, 0, Req);
}

BOOL CGameService::RegisterToProxySvr()
{
	SvrRegToSvrReq Req;
	Req.set_serverid(m_dwServerID);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwProxyConnID, MSG_GASVR_REGTO_PROXY_REQ, 0, 0, Req);
}

UINT32 CGameService::GetServerID()
{
	return m_dwServerID;
}

BOOL CGameService::OnMsgDefautReq(NetPacket* pNetPacket)
{
	return TRUE;
}

BOOL CGameService::OnMsgRegToProxyAck(NetPacket* pNetPacket)
{
	SvrRegToSvrAck Ack;

	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	return TRUE;
}

BOOL CGameService::OnMsgWebCommandReq(NetPacket* pNetPacket)
{
	CHAR szMsgBuf[1024] = { 0 };
	strncpy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	HttpParameter Params;
	Params.ParseStringToMap(szMsgBuf);
	std::string strAction = Params.GetStrValue("Action");
	CLog::GetInstancePtr()->LogInfo("Web Action :%s", strAction.c_str());

	EWebAction eWebAction = (EWebAction)CommonConvert::StringToInt(strAction.c_str());
	switch (eWebAction)
	{
		case EWA_RELOAD_TABLE:
		{
			std::string strName = Params.GetStrValue("TableName");
			CStaticData::GetInstancePtr()->ReloadConfigData(strName);
		}
		break;
	}

	return TRUE;
}
