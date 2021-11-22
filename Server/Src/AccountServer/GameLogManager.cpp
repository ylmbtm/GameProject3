#include "stdafx.h"
#include "GameLogManager.h"
#include "CommonSocket.h"
#include "../LogData/LogStruct.h"

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

BOOL CGameLogManager::LogAccountCreate(UINT64 uAccountID, std::string strAccountName, const AccountLog& Log)
{
    Log_AccountCreate log;
    log.m_uAccountID = uAccountID;
    log.m_nChannel = Log.channel();
    log.m_dwIpAddr = Log.ipaddr();
    log.m_uOpTime = CommonFunc::GetCurrTime();
    log.m_dwVersion = CommonConvert::VersionToInt(Log.version());
    CommonConvert::StrCopy(log.m_szImei, Log.imei().c_str(), sizeof(log.m_szImei));
    CommonConvert::StrCopy(log.m_szModel, Log.imodel().c_str(), sizeof(log.m_szModel));
    CommonConvert::StrCopy(log.m_szIdfa, Log.idfa().c_str(), sizeof(log.m_szIdfa));
    CommonConvert::StrCopy(log.m_szUuid, Log.uuid().c_str(), sizeof(log.m_szUuid));
    CommonConvert::StrCopy(log.m_szOpenID, strAccountName.c_str(), sizeof(log.m_szOpenID));

    WriteGameLog(log);

    return TRUE;
}

BOOL CGameLogManager::LogAccountLogin(UINT64 uAccountID, std::string strAccountName, const AccountLog& Log)
{
    Log_AccountLogin log;
    log.m_uAccountID = uAccountID;
    log.m_nChannel = Log.channel();
    log.m_dwIpAddr = Log.ipaddr();
    log.m_uOpTime = CommonFunc::GetCurrTime();
    log.m_dwVersion = CommonConvert::VersionToInt(Log.version());
    CommonConvert::StrCopy(log.m_szImei, Log.imei().c_str(), sizeof(log.m_szImei));
    CommonConvert::StrCopy(log.m_szModel, Log.imodel().c_str(), sizeof(log.m_szModel));
    CommonConvert::StrCopy(log.m_szIdfa, Log.idfa().c_str(), sizeof(log.m_szIdfa));
    CommonConvert::StrCopy(log.m_szUuid, Log.uuid().c_str(), sizeof(log.m_szUuid));
    CommonConvert::StrCopy(log.m_szOpenID, strAccountName.c_str(), sizeof(log.m_szOpenID));

    WriteGameLog(log);

    return TRUE;
}
