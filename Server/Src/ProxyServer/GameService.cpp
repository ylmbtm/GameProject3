#include "stdafx.h"
#include "GameService.h"
#include "ObjectID.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "Utility/Position.h"

CGameService::CGameService(void)
{
	m_dwLogicConnID = 0;
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

	if(!CLog::GetInstancePtr()->StartLog("ProxyServer","log"))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn,this))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");
		return FALSE;
	}


	if(!m_ProxyMsgHandler.Init(0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

BOOL CGameService::OnNewConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("新连接来到!");
	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("断开连接!");
	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket *pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
	default:
		{

		}
		break;
	}

	return TRUE;
}

UINT32 CGameService::GetLogicConnID()
{
	return m_dwLogicConnID;
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






