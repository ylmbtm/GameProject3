#include "stdafx.h"
#include "CommandDef.h"
#include "WatchMsgHandler.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "CommonSocket.h"
#include "../Message/Msg_ID.pb.h"
#include "Log.h"

CWatchMsgHandler::CWatchMsgHandler()
{

}

CWatchMsgHandler::~CWatchMsgHandler()
{

}

BOOL CWatchMsgHandler::Init(UINT32 dwReserved)
{
	//根据清单启动所有的进程

	//



	return TRUE;
}

BOOL CWatchMsgHandler::Uninit()
{
	return TRUE;
}

BOOL CWatchMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	return TRUE;
}

BOOL CWatchMsgHandler::OnUpdate(UINT64 uTick)
{
	return TRUE;
}


BOOL CWatchMsgHandler::OnMsUpdateServerReq(NetPacket* pNetPacket)
{
	/*
	ReadConfigFile();
	MD5校验;
	更新失败

	更新文件
	检查

	//检查完成，更新完成
	*/
	return TRUE;
}

BOOL CWatchMsgHandler::OnMsgStartServerReq(NetPacket* pNetPacket)
{


	return TRUE;

}


BOOL CWatchMsgHandler::OnMsgStopServerReq(NetPacket* pNetPacket)
{


	return TRUE;

}

