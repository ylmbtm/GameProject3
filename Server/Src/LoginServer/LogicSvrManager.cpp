#include "StdAfx.h"
#include "LogicSvrManager.h"


LogicSvrManager::LogicSvrManager(void)
{
}


LogicSvrManager::~LogicSvrManager(void)
{
}

BOOL LogicSvrManager::Init()
{
	return TRUE;
}

BOOL LogicSvrManager::RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID)
{
	LogicServerNode *pNode = GetByKey(dwServerID);
	if(pNode == NULL)
	{
		pNode = InsertAlloc(dwServerID);
		pNode->dwServerID = dwServerID;
		pNode->dwConnID = dwConnID;
		pNode->strIpAddr = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_out_ip");
		pNode->dwPort = 9876;
	}

	pNode->dwConnID = dwConnID;
	pNode->dwServerID = dwServerID;

	return TRUE;
}

BOOL LogicSvrManager::UnregisterLogicServer(UINT32 dwConnID, UINT32 dwServerID)
{
	LogicServerNode *pNode = GetByKey(dwServerID);
	if(pNode == NULL)
	{
		return TRUE;
	}

	pNode->dwConnID = 0;

	return TRUE;
}

UINT32 LogicSvrManager::GetLogicConnID(UINT32 dwServerID)
{
	LogicServerNode *pNode = GetByKey(dwServerID);
	if(pNode == NULL)
	{
		return 0;
	}

	return pNode->dwConnID;
}

LogicServerNode* LogicSvrManager::GetLogicServerInfo(UINT32 dwServerID)
{
	LogicServerNode *pNode = GetByKey(dwServerID);
	if(pNode == NULL)
	{
		return 0;
	}

	return pNode;
}
