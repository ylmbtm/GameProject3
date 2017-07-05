#include "stdafx.h"
#include "StaticPlayerMgr.h"

CProxyPlayer::~CProxyPlayer()
{

}

UINT64 CProxyPlayer::GetGameSvrID()
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

VOID CProxyPlayer::SetGameSvrID(UINT32 dwSvrID )
{
	m_dwGameSvrID = dwSvrID;
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
	CProxyPlayer *pClientObj = InsertAlloc(u64RoleID);
	if(pClientObj == NULL)
	{
		return NULL;
	}

	pClientObj->m_u64RoleID = u64RoleID;

	return pClientObj;
}

BOOL CProxyPlayerMgr::RemoveByCharID(UINT64 u64RoleID)
{
	return Delete(u64RoleID);
}


