#include "stdafx.h"
#include "ProxyPlayerMgr.h"

CProxyPlayer::CProxyPlayer()
{
	m_u64RoleID = 0;
	m_dwGameSvrID = 0;
	m_dwConnID = 0;
	m_dwCopyGuid = 0;
}

CProxyPlayer::~CProxyPlayer()
{

}

UINT32 CProxyPlayer::GetGameSvrID()
{
	return m_dwGameSvrID;
}

UINT64 CProxyPlayer::GetCharID()
{
	return m_u64RoleID;
}

UINT32 CProxyPlayer::GetConnID()
{
	return m_dwConnID;
}

UINT32 CProxyPlayer::GetCopyGuid()
{
	return m_dwCopyGuid;
}

VOID CProxyPlayer::SetGameSvrInfo( UINT32 dwSvrID, UINT32 dwCopyGuid )
{
	m_dwGameSvrID = dwSvrID;
	m_dwCopyGuid = dwCopyGuid;
}


VOID CProxyPlayer::SetConnID(UINT32 dwConnID)
{
	m_dwConnID = dwConnID;
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

CProxyPlayer* CProxyPlayerMgr::GetByConnID(UINT32 dwConnID)
{
	return NULL;
}

