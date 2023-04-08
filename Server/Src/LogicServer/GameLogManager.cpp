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

VOID CGameLogManager::SetLogBase(CPlayerObject* pPlayer, Log_BaseData* pLogBase)
{
    pLogBase->m_uRoleID = pPlayer->GetRoleID();
    pLogBase->m_uAccountID = pPlayer->GetAccountID();
    pLogBase->m_nAreaID = CGameService::GetInstancePtr()->GetServerID();
    pLogBase->m_uOpTime = CommonFunc::GetCurrTime();
    pLogBase->m_nChannel = (INT32)(pPlayer->GetProperty(ERP_CHANNEL));
    pLogBase->m_nLevel = (INT32)(pPlayer->GetProperty(ERP_LEVEL));
    pLogBase->m_nVipLevel = (INT32)(pPlayer->GetProperty(ERP_VIPLEVEL));
    CommonConvert::StrCopy(pLogBase->m_szRoleName, pPlayer->GetName(), sizeof(pLogBase->m_szRoleName));
}

BOOL CGameLogManager::LogRoleCreate(CPlayerObject* pPlayer)
{
    Log_RoleCreate log;
    SetLogBase(pPlayer, &log);

    WriteGameLog(log);

    return TRUE;
}

BOOL CGameLogManager::LogRoleLogin(CPlayerObject* pPlayer)
{
    Log_RoleLogin log;

    SetLogBase(pPlayer, &log);

    WriteGameLog(log);

    return TRUE;
}

BOOL CGameLogManager::LogRoleLogout(CPlayerObject* pPlayer)
{
    Log_RoleLogout log;

    SetLogBase(pPlayer, &log);

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
    Log_RoleChat log;
    SetLogBase(pPlayer, &log);
    log.m_nChatChl = nChatChl;
    CommonConvert::StrCopy(log.m_szText, strContent.c_str(), 256);
    CommonConvert::StrCopy(log.m_szTargetName, strTargetName.c_str(), sizeof(log.m_szTargetName));
    CommonConvert::StrCopy(log.m_szRoleName, pPlayer->GetName(), sizeof(log.m_szRoleName));

    log.m_uTargetID = uTargetID;
    WriteGameLog(log);
    return TRUE;
}
