#include "stdafx.h"
#include "GameLogManager.h"
#include "CommonSocket.h"
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
	log.m_uAccountID = pPlayer->GetAccountID();
	strncpy(log.m_szRoleName, pPlayer->GetName(), 63);
	log.m_dwAreaID = CGameService::GetInstancePtr()->GetServerID();
	log.m_uOpTime = CommonFunc::GetCurrTime();
	log.m_dwChannel = 1;
	log.m_nLeve = (UINT32)(pPlayer->GetProperty(ERP_LEVEL));
	log.m_nVipLevel = (UINT32)(pPlayer->GetProperty(ERP_VIPLEVEL));
	WriteGameLog(log);

	return TRUE;
}

BOOL CGameLogManager::LogRoleLogin(CPlayerObject* pPlayer)
{
	ERROR_RETURN_FALSE(pPlayer != NULL);
	Log_RoleLogin log;

	log.m_uID = pPlayer->GetRoleID();
	log.m_uAccountID = pPlayer->GetAccountID();
	strncpy(log.m_szRoleName, pPlayer->GetName(), 63);
	log.m_dwAreaID = CGameService::GetInstancePtr()->GetServerID();
	log.m_uOpTime = CommonFunc::GetCurrTime();
	log.m_dwChannel = 1;
	log.m_nLeve = (UINT32)(pPlayer->GetProperty(ERP_LEVEL));
	log.m_nVipLevel = (UINT32)(pPlayer->GetProperty(ERP_VIPLEVEL));

	WriteGameLog(log);

	return TRUE;
}
