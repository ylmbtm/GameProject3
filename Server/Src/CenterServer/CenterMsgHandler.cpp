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

BOOL CCenterMsgHandler::Init(INT32 nReserved)
{

    return TRUE;
}

BOOL CCenterMsgHandler::Uninit()
{
    return TRUE;
}

BOOL CCenterMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
    switch(pNetPacket->m_nMsgID)
    {
            PROCESS_MESSAGE_ITEM(MSG_LOGIC_REGTO_CENTER_REQ,    OnMsgLogicSvrRegReq);
    }

    return FALSE;
}


BOOL CCenterMsgHandler::OnMsgLogicSvrRegReq(NetPacket* pPacket)
{
    SvrRegToSvrReq Req;
    Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
    CLogicSvrMgr::GetInstancePtr()->RegisterLogicServer(pPacket->m_nConnID, Req.serverid(), Req.servername());
    CLog::GetInstancePtr()->LogInfo("ServerID:%d Register Successed!", Req.serverid());

    SvrRegToSvrAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_LOGIC_REGTO_CENTER_ACK, 0, 0, Ack);
    return TRUE;
}



