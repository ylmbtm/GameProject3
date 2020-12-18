#include "stdafx.h"
#include "GameService.h"
#include "TimerManager.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "LoginClientMgr.h"
#include "WebCommandMgr.h"
#include "WatcherClient.h"
#include "GiftCodeManager.h"
CGameService::CGameService(void)
{
	m_dwAccountConnID	= 0;
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

	if(!CLog::GetInstancePtr()->Start("LoginServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogInfo("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	if (CommonFunc::IsAlreadyRun("LoginServer"))
	{
		CLog::GetInstancePtr()->LogError("LoginServer己经在运行!");
		return FALSE;
	}

	CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("login_log_level"));

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_port");
	if (nPort <= 0)
	{
		CLog::GetInstancePtr()->LogError("配制文件login_svr_port配制错误!");
		return FALSE;
	}

	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	ERROR_RETURN_FALSE(CWebCommandMgr::GetInstancePtr()->Init());

	ERROR_RETURN_FALSE(m_LoginMsgHandler.Init());

	ERROR_RETURN_FALSE(GiftCodeManager::GetInstancePtr()->Init());

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

	return TRUE;
}


BOOL CGameService::Uninit()
{
	CLog::GetInstancePtr()->LogError("==========服务器开始关闭=======================");

	ServiceBase::GetInstancePtr()->StopNetwork();

	m_LoginMsgHandler.Uninit();

	google::protobuf::ShutdownProtobufLibrary();

	CLog::GetInstancePtr()->LogError("==========服务器关闭完成=======================");

	return TRUE;
}

BOOL CGameService::Run()
{
	while (CWatcherClient::GetInstancePtr()->IsRun())
	{
		ServiceBase::GetInstancePtr()->Update();

		GiftCodeManager::GetInstancePtr()->Update();

		CommonFunc::Sleep(1);
	}

	return TRUE;
}


BOOL CGameService::SendCmdToAccountConnection(UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
{
	ERROR_RETURN_FALSE(m_dwAccountConnID != 0);
	ERROR_RETURN_FALSE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwAccountConnID, dwMsgID, u64TargetID, dwUserData, pdata));
	return TRUE;
}

BOOL CGameService::ConnectToAccountSvr()
{
	if (m_dwAccountConnID != 0)
	{
		return TRUE;
	}
	UINT32 nAccountPort = CConfigFile::GetInstancePtr()->GetIntValue("account_svr_port");
	ERROR_RETURN_FALSE(nAccountPort > 0);
	std::string strAccountIp = CConfigFile::GetInstancePtr()->GetStringValue("account_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strAccountIp, nAccountPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwAccountConnID = pConnection->GetConnectionID();
	pConnection->SetConnectionData(1);
	return TRUE;
}

BOOL CGameService::OnNewConnect(UINT32 nConnID)
{
	CWatcherClient::GetInstancePtr()->OnNewConnect(nConnID);

	return TRUE;
}

BOOL CGameService::OnCloseConnect(UINT32 nConnID)
{
	if(nConnID == m_dwAccountConnID)
	{
		m_dwAccountConnID = 0;
	}

	CLoginClientMgr::GetInstancePtr()->OnCloseConnect(nConnID);

	CWatcherClient::GetInstancePtr()->OnCloseConnect(nConnID);

	m_LoginMsgHandler.OnCloseConnect(nConnID);

	return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
	ConnectToAccountSvr();

	CWatcherClient::GetInstancePtr()->OnSecondTimer();

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	if (CWatcherClient::GetInstancePtr()->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	if (CWebCommandMgr::GetInstancePtr()->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	if (GiftCodeManager::GetInstancePtr()->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	if (m_LoginMsgHandler.DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return FALSE;
}
