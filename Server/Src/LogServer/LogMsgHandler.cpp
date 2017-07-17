#include "stdafx.h"
#include "CommandDef.h"
#include "LogMsgHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "Utility/CommonSocket.h"

CLogMsgHandler::CLogMsgHandler()
{

}

CLogMsgHandler::~CLogMsgHandler()
{

}

BOOL CLogMsgHandler::Init(UINT32 dwReserved)
{
	return TRUE;
}

BOOL CLogMsgHandler::Uninit()
{
	return TRUE;
}



BOOL CLogMsgHandler::DispatchPacket(NetPacket *pNetPacket)
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






