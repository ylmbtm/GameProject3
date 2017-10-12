#include "stdafx.h"
#include "LogicSvrManager.h"
#include "CommonConvert.h"
#include "Log.h"
#include "CommonSocket.h"


LogicSvrManager::LogicSvrManager(void)
{
	m_dwRecommendSvrID = 0;
}


LogicSvrManager::~LogicSvrManager(void)
{
}

BOOL LogicSvrManager::Init()
{
	std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_ip");
	UINT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_gm_svr_port");
	std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_user");
	std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_pwd");
	std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_db_name");

	if(!m_DBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
	{
		CLog::GetInstancePtr()->LogError("ReloadServerList Error: Can not open database!!!");
		return FALSE;
	}

	if(!ReloadServerList())
	{
		return FALSE;
	}

	if(!ReloadReviewVersion())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL LogicSvrManager::RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwPort, std::string strSvrName)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		LogicServerNode* pTempNode = new LogicServerNode();
		pTempNode->m_dwServerID = dwServerID;
		pTempNode->m_dwConnID   = dwConnID;
		pTempNode->m_dwPort     = dwPort;
		pTempNode->m_strSvrName = strSvrName;
		insert(std::make_pair(dwServerID, pTempNode));

		char szSql[1024];
		sprintf_s(szSql, 1024, "replace into server_list(id, name,port) values(%d, '%s', %d);",	dwServerID, strSvrName.c_str(), dwPort);
		m_DBConnection.execSQL(szSql);

		return TRUE;
	}

	pNode->m_dwConnID = dwConnID;
	pNode->m_dwServerID = dwServerID;
	pNode->m_strSvrName = strSvrName;

	return TRUE;
}

BOOL LogicSvrManager::UnregisterLogicServer(UINT32 dwConnID, UINT32 dwServerID)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		return TRUE;
	}

	pNode->m_dwConnID = 0;

	return TRUE;
}

UINT32 LogicSvrManager::GetLogicConnID(UINT32 dwServerID)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		return 0;
	}

	return pNode->m_dwConnID;
}

LogicServerNode* LogicSvrManager::GetLogicServerInfo(UINT32 dwServerID)
{
	auto itor = find(dwServerID);
	if(itor != end())
	{
		return itor->second;
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
			LogicServerNode* pNode = itor->second;
			if (pNode->m_dwServerID != 0)
			{
				return pNode;

			}
		}
	}

	return pNode;
}

BOOL LogicSvrManager::IsReviewVersion(std::string strPackageName)
{
	if(m_setReviewVersion.find(strPackageName) == m_setReviewVersion.end())
	{
		return FALSE;
	}

	return TRUE;

}

BOOL LogicSvrManager::ReloadServerList()
{
	CppMySQLQuery QueryResult = m_DBConnection.querySQL("select * from server_list");
	while(!QueryResult.eof())
	{
		UINT32 dwSvrID = QueryResult.getIntField("id");
		LogicServerNode* pNode = GetLogicServerInfo(dwSvrID);
		if(pNode == NULL)
		{
			pNode = new LogicServerNode();
			pNode->m_dwServerID = dwSvrID;
			insert(std::make_pair(dwSvrID, pNode));
		}
		pNode->m_strSvrName = QueryResult.getStringField("name");
		pNode->m_Statue = QueryResult.getIntField("statue");
		pNode->m_Flag = QueryResult.getIntField("flag");
		pNode->m_strIpAddr = QueryResult.getStringField("ip", "*");
		pNode->m_dwPort = QueryResult.getIntField("port", 0);
		std::string strCheckVersion = QueryResult.getStringField("check_version", "*");
		if(strCheckVersion.empty() || strCheckVersion == "*")
		{
			pNode->m_dwCheckVersion = 0;
		}
		else
		{
			pNode->m_dwCheckVersion = 0;
		}

		std::string strCheckChannel = QueryResult.getStringField("check_chan", "*");
		if(strCheckChannel.empty() || strCheckChannel == "*")
		{
			pNode->m_CheckChannelList.clear();
		}
		else
		{
			std::vector<std::string> vtValue;
			CommonConvert::SpliteString(strCheckChannel, ";", vtValue);
			for(int i = 0; i < vtValue.size(); i++)
			{
				pNode->m_CheckChannelList.insert(CommonConvert::StringToInt(vtValue[i].c_str()));
			}

		}

		std::string strCheckIp = QueryResult.getStringField("check_ip", "*");
		if(strCheckIp.empty() || strCheckIp == "*")
		{
			pNode->m_CheckIpList.clear();
		}
		else
		{
			std::vector<std::string> vtValue;
			CommonConvert::SpliteString(strCheckIp,  ";", vtValue);
			for(int i = 0; i < vtValue.size(); i++)
			{
				pNode->m_CheckIpList.insert(vtValue[i]);
			}
		}

		QueryResult.nextRow();
	}

	return TRUE;
}

BOOL LogicSvrManager::ReloadReviewVersion()
{
	CppMySQLQuery QueryResult = m_DBConnection.querySQL("select * from review_client");
	while(!QueryResult.eof())
	{
		UINT32 dwID = QueryResult.getIntField("id");
		std::string strVersion = QueryResult.getStringField("client_version");

		m_setReviewVersion.insert(strVersion);

		QueryResult.nextRow();
	}

	return TRUE;
}


BOOL LogicServerNode::CheckIP( UINT32 dwIPaddr )
{
	if(m_CheckIpList.size() <= 0)
	{
		return TRUE;
	}

	return TRUE;
}

BOOL LogicServerNode::CheckChannel( UINT32 dwChannel )
{
	if(m_CheckChannelList.size() <= 0)
	{
		return TRUE;
	}

	return TRUE;
}

BOOL LogicServerNode::CheckVersion( std::string strVersion )
{
	if(m_dwCheckVersion == 0)
	{
		return TRUE;
	}

	UINT32 nVersion = CommonConvert::VersionToInt(strVersion);
	if(nVersion == 0)
	{
		return FALSE;
	}

	return TRUE;
}
