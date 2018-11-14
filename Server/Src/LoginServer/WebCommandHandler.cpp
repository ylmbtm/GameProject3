#include "stdafx.h"
#include "../Message/Msg_ID.pb.h"
#include "LoginMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "HttpParameter.h"

BOOL CLoginMsgHandler::OnMsgWebCommandReq(NetPacket* pNetPacket)
{
	CHAR szMsgBuf[1024] = { 0 };
	strncpy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	HttpParameter Params;
	Params.ParseStringToMap(szMsgBuf);
	std::string strEvent = Params.GetStrValue("event");
	CLog::GetInstancePtr()->LogError("GmCommand Event:%s", strEvent.c_str());
	
	//更新服务器配制信息
	if (strEvent == "setserverinfo")
	{
		INT32 nSvrID = Params.GetIntValue("serverid");
		m_LogicSvrMgr.ReloadServerList(nSvrID);
	}
	//封禁账号
	else if (strEvent == "seal_account")
	{
		SealAccountReq Req;
		UINT32 nConnID = pNetPacket->m_dwConnID;
		ERROR_RETURN_TRUE(nConnID != 0);
		CGameService::GetInstancePtr()->SendCmdToAccountConnection(MSG_SEAL_ACCOUNT_REQ, nConnID, 0, Req);
	}

	ServiceBase::GetInstancePtr()->SendMsgRawData(pNetPacket->m_dwConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, szMsgBuf, pNetPacket->m_pDataBuffer->GetBodyLenth());
	return TRUE;
}