#include "stdafx.h"
#include "CommandDef.h"
#include "HttpMsgHandler.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "CommonSocket.h"
#include "../Message/Msg_ID.pb.h"
#include "Log.h"

CHttpMsgHandler::CHttpMsgHandler()
{

}

CHttpMsgHandler::~CHttpMsgHandler()
{

}

BOOL CHttpMsgHandler::Init(UINT32 dwReserved)
{
	return TRUE;
}

BOOL CHttpMsgHandler::Uninit()
{
	return TRUE;
}



BOOL CHttpMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_PHP_WEB_REQ,		OnMsgPhpWebReq);
		default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CHttpMsgHandler::OnMsgPhpWebReq(NetPacket* pNetPacket)
{
	char sz[20] = "abcdefghiji";

	ServiceBase::GetInstancePtr()->SendMsgRawData(pNetPacket->m_dwConnID, 12345, 0, 0, sz, 20);

	return TRUE;
}
