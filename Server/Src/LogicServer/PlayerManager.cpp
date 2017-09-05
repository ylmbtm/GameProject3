#include "StdAfx.h"
#include "PlayerManager.h"
#include "Log.h"

CPlayerManager* CPlayerManager::GetInstancePtr()
{
	static CPlayerManager _PlayerManager;

	return &_PlayerManager;
}

CPlayerObject* CPlayerManager::CreatePlayer(UINT64 u64RoleID)
{
	CPlayerObject* pPlayer = CreatePlayerByID(u64RoleID);
	return pPlayer;
}

CPlayerObject* CPlayerManager::GetPlayer( UINT64 u64RoleID )
{
	return GetByKey(u64RoleID);
}

CPlayerObject* CPlayerManager::CreatePlayerByID( UINT64 u64RoleID )
{
	return InsertAlloc(u64RoleID);
}

BOOL CPlayerManager::ReleasePlayer( UINT64 u64RoleID )
{
	CPlayerObject* pPlayer = GetByKey(u64RoleID);
	ERROR_RETURN_FALSE(pPlayer != NULL);

	pPlayer->OnDestroy();

	return Delete(u64RoleID);
}

BOOL CPlayerManager::DeletePlayer(UINT64 u64RoleID)
{
	return TRUE;
}
