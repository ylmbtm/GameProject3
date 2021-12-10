#include "stdafx.h"
#include "AccountMsgHandler.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "GameLogManager.h"


CAccountMsgHandler::CAccountMsgHandler()
{

}

CAccountMsgHandler::~CAccountMsgHandler()
{

}

BOOL CAccountMsgHandler::Init(INT32 nReserved)
{
    if (!m_AccountManager.Init())
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CAccountMsgHandler::Uninit()
{
    m_AccountManager.Uninit();

    return TRUE;
}


BOOL CAccountMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
    switch(pNetPacket->m_nMsgID)
    {
            PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_REG_REQ,       OnMsgAccountRegReq);
            PROCESS_MESSAGE_ITEM(MSG_ACCOUNT_LOGIN_REQ,     OnMsgAccontLoginReq);
            PROCESS_MESSAGE_ITEM(MSG_SEAL_ACCOUNT_REQ,      OnMsgSealAccountReq);
            PROCESS_MESSAGE_ITEM(MSG_SET_LAST_SERVER_NTY,   OnMsgSetLastServerNty);
    }

    return FALSE;
}


BOOL CAccountMsgHandler::OnMsgAccountRegReq(NetPacket* pPacket)
{
    AccountRegReq Req;

    Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

    PacketHeader* pHeader = (PacketHeader*) pPacket->m_pDataBuffer->GetBuffer();

    ERROR_RETURN_TRUE(pHeader->dwUserData != 0);

    AccountRegAck Ack;

    std::string strAccountName = Req.accountname();

    if(!m_AccountManager.CheckAccountName(strAccountName, false))
    {
        Ack.set_retcode(MRC_ACCOUNT_NAME_ERR_FMT);
        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ACCOUNT_REG_ACK, 0, pHeader->dwUserData, Ack);
        return TRUE;
    }

    CommonConvert::StringTrim(strAccountName);

    CAccountObject* pAccount = m_AccountManager.GetAccountObject(strAccountName, Req.channel());
    if(pAccount != NULL)
    {
        Ack.set_retcode(MRC_ACCOUNT_NAME_EXIST);
        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ACCOUNT_REG_ACK, 0, pHeader->dwUserData, Ack);
        return TRUE;
    }

    pAccount = m_AccountManager.CreateAccountObject(strAccountName, Req.password(), Req.channel());
    if(pAccount == NULL)
    {
        Ack.set_retcode(MRC_UNKNOW_ERROR);
        CLog::GetInstancePtr()->LogError("Error CAccountMsgHandler::OnMsgAccountRegReq RetCode:MRC_UNKNOW_ERROR");
    }
    else
    {
        Ack.set_retcode(MRC_SUCCESSED);
        Ack.set_accountid(pAccount->m_ID);
        CGameLogManager::GetInstancePtr()->LogAccountCreate(pAccount->m_ID, pAccount->m_strName, Req.reglog());
    }

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ACCOUNT_REG_ACK, 0, pHeader->dwUserData, Ack);

    return TRUE;
}

BOOL CAccountMsgHandler::OnMsgAccontLoginReq(NetPacket* pPacket)
{
    AccountLoginReq Req;
    Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

    PacketHeader* pHeader = (PacketHeader*) pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pHeader->dwUserData != 0);

    AccountLoginAck Ack;
    Ack.set_review(Req.review());
    Ack.set_channel(Req.channel());
    Ack.set_ipaddr(Req.loginlog().ipaddr());
    std::string strAccountName = Req.accountname();

    if (!m_AccountManager.CheckAccountName(strAccountName, Req.fromchannel()))
    {
        Ack.set_retcode(MRC_ACCOUNT_NAME_ERR_FMT);
        CLog::GetInstancePtr()->LogError("Error CAccountMsgHandler::OnMsgAccontLoginReq Invalid Account Name:%s", strAccountName.c_str());
        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ACCOUNT_LOGIN_ACK, 0, pHeader->dwUserData, Ack);
        return TRUE;
    }

    CommonConvert::StringTrim(strAccountName);

    CAccountObject* pAccObj = m_AccountManager.GetAccountObject(strAccountName, Req.channel());
    if(pAccObj != NULL)
    {
        ERROR_RETURN_FALSE(pAccObj->m_ID != 0);
        if (pAccObj->m_uSealTime != 0 && CommonFunc::GetCurrTime() < pAccObj->m_uSealTime)
        {
            Ack.set_retcode(MRC_ACCOUNT_SEALED);
            Ack.set_lastsvrid(0);
            Ack.set_accountid(0);
        }
        else if(Req.password() != pAccObj->m_strPassword)
        {
            Ack.set_retcode(MRC_INVALID_PASSWORD);
            Ack.set_lastsvrid(0);
            Ack.set_accountid(0);
        }
        else
        {
            Ack.set_retcode(MRC_SUCCESSED);
            Ack.set_lastsvrid(pAccObj->m_nLastSvrID[0]);
            Ack.set_accountid(pAccObj->m_ID);
            CGameLogManager::GetInstancePtr()->LogAccountLogin(pAccObj->m_ID, pAccObj->m_strName, Req.loginlog());
        }

        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ACCOUNT_LOGIN_ACK, 0, pHeader->dwUserData, Ack);
        return TRUE;
    }

    if (!Req.fromchannel())
    {
        Ack.set_retcode(MRC_ACCOUNT_NAME_NOT_EXIST);
        Ack.set_lastsvrid(0);
        Ack.set_accountid(0);
        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ACCOUNT_LOGIN_ACK, 0, pHeader->dwUserData, Ack);
        return TRUE;
    }

    pAccObj = m_AccountManager.CreateAccountObject(strAccountName, Req.password(), Req.channel());
    if (pAccObj == NULL)
    {
        Ack.set_retcode(MRC_UNKNOW_ERROR);
        CLog::GetInstancePtr()->LogError("Error CAccountMsgHandler::OnMsgAccontLoginReq RetCode:MRC_UNKNOW_ERROR");
    }
    else
    {
        Ack.set_retcode(MRC_SUCCESSED);
        Ack.set_accountid(pAccObj->m_ID);
        Ack.set_lastsvrid(0);
        CGameLogManager::GetInstancePtr()->LogAccountCreate(pAccObj->m_ID, pAccObj->m_strName, Req.loginlog());
        CGameLogManager::GetInstancePtr()->LogAccountLogin(pAccObj->m_ID, pAccObj->m_strName, Req.loginlog());
    }

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ACCOUNT_LOGIN_ACK, 0, pHeader->dwUserData, Ack);

    return TRUE;
}


BOOL CAccountMsgHandler::OnMsgSealAccountReq(NetPacket* pPacket)
{
    SealAccountReq Req;
    Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*) pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pHeader->dwUserData != 0);

    SealAccountAck Ack;
    UINT64 uAccountID = Req.accountid();
    INT32 nLastSvrID = 0;
    if (m_AccountManager.SealAccount(uAccountID, Req.accountname(), Req.channel(), Req.seal(), Req.sealtime(), nLastSvrID))
    {
        Ack.set_retcode(MRC_SUCCESSED);

        if (Req.seal())
        {
            Ack.set_accountid(uAccountID);
            Ack.set_serverid(nLastSvrID);
        }
    }
    else
    {
        Ack.set_retcode(MRC_UNKNOW_ERROR);
    }

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_SEAL_ACCOUNT_ACK, 0, pHeader->dwUserData, Ack);

    return TRUE;
}

BOOL CAccountMsgHandler::OnMsgSetLastServerNty(NetPacket* pPacket)
{
    SetLastServerNty Nty;
    Nty.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

    m_AccountManager.SetLastServer(Nty.accountid(), Nty.serverid());

    return TRUE;
}
