#include "stdafx.h"
#include "GameLogManager.h"
#include "../LogData/LogStruct.h"
#include "PlayerObject.h"

CGameLogManager::CGameLogManager()
{

}

CGameLogManager::~CGameLogManager()
{

}

CGameLogManager* CGameLogManager::GetInstancePtr()
{
	static CGameLogManager _LogManager;

	return &_LogManager;
}

BOOL CGameLogManager::LogRoleCreate(CPlayerObject* pPlayer)
{
	ERROR_RETURN_FALSE(pPlayer != NULL);
	Log_RoleCreate log;

	log.m_uID = pPlayer->GetRoleID();

	WriteGameLog(log);

	return TRUE;
}

BOOL CGameLogManager::LogRoleLogin(CPlayerObject* pPlayer)
{
	ERROR_RETURN_FALSE(pPlayer != NULL);
	Log_RoleLogin log;

	log.m_uID = pPlayer->GetRoleID();

	WriteGameLog(log);

	return TRUE;
}
