#include "stdafx.h"
#include "HttpMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"


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
			PROCESS_MESSAGE_ITEM(MSG_PHP_GM_COMMAND_REQ,		OnMsgGmCommandReq);
	}

	return FALSE;
}

BOOL CHttpMsgHandler::OnMsgGmCommandReq(NetPacket* pNetPacket)
{
	char sz[20] = "abcdefghiji";

	ServiceBase::GetInstancePtr()->SendMsgRawData(pNetPacket->m_dwConnID, 12345, 0, 0, sz, 20);

	return TRUE;
}

BOOL CHttpMsgHandler::OnMsUpdateServerReq(NetPacket* pNetPacket)
{


	return TRUE;
}
