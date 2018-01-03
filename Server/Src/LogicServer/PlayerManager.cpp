#include "stdafx.h"
#include "PlayerManager.h"
#include "Log.h"
#include "RoleModule.h"
#include "../ServerData/RoleData.h"

CPlayerManager::CPlayerManager()
{

}

CPlayerManager::~CPlayerManager()
{

}

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

BOOL CPlayerManager::TryCleanPlayer()
{
	if (GetCount() > 3000)
	{
		//开始要清理人员了
		UINT64 uMinLeaveTime = 0x0fffffffff;
		UINT64 uRoleID = 0;

		CPlayerManager::TNodeTypePtr pNode = CPlayerManager::GetInstancePtr()->MoveFirst();
		ERROR_RETURN_FALSE(pNode != NULL);

		CPlayerObject* pTempObj = NULL;
		for (; pNode != NULL; pNode = CPlayerManager::GetInstancePtr()->MoveNext(pNode))
		{
			pTempObj = pNode->GetValue();
			ERROR_RETURN_FALSE(pTempObj != NULL);

			if (pTempObj->IsOnline())
			{
				continue;
			}

			CRoleModule* pRoleModule = (CRoleModule*)pTempObj->GetModuleByType(MT_ROLE);
			ERROR_RETURN_FALSE(pRoleModule != NULL);

			if (uMinLeaveTime > pRoleModule->m_pRoleDataObject->m_uLogoffTime)
			{
				uMinLeaveTime = pRoleModule->m_pRoleDataObject->m_uLogoffTime;
				uRoleID = pTempObj->GetObjectID();
			}
		}

		ReleasePlayer(uRoleID);
	}

	return TRUE;
}

