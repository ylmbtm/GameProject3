#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "TimerManager.h"
#include "Utility/CommonThreadFunc.h"
#include "../Message/Msg_ID.pb.h"
#include "DataPool.h"
#include "SimpleManager.h"
#include "../ConfigData/ConfigData.h"
#include "GlobalDataMgr.h"
#include "GroupMailMgr.h"

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
	CommonFunc::SetCurrentWorkPath("");

	if(!CLog::GetInstancePtr()->StartLog("LogicServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogError("---------服务器开始启动--------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	CreateDataPool();

	CConfigData::GetInstancePtr()->LoadConfigData("Config.db");

	CSimpleManager::GetInstancePtr()->LoadSimpleData();

	CGlobalDataManager::GetInstancePtr()->LoadGlobalData();

	CGroupMailMgr::GetInstancePtr()->LoadGroupMailData();

	ConnectToLogServer();

	ConnectToLoginSvr();

	ConnectToDBSvr();

	m_WorldMsgHandler.Init(0);

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

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

BOOL CGameService::SendCmdToDBConnection(IDataBuffer* pBuffer)
{
	return TRUE;
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


BOOL CGameService::RegisterToLoginSvr()
{
	SvrRegToSvrReq Req;
	UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("domainid");
	UINT32 dwPort  = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	Req.set_serverid(dwServerID);
	Req.set_serverport(dwPort);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwLoginConnID, MSG_LOGIC_REGTO_LOGIN_REQ, 0, 0, Req);
}

BOOL CGameService::OnNewConnect(CConnection* pConn)
{
	if(pConn->GetConnectionID() == m_dwLoginConnID)
	{
		RegisterToLoginSvr();
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
		m_dwLogConnID = NULL;
		ConnectToLogServer();
		return TRUE;
	}

	if(m_dwDBConnID == pConn->GetConnectionID())
	{
		m_dwDBConnID = NULL;
		ConnectToDBSvr();
		return TRUE;
	}

	CGameSvrMgr::GetInstancePtr()->OnCloseConnect(pConn->GetConnectionID());

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	if(CGameSvrMgr::GetInstancePtr()->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	if(m_WorldMsgHandler.DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return TRUE;
}

BOOL CGameService::OnTimer(UINT32 dwUserData)
{
	if(m_dwLoginConnID == 0)
	{
		ConnectToLoginSvr();
	}

	if(m_dwLogConnID == 0)
	{
		ConnectToLogServer();
	}

	if(m_dwDBConnID == 0)
	{
		ConnectToDBSvr();
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
	return CConfigFile::GetInstancePtr()->GetIntValue("domainid");
}
