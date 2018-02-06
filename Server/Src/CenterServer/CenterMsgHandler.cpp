#include "stdafx.h"
#include "CenterMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "LogicSvrMgr.h"

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
			PROCESS_MESSAGE_ITEM(MSG_LOGIC_REGTO_CENTER_REQ,	OnMsgLogicSvrRegReq);
	}

	return FALSE;
}


BOOL CCenterMsgHandler::OnMsgLogicSvrRegReq(NetPacket* pPacket)
{
	SvrRegToSvrReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	LogicSvrMgr::GetInstancePtr()->RegisterLogicServer(pPacket->m_dwConnID, Req.serverid(), Req.servername());


	SvrRegToSvrAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_LOGIC_REGTO_LOGIN_ACK, 0, 0, Ack);
	return TRUE;
}



