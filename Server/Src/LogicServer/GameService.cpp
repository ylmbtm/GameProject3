#include "stdafx.h"
#include "GameService.h"

#include "DataPool.h"
#include "../StaticData/StaticData.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"

#include "TimerManager.h"
#include "SimpleManager.h"
#include "GlobalDataMgr.h"
#include "MailManager.h"
#include "PayManager.h"
#include "GuildManager.h"
#include "ActivityManager.h"
#include "GmCommand.h"
#include "RankMananger.h"
#include "MsgHandlerManager.h"
#include "PacketHeader.h"
#include "TeamCopyMgr.h"

//#include "LuaManager.h"
//#include "Lua_Script.h"

CGameService::CGameService(void)
{
	m_dwLogConnID	= 0;
	m_dwLoginConnID = 0;
	m_dwDBConnID	= 0;
	m_dwCenterID	= 0;   //中心服的连接ID
	m_dwWatchSvrConnID = 0;
	m_dwWatchIndex	= 0;
}

CGameService::~CGameService(void)
{

}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
}

BOOL CGameService::SetWatchIndex(UINT32 nIndex)
{
	m_dwWatchIndex = nIndex;

	return TRUE;
}

BOOL CGameService::Init()
{
	CommonFunc::SetCurrentWorkDir("");

	if(!CLog::GetInstancePtr()->Start("LogicServer", "log"))
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

	if (!CDataPool::GetInstancePtr()->InitDataPool())
	{
		CLog::GetInstancePtr()->LogError("初始化共享内存池失败!");
		return FALSE;
	}

	///////////////////////////////////
	//服务器启动之前需要加载的数据
	if (!CStaticData::GetInstancePtr()->LoadConfigData("Config.db"))
	{
		CLog::GetInstancePtr()->LogError("加载静态配制数据失败!");
		return FALSE;
	}

	//if (!CLuaManager::GetInstancePtr()->Init())
	//{
	//	CLog::GetInstancePtr()->LogError("初始化Lua环境失败!");
	//	return FALSE;
	//}

	//if (!luaopen_LuaScript(CLuaManager::GetInstancePtr()->GetLuaState()))
	//{
	//	CLog::GetInstancePtr()->LogError("导出Lua接口失败!");
	//	return FALSE;
	//}

	//if (!CLuaManager::GetInstancePtr()->LoadAllLua(".\\Lua"))
	//{
	//	CLog::GetInstancePtr()->LogError("加载lua代码失败!");
	//	return FALSE;
	//}

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
		CLog::GetInstancePtr()->LogError("Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
		return FALSE;
	}

	BOOL bRet = FALSE;

	bRet = CGlobalDataManager::GetInstancePtr()->LoadGlobalData(tDBConnection);
	ERROR_RETURN_FALSE(bRet);

	bRet = CSimpleManager::GetInstancePtr()->LoadSimpleData(tDBConnection);
	ERROR_RETURN_FALSE(bRet)

	bRet = CMailManager::GetInstancePtr()->LoadGroupMailData(tDBConnection);
	ERROR_RETURN_FALSE(bRet)

	bRet = CMailManager::GetInstancePtr()->LoadOffMailData(tDBConnection);
	ERROR_RETURN_FALSE(bRet)

	bRet = CGuildManager::GetInstancePtr()->LoadAllGuildData(tDBConnection);
	ERROR_RETURN_FALSE(bRet)

	bRet = CActivityManager::GetInstancePtr()->LoadActivityData(tDBConnection);
	ERROR_RETURN_FALSE(bRet)

	bRet = CRankManager::GetInstancePtr()->LoadRankData(tDBConnection);
	ERROR_RETURN_FALSE(bRet)

	bRet = CGameSvrMgr::GetInstancePtr()->Init();
	ERROR_RETURN_FALSE(bRet);

	bRet = CTeamCopyMgr::GetInstancePtr()->Init();
	ERROR_RETURN_FALSE(bRet);

	bRet = CPayManager::GetInstancePtr()->Init();
	ERROR_RETURN_FALSE(bRet);

	bRet = m_LogicMsgHandler.Init(0);
	ERROR_RETURN_FALSE(bRet);

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

	return TRUE;
}

BOOL CGameService::Uninit()
{
	CDataPool::GetInstancePtr()->ReleaseDataPool();
	ServiceBase::GetInstancePtr()->StopNetwork();
	google::protobuf::ShutdownProtobufLibrary();
	return TRUE;
}

BOOL CGameService::Run()
{
	while(TRUE)
	{
		ServiceBase::GetInstancePtr()->Update();

		m_LogicMsgHandler.OnUpdate(CommonFunc::GetTickCount());

		CommonFunc::Sleep(1);
	}

	return TRUE;
}

BOOL CGameService::SendCmdToDBConnection(IDataBuffer* pBuffer)
{
	return ServiceBase::GetInstancePtr()->SendMsgBuffer(CGameService::GetInstancePtr()->GetDBConnID(), pBuffer);
}


BOOL CGameService::ConnectToLogServer()
{
	if (m_dwLogConnID != 0)
	{
		return TRUE;
	}
	UINT32 nLogPort = CConfigFile::GetInstancePtr()->GetIntValue("log_svr_port");
	std::string strLogIp = CConfigFile::GetInstancePtr()->GetStringValue("log_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strLogIp, nLogPort);
	ERROR_RETURN_FALSE(pConnection != NULL);

	m_dwLogConnID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::ConnectToLoginSvr()
{
	if (m_dwLoginConnID != 0)
	{
		return TRUE;
	}
	UINT32 nLoginPort = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_port");
	std::string strLoginIp = CConfigFile::GetInstancePtr()->GetStringValue("login_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strLoginIp, nLoginPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwLoginConnID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::ConnectToDBSvr()
{
	if (m_dwDBConnID != 0)
	{
		return TRUE;
	}
	UINT32 nDBPort = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_port");
	std::string strDBIp = CConfigFile::GetInstancePtr()->GetStringValue("db_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strDBIp, nDBPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwDBConnID = pConnection->GetConnectionID();
	return TRUE;
}


BOOL CGameService::ConnectToCenterSvr()
{
	if (m_dwCenterID != 0)
	{
		return TRUE;
	}
	UINT32 nCenterPort = CConfigFile::GetInstancePtr()->GetIntValue("center_svr_port");
	std::string strCenterIp = CConfigFile::GetInstancePtr()->GetStringValue("center_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strCenterIp, nCenterPort);
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
	UINT32 dwHttpPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	UINT32 dwWatchPort = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_port");
	std::string strIp = CConfigFile::GetInstancePtr()->GetStringValue("proxy_svr_ip");
	Req.set_serverid(dwServerID);
	Req.set_serverport(dwPort);
	Req.set_serverip(strIp);
	Req.set_servername(strSvrName);
	Req.set_httpport(dwHttpPort);
	Req.set_watchport(dwWatchPort);
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
		return TRUE;
	}

	if(m_dwLogConnID == pConn->GetConnectionID())
	{
		m_dwLogConnID = 0;
		return TRUE;
	}

	if(m_dwDBConnID == pConn->GetConnectionID())
	{
		m_dwDBConnID = 0;
		return TRUE;
	}

	if(m_dwCenterID == pConn->GetConnectionID())
	{
		m_dwCenterID = 0;
		return TRUE;
	}

	if (pConn->GetConnectionID() == m_dwWatchSvrConnID)
	{
		m_dwWatchSvrConnID = 0;
	}

	CGameSvrMgr::GetInstancePtr()->OnCloseConnect(pConn->GetConnectionID());

	return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
	ConnectToLogServer();

	ConnectToLoginSvr();

	ConnectToDBSvr();

	ConnectToCenterSvr();

	ReportServerStatus();

	SendWatchHeartBeat();

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	if (CMsgHandlerManager::GetInstancePtr()->FireMessage(pNetPacket->m_dwMsgID, pNetPacket))
	{
		return TRUE;
	}

	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
	if (pPlayer == NULL)
	{
		CLog::GetInstancePtr()->LogError("CGameService::DispatchPacket Error Invalid u64TargetID:%d, MessageID:%d", pHeader->u64TargetID, pNetPacket->m_dwMsgID);
		return TRUE;
	}

	if (pPlayer->m_NetMessagePump.FireMessage(pNetPacket->m_dwMsgID, pNetPacket))
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

BOOL CGameService::ReportServerStatus()
{
	CGlobalDataManager::GetInstancePtr()->GetMaxOnline(); //最大在线人数
	CSimpleManager::GetInstancePtr()->GetTotalCount();//总人数
	CSimpleManager::GetInstancePtr()->GetOnline();
	// 	LogicRegToLoginReq Req;
	UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
	std::string strSvrName = CConfigFile::GetInstancePtr()->GetStringValue("areaname");
	// 	Req.set_serverid(dwServerID);
	// 	Req.set_serverport(dwPort);
	// 	Req.set_serverip(strIp);
	// 	Req.set_servername(strSvrName);
	// 	Req.set_httpport(dwHttpPort);
	// 	Req.set_watchport(dwWatchPort);
	// 	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwLoginConnID, MSG_LOGIC_REGTO_LOGIN_REQ, 0, 0, Req);

	return TRUE;
}

BOOL CGameService::SendWatchHeartBeat()
{
	if (m_dwWatchIndex == 0)
	{
		return TRUE;
	}

	if (m_dwWatchSvrConnID == 0)
	{
		ConnectToWatchServer();
		return TRUE;
	}

	WatchHeartBeatReq Req;
	Req.set_data(m_dwWatchIndex);
	Req.set_processid(CommonFunc::GetCurProcessID());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwWatchSvrConnID, MSG_WATCH_HEART_BEAT_REQ, 0, 0, Req);
	return TRUE;
}



BOOL CGameService::ConnectToWatchServer()
{
	if (m_dwWatchSvrConnID != 0)
	{
		return TRUE;
	}
	UINT32 nWatchPort = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_port");
	std::string strWatchIp = CConfigFile::GetInstancePtr()->GetStringValue("watch_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strWatchIp, nWatchPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwWatchSvrConnID = pConnection->GetConnectionID();
	return TRUE;
}