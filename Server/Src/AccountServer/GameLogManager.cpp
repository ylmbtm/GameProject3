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
	log.m_uID = uAccountID;
	log.m_dwChannel = Log.channel();
	log.m_dwIpAddr = Log.ipaddr();
	log.m_uOpTime = CommonFunc::GetCurrTime();
	log.m_dwVersion = CommonConvert::VersionToInt(Log.version());
	strncpy(log.m_szImei, Log.imei().c_str(), sizeof(log.m_szImei));
	strncpy(log.m_szModel, Log.imei().c_str(), sizeof(log.m_szModel));
	strncpy(log.m_szIdfa, Log.imei().c_str(), sizeof(log.m_szIdfa));
	strncpy(log.m_szUuid, Log.imei().c_str(), sizeof(log.m_szUuid));
	strncpy(log.m_szOpenID, strAccountName.c_str(), sizeof(log.m_szOpenID));

	WriteGameLog(log);

	return TRUE;
}

BOOL CGameLogManager::LogAccountLogin(UINT64 uAccountID, std::string strAccountName, const AccountLog& Log)
{
	Log_AccountLogin log;
	log.m_uID = uAccountID;
	log.m_dwChannel = Log.channel();
	log.m_dwIpAddr = Log.ipaddr();
	log.m_uOpTime = CommonFunc::GetCurrTime();
	log.m_dwVersion = CommonConvert::VersionToInt(Log.version());
	strncpy(log.m_szImei, Log.imei().c_str(), sizeof(log.m_szImei));
	strncpy(log.m_szModel, Log.imei().c_str(), sizeof(log.m_szModel));
	strncpy(log.m_szIdfa, Log.imei().c_str(), sizeof(log.m_szIdfa));
	strncpy(log.m_szUuid, Log.imei().c_str(), sizeof(log.m_szUuid));

	WriteGameLog(log);

	return TRUE;
}
