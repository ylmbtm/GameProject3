#include "stdafx.h"
#include "GameLogManager.h"
#include "CommonSocket.h"
#include "../LogData/LogStruct.h"
#include "PlayerObject.h"
#include "RoleModule.h"

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
	CommonConvert::StrCopy(log.m_szRoleName, pPlayer->GetName(), sizeof(log.m_szRoleName));
	log.m_dwAreaID = CGameService::GetInstancePtr()->GetServerID();
	log.m_uOpTime = CommonFunc::GetCurrTime();
	log.m_dwChannel = (UINT32)(pPlayer->GetProperty(ERP_CHANNEL));
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
	CommonConvert::StrCopy(log.m_szRoleName, pPlayer->GetName(), sizeof(log.m_szRoleName));
	log.m_dwAreaID = CGameService::GetInstancePtr()->GetServerID();
	log.m_uOpTime = CommonFunc::GetCurrTime();
	log.m_dwChannel = (UINT32)(pPlayer->GetProperty(ERP_CHANNEL));
	log.m_nLeve = (UINT32)(pPlayer->GetProperty(ERP_LEVEL));
	log.m_nVipLevel = (UINT32)(pPlayer->GetProperty(ERP_VIPLEVEL));

	WriteGameLog(log);

	return TRUE;
}

BOOL CGameLogManager::LogRoleLogout(CPlayerObject* pPlayer)
{
	ERROR_RETURN_FALSE(pPlayer != NULL);
	Log_RoleLogout log;

	log.m_uID = pPlayer->GetRoleID();
	log.m_uAccountID = pPlayer->GetAccountID();
	CommonConvert::StrCopy(log.m_szRoleName, pPlayer->GetName(), sizeof(log.m_szRoleName));
	log.m_dwAreaID = CGameService::GetInstancePtr()->GetServerID();
	log.m_uOpTime = CommonFunc::GetCurrTime();
	log.m_dwChannel = (UINT32)(pPlayer->GetProperty(ERP_CHANNEL));
	log.m_nLeve = (UINT32)(pPlayer->GetProperty(ERP_LEVEL));
	log.m_nVipLevel = (UINT32)(pPlayer->GetProperty(ERP_VIPLEVEL));

	WriteGameLog(log);

	CRoleModule* pRoleModule = (CRoleModule*)pPlayer->GetModuleByType(MT_ROLE);
	ERROR_RETURN_FALSE(pRoleModule != NULL);

	if (CommonFunc::DiffDays(pRoleModule->GetLastLogonTime(), pRoleModule->GetLastLogoffTime()) > 0)
	{
		LogRoleLogin(pPlayer);
	}

	return TRUE;
}
BOOL CGameLogManager::LogRoleChat(CPlayerObject* pPlayer, INT32 nChatChl, std::string strContent, UINT64 uTargetID, std::string strTargetName)
{
	ERROR_RETURN_FALSE(pPlayer != NULL);
	Log_RoleChat log;
	log.m_uID = pPlayer->GetRoleID();
	log.m_dwAreaID = CGameService::GetInstancePtr()->GetServerID();
	log.m_uOpTime = CommonFunc::GetCurrTime();
	log.m_dwChannel = (UINT32)(pPlayer->GetProperty(ERP_CHANNEL));
	log.m_nLeve = (UINT32)(pPlayer->GetProperty(ERP_LEVEL));
	log.m_nVipLevel = (UINT32)(pPlayer->GetProperty(ERP_VIPLEVEL));
	log.m_nChatChl = nChatChl;
	CommonConvert::StrCopy(log.m_szText, strContent.c_str(), 256);
	CommonConvert::StrCopy(log.m_szTargetName, strTargetName.c_str(), sizeof(log.m_szTargetName));
	CommonConvert::StrCopy(log.m_szSrcName, pPlayer->GetName(), sizeof(log.m_szSrcName));

	log.m_uTargetID = uTargetID;
	WriteGameLog(log);
	return TRUE;
}
