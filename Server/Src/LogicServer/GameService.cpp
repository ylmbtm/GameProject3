#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "TimerManager.h"
#include "Utility/CommonThreadFunc.h"
#include "../Message/Msg_Login.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "DataPool.h"
#include "TypeDataParser.h"
#include "SimpleMananger.h"

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
		ASSERT_FAIELD;
		return FALSE;
	}

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");

		return FALSE;
	}

	CGloalData::GetInstancePtr()->ReadTypeData();

	CSimpleManager::GetInstancePtr()->LoadSimpleData();

	ConnectToLogServer();

	ConnectToLoginSvr();

	ConnectToDBSvr();

	CreateDataPool();

	m_WorldMsgHandler.Init(0);

	return TRUE;
}

BOOL CGameService::Uninit()
{
	ServiceBase::GetInstancePtr()->StopNetwork();
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

BOOL CGameService::SendCmdToDBConnection(IDataBuffer *pBuffer)
{
	return TRUE;
}


BOOL CGameService::ConnectToLogServer()
{
	UINT32 nStatPort = CConfigFile::GetInstancePtr()->GetIntValue("stat_svr_port");
	std::string strStatIp = CConfigFile::GetInstancePtr()->GetStringValue("stat_svr_ip");
	CConnection *pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strStatIp, nStatPort);
	if(pConnection == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_dwLogConnID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::ConnectToLoginSvr()
{
	UINT32 nLoginPort = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_port");
	std::string strLoginIp = CConfigFile::GetInstancePtr()->GetStringValue("login_svr_ip");
	CConnection *pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strLoginIp, nLoginPort);
	if(pConnection == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}
	m_dwLoginConnID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::ConnectToDBSvr()
{
	UINT32 nDBPort = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_port");
	std::string strDBIp = CConfigFile::GetInstancePtr()->GetStringValue("db_svr_ip");
	CConnection *pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strDBIp, nDBPort);
	if(pConnection == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}
	m_dwDBConnID = pConnection->GetConnectionID();
	return TRUE;
}


BOOL CGameService::RegisterToLoginSvr()
{
	SvrRegToSvrReq Req;
	UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("domainid");
	Req.set_serverid(dwServerID);

	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwLoginConnID, MSG_LOGIC_REGTO_LOGIN_REQ, 0, 0, Req);
}

BOOL CGameService::OnNewConnect(CConnection *pConn)
{
	//CLog::GetInstancePtr()->AddLog("新连接来到!");

	if(pConn->GetConnectionID() == m_dwLoginConnID)
	{
		RegisterToLoginSvr();
	}

	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("断开连接!");

	if(m_dwLoginConnID == pConn->GetConnectionID())
	{
		m_dwLoginConnID = 0;
		ConnectToLoginSvr();
	}

	if(m_dwLogConnID == pConn->GetConnectionID())
	{
		m_dwLogConnID = NULL;
		ConnectToLogServer();
	}

	if(m_dwDBConnID == pConn->GetConnectionID())
	{
		m_dwDBConnID = NULL;
		ConnectToDBSvr();
	}


	CGameSvrMgr::GetInstancePtr()->OnCloseConnect(pConn->GetConnectionID());
	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket *pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
	default:
		{
			m_WorldMsgHandler.DispatchPacket(pNetPacket);
		}
		break;
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
