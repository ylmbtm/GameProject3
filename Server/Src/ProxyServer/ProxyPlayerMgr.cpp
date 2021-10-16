#include "stdafx.h"
#include "ProxyPlayerMgr.h"

CProxyPlayer::CProxyPlayer()
{
    m_u64RoleID = 0;
    m_nGameSvrID = 0;
    m_nConnID = 0;
    m_dwCopyGuid = 0;
}

CProxyPlayer::~CProxyPlayer()
{

}

UINT32 CProxyPlayer::GetGameSvrID()
{
    return m_nGameSvrID;
}

UINT64 CProxyPlayer::GetCharID()
{
    return m_u64RoleID;
}

UINT32 CProxyPlayer::GetConnID()
{
    return m_nConnID;
}

UINT32 CProxyPlayer::GetCopyGuid()
{
    return m_dwCopyGuid;
}

VOID CProxyPlayer::SetGameSvrInfo( UINT32 dwSvrID, UINT32 dwCopyGuid )
{
    m_nGameSvrID = dwSvrID;
    m_dwCopyGuid = dwCopyGuid;
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

CProxyPlayer* CProxyPlayerMgr::GetByCharID(UINT64 u64RoleID)
{
    return GetByKey(u64RoleID);
}

CProxyPlayer* CProxyPlayerMgr::CreateProxyPlayer(UINT64 u64RoleID)
{
    CProxyPlayer* pClientObj = InsertAlloc(u64RoleID);
    ERROR_RETURN_NULL(pClientObj != NULL);

    pClientObj->m_u64RoleID = u64RoleID;

    return pClientObj;
}

BOOL CProxyPlayerMgr::RemoveByCharID(UINT64 u64RoleID)
{
    return Delete(u64RoleID);
}

CProxyPlayer* CProxyPlayerMgr::GetByConnID(INT32 nConnID)
{
    return NULL;
}

