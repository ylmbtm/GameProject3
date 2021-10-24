#include "stdafx.h"
#include "ProxyPlayerMgr.h"

CProxyPlayer::CProxyPlayer()
{
    m_uRoleID = 0;
    m_nGameSvrID = 0;
    m_nConnID = 0;
    m_nCopyGuid = 0;
}

CProxyPlayer::~CProxyPlayer()
{

}

INT32 CProxyPlayer::GetGameSvrID()
{
    return m_nGameSvrID;
}

UINT64 CProxyPlayer::GetRoleID()
{
    return m_uRoleID;
}

INT32 CProxyPlayer::GetConnID()
{
    return m_nConnID;
}

INT32 CProxyPlayer::GetCopyGuid()
{
    return m_nCopyGuid;
}

VOID CProxyPlayer::SetGameSvrInfo(INT32 nSvrID, INT32 nCopyGuid)
{
    m_nGameSvrID = nSvrID;
    m_nCopyGuid = nCopyGuid;
}


VOID CProxyPlayer::SetConnID(INT32 nConnID)
{
    m_nConnID = nConnID;
}

CProxyPlayerMgr::CProxyPlayerMgr(void)
{
}

CProxyPlayerMgr::~CProxyPlayerMgr(void)
{
}

CProxyPlayerMgr* CProxyPlayerMgr::GetInstancePtr()
{
    static CProxyPlayerMgr _StaicPlayerMgr;

    return &_StaicPlayerMgr;
}

CProxyPlayer* CProxyPlayerMgr::GetByRoleID(UINT64 uRoleID)
{
    return GetByKey(uRoleID);
}

CProxyPlayer* CProxyPlayerMgr::CreateProxyPlayer(UINT64 uRoleID)
{
    CProxyPlayer* pClientObj = InsertAlloc(uRoleID);
    ERROR_RETURN_NULL(pClientObj != NULL);

    pClientObj->m_uRoleID = uRoleID;

    return pClientObj;
}

BOOL CProxyPlayerMgr::RemoveByRoleID(UINT64 uRoleID)
{
    return Delete(uRoleID);
}

CProxyPlayer* CProxyPlayerMgr::GetByConnID(INT32 nConnID)
{
    return NULL;
}

