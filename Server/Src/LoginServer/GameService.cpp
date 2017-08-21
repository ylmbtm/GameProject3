#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "CommonThreadFunc.h"
#include "TimerManager.h"
#include "Log.h"

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

	if(!CLog::GetInstancePtr()->StartLog("LoginServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogError("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	//ConnectToLogServer();

	ConnectToAccountSvr();

	m_LoginMsgHandler.Init();

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


BOOL CGameService::SendCmdToAccountConnection(UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
{
	ERROR_RETURN_FALSE(m_dwAccountConnID != 0);
	ERROR_RETURN_FALSE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwAccountConnID, dwMsgID, u64TargetID, dwUserData, pdata));
	return TRUE;
}

BOOL CGameService::ConnectToLogServer()
{
	UINT32 nStatPort = CConfigFile::GetInstancePtr()->GetIntValue("log_svr_port");
	std::string strStatIp = CConfigFile::GetInstancePtr()->GetStringValue("log_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strStatIp, nStatPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwLogSvrConnID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::ConnectToAccountSvr()
{
	UINT32 nAccountPort = CConfigFile::GetInstancePtr()->GetIntValue("account_svr_port");
	std::string strAccountIp = CConfigFile::GetInstancePtr()->GetStringValue("account_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strAccountIp, nAccountPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwAccountConnID = pConnection->GetConnectionID();
	return TRUE;
}

BOOL CGameService::OnNewConnect(CConnection* pConn)
{
	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection* pConn)
{
	if(pConn->GetConnectionID() == m_dwAccountConnID)
	{
		m_dwAccountConnID = 0;
		ConnectToAccountSvr();
	}

	if(pConn->GetConnectionID() == m_dwLogSvrConnID)
	{
		m_dwLogSvrConnID = 0;
		//ConnectToLogServer();
	}

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
		default:
		{
			m_LoginMsgHandler.DispatchPacket(pNetPacket);
		}
		break;
	}

	return TRUE;
}