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

	if(!CLog::GetInstancePtr()->StartLog("WorldServer", "log"))
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

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("world_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("world_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");

		return FALSE;
	}

	ConnectToStatServer();

	ConnectToLoginSvr();

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


BOOL CGameService::ConnectToStatServer()
{
	UINT32 nStatPort = CConfigFile::GetInstancePtr()->GetIntValue("stat_svr_port");
	std::string strStatIp = CConfigFile::GetInstancePtr()->GetStringValue("stat_svr_ip");
	m_pStatSvrConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strStatIp, nStatPort);
	if(m_pStatSvrConn == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}
	return TRUE;
}

BOOL CGameService::ConnectToLoginSvr()
{
	UINT32 nLoginPort = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_port");
	std::string strLoginIp = CConfigFile::GetInstancePtr()->GetStringValue("login_svr_ip");
	m_pLoginSvrConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strLoginIp, nLoginPort);
	if(m_pLoginSvrConn == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

BOOL CGameService::ConnectToDBSvr()
{
	UINT32 nDBPort = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_port");
	std::string strDBIp = CConfigFile::GetInstancePtr()->GetStringValue("db_svr_ip");
	m_pDBServerConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strDBIp, nDBPort);
	if(m_pDBServerConn == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

BOOL CGameService::ConnectToProxySvr()
{
	UINT32 nProxyPort = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	std::string strProxyIp = CConfigFile::GetInstancePtr()->GetStringValue("proxy_svr_ip");
	m_pProxySvrConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strProxyIp, nProxyPort);
	if(m_pProxySvrConn == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

BOOL CGameService::RegisterToLoginSvr()
{
	RegToLoginSvrReq Req;

	UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("domainid");
	Req.set_serverid(dwServerID);

	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_pLoginSvrConn->GetConnectionID(), MSG_LOGIC_REGTO_LOGIN_REQ, 0, 0, Req);
}

BOOL CGameService::OnNewConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("新连接来到!");

	if(pConn == m_pLoginSvrConn)
	{
		RegisterToLoginSvr();
	}

	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("断开连接!");

	if(m_pLoginSvrConn == pConn)
	{
		m_pLoginSvrConn = NULL;
		ConnectToLoginSvr();
	}

	if(m_pStatSvrConn == pConn)
	{
		m_pStatSvrConn = NULL;
		ConnectToStatServer();
	}

	if(m_pDBServerConn == pConn)
	{
		m_pDBServerConn = NULL;
		ConnectToDBSvr();
	}

	if(m_pProxySvrConn == pConn)
	{
		m_pProxySvrConn = NULL;
		ConnectToProxySvr();
	}
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
	if(m_pLoginSvrConn == NULL)
	{
		ConnectToLoginSvr();
	}

	if(m_pStatSvrConn == NULL)
	{
		ConnectToStatServer();
	}

	if(m_pDBServerConn == NULL)
	{
		ConnectToDBSvr();
	}

	if(m_pProxySvrConn == NULL)
	{
		ConnectToProxySvr();
	}
	

	return TRUE;
}