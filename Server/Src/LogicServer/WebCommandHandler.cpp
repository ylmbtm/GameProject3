#include "stdafx.h"
#include "../Message/Msg_ID.pb.h"
#include "LogicMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "HttpParameter.h"

BOOL CLogicMsgHandler::OnMsgWebCommandReq(NetPacket* pNetPacket)
{
	CHAR szMsgBuf[1024] = { 0 };
	strncpy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	HttpParameter Params;
	Params.ParseStringToMap(szMsgBuf);
	std::string strEvent = Params.GetStrValue("event");
	CLog::GetInstancePtr()->LogError("GmCommand Event:%s", strEvent.c_str());
	

	ServiceBase::GetInstancePtr()->SendMsgRawData(pNetPacket->m_dwConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, szMsgBuf, pNetPacket->m_pDataBuffer->GetBodyLenth());
	return TRUE;
}