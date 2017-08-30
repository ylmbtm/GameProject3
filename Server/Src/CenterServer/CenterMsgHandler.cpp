#include "stdafx.h"
#include "CommandDef.h"
#include "CenterMsgHandler.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "CommonSocket.h"

CCenterMsgHandler::CCenterMsgHandler()
{

}

CCenterMsgHandler::~CCenterMsgHandler()
{

}

BOOL CCenterMsgHandler::Init(UINT32 dwReserved)
{
	return TRUE;
}

BOOL CCenterMsgHandler::Uninit()
{
	return TRUE;
}



BOOL CCenterMsgHandler::DispatchPacket(NetPacket* pNetPacket)
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






