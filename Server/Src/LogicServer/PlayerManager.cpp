#include "StdAfx.h"
#include "PlayerManager.h"

CPlayerManager* CPlayerManager::GetInstancePtr()
{
	static CPlayerManager _PlayerManager;

	return &_PlayerManager;
}

CPlayerObject* CPlayerManager::CreatePlayer(UINT64 u64RoleID)
{
	CPlayerObject * pPlayer = CreatePlayerByID(u64RoleID);

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
	return Delete(u64RoleID);
}

BOOL CPlayerManager::DeletePlayer(UINT32 u64RoleID)
{
	return TRUE;
}
