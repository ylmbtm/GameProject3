#include "StdAfx.h"
#include "LogicSvrManager.h"


LogicSvrManager::LogicSvrManager(void)
{
	m_dwRecommendSvrID = 0;
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
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		LogicServerNode tempNode;
		tempNode.dwServerID = dwServerID;
		tempNode.dwConnID   = dwConnID;
		tempNode.strIpAddr  = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_out_ip");
		tempNode.dwPort     = 9876;
		tempNode.strServerName = "Test_Server1";
		insert(std::make_pair(dwServerID, tempNode));
		return TRUE;
	}

	pNode->dwConnID = dwConnID;
	pNode->dwServerID = dwServerID;

	return TRUE;
}

BOOL LogicSvrManager::UnregisterLogicServer(UINT32 dwConnID, UINT32 dwServerID)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		return TRUE;
	}

	pNode->dwConnID = 0;

	return TRUE;
}

UINT32 LogicSvrManager::GetLogicConnID(UINT32 dwServerID)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		return 0;
	}

	return pNode->dwConnID;
}

LogicServerNode* LogicSvrManager::GetLogicServerInfo(UINT32 dwServerID)
{
	auto itor = find(dwServerID);
	if(itor != end())
	{
		return &itor->second;
	}

	return NULL;
}

LogicServerNode* LogicSvrManager::GetRecommendServerInfo()
{
	LogicServerNode* pNode = GetLogicServerInfo(m_dwRecommendSvrID);
	if(pNode == NULL)
	{
		for(auto itor = begin(); itor != end(); itor++)
		{
			LogicServerNode* pNode = &itor->second;
			if (pNode->dwServerID != 0)
			{
				return pNode;

			}
		}
	}

	return pNode;
}
