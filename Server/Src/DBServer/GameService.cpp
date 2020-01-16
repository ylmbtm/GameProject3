#include "stdafx.h"
#include "GameService.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"
CGameService::CGameService(void)
{
	m_dwWatchSvrConnID	= 0;
	m_dwWatchIndex		= 0;
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

	if(!CLog::GetInstancePtr()->Start("DBServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogError("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("db_log_level"));

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	m_DBMsgHandler.Init(0);

	m_DBWriterManger.Init();

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

	return TRUE;
}



BOOL CGameService::OnNewConnect(CConnection* pConn)
{
	if (pConn->GetConnectionID() == m_dwWatchSvrConnID)
	{
		SendWatchHeartBeat();
	}

	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection* pConn)
{
	if (pConn->GetConnectionID() == m_dwWatchSvrConnID)
	{
		m_dwWatchSvrConnID = 0;
	}

	return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
	SendWatchHeartBeat();

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_ACK, OnMsgWatchHeartBeatAck)
	}

	if (m_DBMsgHandler.DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return FALSE;
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

		CommonFunc::Sleep(1);
	}

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

BOOL CGameService::OnMsgWatchHeartBeatAck(NetPacket* pNetPacket)
{
	WatchHeartBeatAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

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
