#include "stdafx.h"
#include "LoginClientMgr.h"
#include "../Message/Msg_ID.pb.h"

CLoginClient::CLoginClient()
{
    m_nConnID = 0;
}

CLoginClient::~CLoginClient()
{
}

INT32 CLoginClient::GetConnID()
{
    return m_nConnID;
}

EClientStatue CLoginClient::GetClientStatue()
{
    return m_ClientStatue;
}

VOID CLoginClient::SetConnID(INT32 nConnID)
{
    m_nConnID = nConnID;
}

CLoginClientMgr::CLoginClientMgr(void)
{
}

CLoginClientMgr::~CLoginClientMgr(void)
{
}

CLoginClientMgr* CLoginClientMgr::GetInstancePtr()
{
    static CLoginClientMgr _StaicClientMgr;

    return &_StaicClientMgr;
}

CLoginClient* CLoginClientMgr::GetByConnID(INT32 nConnID)
{
    return GetByKey(nConnID);
}

BOOL CLoginClientMgr::RemoveByConnID(INT32 nConnID)
{
    return Delete(nConnID);
}

CLoginClient* CLoginClientMgr::CreateLoginClient(INT32 nConnID)
{
    CLoginClient* pLoginClient = InsertAlloc(nConnID);
    ERROR_RETURN_NULL(pLoginClient != NULL);

    pLoginClient->m_nConnID = nConnID;
    pLoginClient->m_ClientStatue = ECS_NONE;

    return pLoginClient;
}

BOOL CLoginClientMgr::CheckClientMessage(INT32 nConnID, INT32 nMsgID)
{
    return TRUE;
    CLoginClient* pLoginClient = GetByConnID(nConnID);

    if (pLoginClient == NULL)
    {
        if (nMsgID != MSG_CHECK_VERSION_REQ)
        {
            return FALSE;
        }

        pLoginClient = CreateLoginClient(nConnID);

        return TRUE;
    }

    //如果是初始状态，则只能接收MSG_CHECK_VERSION_REQ版本验证消息，否则非法
    if (pLoginClient->m_ClientStatue == ECS_NONE)
    {
        if (nMsgID == MSG_CHECK_VERSION_REQ)
        {
            pLoginClient->m_ClientStatue = ECS_VER_CHECKED;
            return TRUE;
        }

        return FALSE;
    }

    //如果版本己验证，则下一个消息必须是账号注册或登录，否则非法
    if(pLoginClient->m_ClientStatue == ECS_VER_CHECKED)
    {
        if (nMsgID == MSG_ACCOUNT_LOGIN_REQ || nMsgID == MSG_ACCOUNT_REG_REQ)
        {
            pLoginClient->m_ClientStatue = ECS_PSD_CHECKED;
            return TRUE;
        }

        return FALSE;
    }

    //如果账号密码己验证，则下一个消息必须是选服消息，否则非法
    if (pLoginClient->m_ClientStatue == ECS_PSD_CHECKED)
    {
        if (nMsgID == MSG_SERVER_LIST_REQ)
        {
            return TRUE;
        }

        return FALSE;
    }

    return TRUE;
}

BOOL CLoginClientMgr::OnCloseConnect(INT32 nConnID)
{
    RemoveByConnID(nConnID);

    return TRUE;
}

