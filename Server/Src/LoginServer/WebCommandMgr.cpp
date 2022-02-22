#include "stdafx.h"
#include "WebCommandMgr.h"
#include "GameService.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Game_Define.pb.h"
#include "PacketHeader.h"
#include "HttpParameter.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_Account.pb.h"

CWebCommandMgr::CWebCommandMgr()
{

}

CWebCommandMgr::~CWebCommandMgr()
{

}

CWebCommandMgr* CWebCommandMgr::GetInstancePtr()
{
    static CWebCommandMgr _StaticMgr;

    return &_StaticMgr;
}


BOOL CWebCommandMgr::Init()
{
    return TRUE;
}

BOOL CWebCommandMgr::Uninit()
{
    return TRUE;
}

BOOL CWebCommandMgr::SendWebResult(INT32 nConnID, EWebResult eResult)
{
    std::string strResult = CommonConvert::IntToString((INT64)eResult);

    ServiceBase::GetInstancePtr()->SendMsgRawData(nConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, strResult.c_str(), (INT32)strResult.size());

    return TRUE;
}

BOOL CWebCommandMgr::DispatchPacket(NetPacket* pNetPacket)
{
    switch (pNetPacket->m_nMsgID)
    {
            PROCESS_MESSAGE_ITEM(MSG_PHP_GM_COMMAND_REQ, OnMsgGmCommandReq);
    }


    return FALSE;
}

BOOL CWebCommandMgr::OnMsgGmCommandReq(NetPacket* pNetPacket)
{
    ERROR_RETURN_TRUE(pNetPacket->m_pDataBuffer->GetBodyLenth() < 2000);

    CHAR szMsgBuf[2048] = { 0 };
    CommonConvert::StrCopy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth() + 1);

    HttpParameter Params;
    Params.ParseStringToMap(szMsgBuf);
    std::string strAction = Params.GetStrValue("Action");
    CLog::GetInstancePtr()->LogHiInfo("Web Action :%s,  Params:%s", strAction.c_str(), szMsgBuf);

    EWebAction eWebAction = (EWebAction)CommonConvert::StringToInt(strAction.c_str());

    switch (eWebAction)
    {
        case EWA_SEAL_ACCOUNT:
            OnGmSealAccount(Params, pNetPacket->m_nConnID);
            break;
        case EWA_SERVER_CHNAGE:
            OnGmServerChange(Params, pNetPacket->m_nConnID);
            break;
        default:
            SendWebResult(pNetPacket->m_nConnID, EWR_INVALID_ACT);
            break;
    }

    return TRUE;
}


void CWebCommandMgr::OnGmSealAccount(HttpParameter& hParams, INT32 nConnID)
{
    ERROR_RETURN_NONE(nConnID != 0);
    SealAccountReq Req;
    Req.set_accountid(hParams.GetLongValue("accountid"));
    Req.set_accountname(hParams.GetStrValue("accountname"));
    Req.set_channel(hParams.GetIntValue("channel"));
    Req.set_seal(hParams.GetIntValue("seal"));
    Req.set_sealtime(hParams.GetIntValue("sealtime"));
    CGameService::GetInstancePtr()->SendCmdToAccountConnection(MSG_SEAL_ACCOUNT_REQ, 0, nConnID, Req);
    return;
}

void CWebCommandMgr::OnGmServerChange(HttpParameter& hParams, INT32 nConnID)
{
    INT32 nSvrID = hParams.GetIntValue("serverid");
    CGameService::GetInstancePtr()->m_LoginMsgHandler.m_LogicSvrMgr.ReloadServerList(nSvrID);
    SendWebResult(nConnID, EWR_SUCCESSED);
    return;
}


