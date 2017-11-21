#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "CommonFunc.h"
#include "CommonThreadFunc.h"
#include "Log.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
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
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_REQ, OnMsgWatchHeartBeatReq)
		default:
		{
			m_LogMsgHandler.DispatchPacket(pNetPacket);
		}

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
	UINT64 uTickCount = 0;
	while(TRUE)
	{
		ServiceBase::GetInstancePtr()->Update();

		uTickCount = CommonFunc::GetTickCount();

		m_LogMsgHandler.OnUpdate(uTickCount);

		CommonThreadFunc::Sleep(1);
	}

	return TRUE;
}

BOOL CGameService::OnMsgWatchHeartBeatReq(NetPacket* pNetPacket)
{
	WatchHeartBeatReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	WatchHeartBeatAck Ack;

	Ack.set_data(Req.data());
	Ack.set_retcode(MRC_SUCCESSED);
	Ack.set_processid(CommonFunc::GetCurProcessID());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_WATCH_HEART_BEAT_ACK, 0, 0, Ack);

	return TRUE;
}