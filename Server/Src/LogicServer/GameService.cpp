#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Log.h"
#include "CommonFunc.h"
#include "TimerManager.h"
#include "CommonThreadFunc.h"
#include "../Message/Msg_ID.pb.h"
#include "DataPool.h"
#include "SimpleManager.h"
#include "../ConfigData/ConfigData.h"
#include "GlobalDataMgr.h"
#include "GroupMailMgr.h"
#include "PayManager.h"
#include "GuildManager.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"
CGameService::CGameService(void)
{

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

	if(!CLog::GetInstancePtr()->StartLog("LogicServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogError("---------服务器开始启动--------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("加载 servercfg.ini文件失败!");
		return FALSE;
	}

	CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("logic_log_level"));

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	if (!CConfigData::GetInstancePtr()->LoadConfigData("Config.db"))
	{
		CLog::GetInstancePtr()->LogError("加载静态配制数据失败!");
		return FALSE;
	}

	if (!CreateDataPool())
	{
		CLog::GetInstancePtr()->LogError("初始化共享内存池失败!");
		return FALSE;
	}

	///////////////////////////////////
	//服务器启动之前需要加载的数据
	std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_ip");
	nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
	std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_user");
	std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_pwd");
	std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_db_name");

	CppMySQL3DB tDBConnection;
	if(!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
	{
		CLog::GetInstancePtr()->LogError("连接数据库服务器GameSvrDB失败!");
		return FALSE;
	}

	CGlobalDataManager::GetInstancePtr()->LoadGlobalData(tDBConnection);

	CSimpleManager::GetInstancePtr()->LoadSimpleData(tDBConnection);

	CGroupMailMgr::GetInstancePtr()->LoadGroupMailData(tDBConnection);

	CGuildManager::GetInstancePtr()->LoadAllGuildData(tDBConnection);
	///////////////////////////////////////////////

	if (!CPayManager::GetInstancePtr()->InitPayManager())
	{
		CLog::GetInstancePtr()->LogError("初始化支付管理器失败!");
		return FALSE;
	}

	ConnectToLogServer();

	ConnectToLoginSvr();

	ConnectToDBSvr();

	ConnectToCenterSvr();

	m_LogicMsgHandler.Init(0);

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

	return TRUE;
}

BOOL CGameService::Uninit()
{
	ReleaseDataPool();
	ServiceBase::GetInstancePtr()->StopNetwork();
	google::protobuf::ShutdownProtobufLibrary();
	return TRUE;
}

BOOL CGameService::Run()
{
	UINT64 uTickCount = 0;
	while(TRUE)
	{
		uTickCount = CommonFunc::GetTickCount();

		ServiceBase::GetInstancePtr()->Update();

		m_LogicMsgHandler.OnUpdate(uTickCount);

		CommonThreadFunc::Sleep(1);
	}

	return TRUE;
}

BOOL CGameService::SendCmdToDBConnection(IDataBuffer* pBuffer)
{
	return ServiceBase::GetInstancePtr()->SendMsgBuffer(CGameService::GetInstancePtr()->GetDBConnID(), pBuffer);
}


BOOL CGameService::ConnectToLogServer()
{
	UINT32 nLogPort = CConfigFile::GetInstancePtr()->GetIntValue("log_svr_port");
	std::string strLogIp = CConfigFile::GetInstancePtr()->GetStringValue("log_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strLogIp, nLogPort);
	ERROR_RETURN_FALSE(pConnection != NULL);

	m_dwLogConnID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::ConnectToLoginSvr()
{
	UINT32 nLoginPort = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_port");
	std::string strLoginIp = CConfigFile::GetInstancePtr()->GetStringValue("login_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strLoginIp, nLoginPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwLoginConnID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::ConnectToDBSvr()
{
	UINT32 nDBPort = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_port");
	std::string strDBIp = CConfigFile::GetInstancePtr()->GetStringValue("db_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strDBIp, nDBPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwDBConnID = pConnection->GetConnectionID();
	return TRUE;
}


BOOL CGameService::ConnectToCenterSvr()
{
	UINT32 nCenterPort = CConfigFile::GetInstancePtr()->GetIntValue("center_svr_port");
	std::string strCenterIp = CConfigFile::GetInstancePtr()->GetStringValue("center_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strCenterIp, nCenterPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwCenterID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::RegisterToLoginSvr()
{
	LogicRegToLoginReq Req;
	UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
	std::string strSvrName = CConfigFile::GetInstancePtr()->GetStringValue("areaname");
	UINT32 dwPort  = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	std::string strIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
	Req.set_serverid(dwServerID);
	Req.set_serverport(dwPort);
	Req.set_serverip(strIp);
	Req.set_servername(strSvrName);
	Req.set_httpport(dwPort);
	Req.set_watchport(dwPort);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwLoginConnID, MSG_LOGIC_REGTO_LOGIN_REQ, 0, 0, Req);
}

BOOL CGameService::RegisterToCenterSvr()
{
	SvrRegToSvrReq Req;
	UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
	std::string strSvrName = CConfigFile::GetInstancePtr()->GetStringValue("areaname");
	UINT32 dwPort  = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	std::string strIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
	Req.set_serverid(dwServerID);
	Req.set_serverport(dwPort);
	Req.set_serverip(strIp);
	Req.set_servername(strSvrName);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwCenterID, MSG_LOGIC_REGTO_CENTER_REQ, 0, 0, Req);
}

BOOL CGameService::OnNewConnect(CConnection* pConn)
{
	if(pConn->GetConnectionID() == m_dwLoginConnID)
	{
		RegisterToLoginSvr();
	}

	if(pConn->GetConnectionID() == m_dwCenterID)
	{
		RegisterToCenterSvr();
	}

	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection* pConn)
{
	if(m_dwLoginConnID == pConn->GetConnectionID())
	{
		m_dwLoginConnID = 0;
		ConnectToLoginSvr();
		return TRUE;
	}

	if(m_dwLogConnID == pConn->GetConnectionID())
	{
		m_dwLogConnID = 0;
		ConnectToLogServer();
		return TRUE;
	}

	if(m_dwDBConnID == pConn->GetConnectionID())
	{
		m_dwDBConnID = 0;
		ConnectToDBSvr();
		return TRUE;
	}

	if(m_dwCenterID == pConn->GetConnectionID())
	{
		m_dwCenterID = 0;
		ConnectToCenterSvr();
		return TRUE;
	}

	CGameSvrMgr::GetInstancePtr()->OnCloseConnect(pConn->GetConnectionID());

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	BOOL bHandled = TRUE;
	switch (pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_REQ, OnMsgWatchHeartBeatReq)
		default:
		{
			bHandled = FALSE;
		}
		break;
	}

	if (bHandled)
	{
		return TRUE;
	}

	if(CGameSvrMgr::GetInstancePtr()->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	if(m_LogicMsgHandler.DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return TRUE;
}


UINT32 CGameService::GetDBConnID()
{
	return m_dwDBConnID;
}

UINT32 CGameService::GetLoginConnID()
{
	return m_dwLoginConnID;
}

UINT32 CGameService::GetServerID()
{
	return CConfigFile::GetInstancePtr()->GetIntValue("areaid");
}

UINT32 CGameService::GetCenterID()
{
	return m_dwCenterID;
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