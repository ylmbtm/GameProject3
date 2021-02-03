#include "stdafx.h"
#include "GameService.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "WatcherClient.h"
CGameService::CGameService(void)
{
	m_dwLogicConnID = 0;
	m_dwLogicProcessID = 0;
}

CGameService::~CGameService(void)
{
	m_dwLogicConnID = 0;
	m_dwLogicProcessID = 0;
}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
}

BOOL CGameService::Init()
{
	CommonFunc::SetCurrentWorkDir("");

	if(!CLog::GetInstancePtr()->Start("DBServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogInfo("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	if (CommonFunc::IsAlreadyRun("DBServer" + CConfigFile::GetInstancePtr()->GetStringValue("areaid")))
	{
		CLog::GetInstancePtr()->LogError("DBServer己经在运行!");
		return FALSE;
	}

	CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("db_log_level"));

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetRealNetPort("db_svr_port");
	if (nPort <= 0)
	{
		CLog::GetInstancePtr()->LogError("配制文件db_svr_port配制错误!");
		return FALSE;
	}

	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	m_DBMsgHandler.Init(0);

	ERROR_RETURN_FALSE(m_DBWriterManger.Init());

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

	return TRUE;
}

BOOL CGameService::OnNewConnect(UINT32 nConnID)
{
	CWatcherClient::GetInstancePtr()->OnNewConnect(nConnID);

	return TRUE;
}

BOOL CGameService::OnCloseConnect(UINT32 nConnID)
{
	CWatcherClient::GetInstancePtr()->OnCloseConnect(nConnID);

	return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
	CWatcherClient::GetInstancePtr()->OnSecondTimer();

	CheckLogicServer();

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	if (CWatcherClient::GetInstancePtr()->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	if (m_DBMsgHandler.DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CGameService::SetLogicConnID(UINT32 dwConnID)
{
	m_dwLogicConnID = dwConnID;

	return TRUE;
}

UINT32 CGameService::GetLogicConnID()
{
	return m_dwLogicConnID;
}

BOOL CGameService::SetLogicProcessID(UINT32 dwProcesssID)
{
	m_dwLogicProcessID = dwProcesssID;

	return TRUE;
}

UINT32 CGameService::GetLogicProcessID()
{
	return m_dwLogicProcessID;
}

BOOL CGameService::CheckLogicServer()
{
	if (m_dwLogicProcessID <= 0)
	{
		return TRUE;
	}

	if (CommonFunc::IsProcessExist(m_dwLogicProcessID))
	{
		return TRUE;
	}

	CWatcherClient::GetInstancePtr()->StopServer();

	return TRUE;
}

BOOL CGameService::Uninit()
{
	CLog::GetInstancePtr()->LogError("==========服务器开始关闭=======================");

	ServiceBase::GetInstancePtr()->StopNetwork();

	m_DBWriterManger.Uninit();

	google::protobuf::ShutdownProtobufLibrary();

	CLog::GetInstancePtr()->LogError("==========服务器关闭完成=======================");

	return TRUE;
}

BOOL CGameService::Run()
{
	while (CWatcherClient::GetInstancePtr()->IsRun())
	{
		ServiceBase::GetInstancePtr()->Update();

		m_DBWriterManger.Update();

		CommonFunc::Sleep(1);
	}

	return TRUE;
}

