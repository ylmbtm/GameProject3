#include "stdafx.h"
#include "StaticPlayerMgr.h"

CProxyPlayer::~CProxyPlayer()
{

}

UINT64 CProxyPlayer::GetGameSvrConnID()
{
	return m_GameSvrConnID;
}

UINT64 CProxyPlayer::GetCharID()
{
	return m_u64RoleID;
}

UINT32 CProxyPlayer::GetClientConnID()
{
	return m_dwClientConnID;
}

VOID CProxyPlayer::SetGameSvrConnID( UINT64 u64ConnID )
{
	m_GameSvrConnID = u64ConnID;
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


