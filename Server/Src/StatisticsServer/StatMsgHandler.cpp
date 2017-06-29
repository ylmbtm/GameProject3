#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "StatMsgHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "Utility/CommonSocket.h"

CStatMsgHandler::CStatMsgHandler()
{

}

CStatMsgHandler::~CStatMsgHandler()
{

}

BOOL CStatMsgHandler::Init(UINT32 dwReserved)
{
	return TRUE;
}

BOOL CStatMsgHandler::Uninit()
{
	return TRUE;
}



BOOL CStatMsgHandler::DispatchPacket(NetPacket *pNetPacket)
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






