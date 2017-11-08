#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "CommonThreadFunc.h"
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

	if(!CLog::GetInstancePtr()->StartLog("LogServer", "log"))
	{
		return FALSE;
	}
	CLog::GetInstancePtr()->LogError("---------服务器开始启动--------");
	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("log_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("log_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	m_LogMsgHandler.Init(0);
	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");
	return TRUE;
}


BOOL CGameService::OnNewConnect(CConnection* pConn)
{
	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection* pConn)
{
	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
		default:
		{
			m_LogMsgHandler.DispatchPacket(pNetPacket);
		}
		break;
	}

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
	UINT64 dwTickCount = 0;
	while(TRUE)
	{
		ServiceBase::GetInstancePtr()->Update();

		dwTickCount = CommonFunc::GetTickCount();

		m_LogMsgHandler.OnUpdate(dwTickCount);

		CommonThreadFunc::Sleep(1);
	}

	return TRUE;
}

