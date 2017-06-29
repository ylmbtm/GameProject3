#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer.h"
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


BOOL CGameService::Init(UINT32 dwServerID, UINT32 dwPort)
{
	CommonFunc::SetCurrentWorkPath("");

	if(!CLog::GetInstancePtr()->StartLog("GameServer", "log"))
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

	m_dwServerID = dwServerID;

	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("game_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(dwPort, nMaxConn,this))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");
		return FALSE;
	}

	if(!m_SceneManager.Init(0))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动场景管理器失败!");
		return FALSE;
	}

	ConnectToLogicSvr();

	return TRUE;
}

BOOL CGameService::OnNewConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("新连接来到!");

	if(pConn->GetConnectionID() == m_dwLogicConnID)
	{
		RegisterToLoginSvr();
	}

	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("断开连接!");

	if(m_dwLogicConnID == pConn->GetConnectionID())
	{
		m_dwLogicConnID = 0;
		ConnectToLogicSvr();
	}

	if(m_dwProxyConnID == pConn->GetConnectionID())
	{
		m_dwProxyConnID = NULL;
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
			m_SceneManager.DispatchPacket(pNetPacket);
		}
		break;
	}

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

BOOL CGameService::SetLogicConnID( UINT32 dwConnID )
{
	m_dwLogicConnID = dwConnID;

	return TRUE;
}


UINT32 CGameService::GetLogicConnID()
{
	return m_dwLogicConnID;
}

UINT32 CGameService::GetProxyConnID()
{
	return m_dwProxyConnID;
}

BOOL CGameService::ConnectToLogicSvr()
{
	UINT32 nLogicPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	std::string strLogicIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
	CConnection *pConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strLogicIp, nLogicPort);
	if(pConn == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_dwLogicConnID = pConn->GetConnectionID();

	return TRUE;
}

BOOL CGameService::ConnectToProxySvr()
{
	UINT32 nProxyPort = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	std::string strProxyIp = CConfigFile::GetInstancePtr()->GetStringValue("proxy_svr_ip");
	CConnection *pConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strProxyIp, nProxyPort);
	if(pConn == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_dwProxyConnID = pConn->GetConnectionID();

	return TRUE;
}

BOOL CGameService::RegisterToLoginSvr()
{
	GmsvrRegToLogicReq Req;

	Req.set_serverid(m_dwServerID);

	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwLogicConnID, MSG_GMSVR_REGTO_LOGIC_REQ, 0, 0, Req);
}

UINT32 CGameService::GetServerID()
{
	return m_dwServerID;
}
