#include "stdafx.h"
#include "LogicSvrManager.h"
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

BOOL LogicSvrManager::Uninit()
{
	for (auto itor = begin(); itor != end(); itor++)
	{
		LogicServerNode* pTempNode = itor->second;

		delete pTempNode;
	}

	clear();

	m_DBConnection.close();

	return TRUE;
}

BOOL LogicSvrManager::RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwPort, UINT32 dwHttpPort, UINT32 dwWatchPort, const std::string& strSvrName)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		pNode = new LogicServerNode();
		pNode->m_dwServerID = dwServerID;
		pNode->m_dwConnID   = dwConnID;
		pNode->m_dwPort     = dwPort;
		pNode->m_dwHttpPort = dwHttpPort;
		pNode->m_dwWatchPort = dwWatchPort;
		pNode->m_strSvrName = strSvrName;
		insert(std::make_pair(dwServerID, pNode));

		char szSql[SQL_BUFF_LEN] = { 0 };
		snprintf(szSql, SQL_BUFF_LEN, "replace into server_list(id, name, ip, port,http_port,watch_port,state, min_version, max_version, check_chan, check_ip) values(%d, '%s', '%s', %d, %d, %d, %d, '%s','%s','%s','%s');",
		         pNode->m_dwServerID, pNode->m_strSvrName.c_str(),"127.0.0.1", pNode->m_dwPort, pNode->m_dwHttpPort, pNode->m_dwWatchPort, ESS_GOOD, "1.0.0", "9.0.0","*", "*");
		if (m_DBConnection.execSQL(szSql) < 0)
		{
			CLog::GetInstancePtr()->LogError("LogicSvrManager::RegisterLogicServer Error :%s", m_DBConnection.GetErrorMsg());
			return FALSE;
		}
	}
	else
	{
		pNode->m_dwConnID = dwConnID;
		if ((pNode->m_dwPort != dwPort) ||
		        (pNode->m_dwHttpPort != dwHttpPort) ||
		        (pNode->m_dwWatchPort != dwWatchPort) ||
		        (pNode->m_strSvrName != strSvrName))
		{
			pNode->m_dwServerID = dwServerID;
			pNode->m_strSvrName = strSvrName;
			pNode->m_dwPort = dwPort;
			pNode->m_dwHttpPort = dwHttpPort;
			pNode->m_dwWatchPort = dwWatchPort;

			char szSql[SQL_BUFF_LEN] = { 0 };
			snprintf(szSql, SQL_BUFF_LEN, "update server_list set name = '%s', port = %d ,http_port = %d,watch_port = %d where id = %d;",
			         pNode->m_strSvrName.c_str(), pNode->m_dwPort, pNode->m_dwHttpPort, pNode->m_dwWatchPort, pNode->m_dwServerID);
			if (m_DBConnection.execSQL(szSql) < 0)
			{
				CLog::GetInstancePtr()->LogError("LogicSvrManager::RegisterLogicServer Error :%s", m_DBConnection.GetErrorMsg());
				return FALSE;
			}
		}
	}

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

BOOL LogicSvrManager::ReloadServerList(UINT32 dwServerID)
{
	CHAR szSql[SQL_BUFF_LEN] = { 0 };
	if (dwServerID == 0)
	{
		snprintf(szSql, SQL_BUFF_LEN, "select * from server_list");
	}
	else
	{
		snprintf(szSql, SQL_BUFF_LEN, "select * from server_list where id = %d", dwServerID);
	}

	CppMySQLQuery QueryResult = m_DBConnection.querySQL(szSql);
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
		pNode->m_bDelete = FALSE;
		std::string strMinVersion = QueryResult.getStringField("min_version", "0");
		if(strMinVersion.empty())
		{
			pNode->m_dwMinVersion = 0;
		}
		else
		{
			pNode->m_dwMinVersion = CommonConvert::VersionToInt(strMinVersion);
		}

		std::string strMaxVersion = QueryResult.getStringField("max_version", "0");
		if (strMaxVersion.empty())
		{
			pNode->m_dwMaxVersion = 0;
		}
		else
		{
			pNode->m_dwMaxVersion = CommonConvert::VersionToInt(strMaxVersion);
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
				pNode->m_CheckIpList.insert(CommonSocket::IpAddrStrToInt(vtValue[i].c_str()));
			}
		}

		QueryResult.nextRow();
	}

	return TRUE;
}

BOOL LogicSvrManager::ReloadReviewVersion()
{
	m_setReviewVersion.clear();
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
	if(m_dwMinVersion == 0)
	{
		return TRUE;
	}

	UINT32 nVersion = CommonConvert::VersionToInt(strVersion);
	if(nVersion == 0)
	{
		return FALSE;
	}

	if ((nVersion < m_dwMinVersion) && (nVersion > m_dwMaxVersion))
	{
		return FALSE;
	}

	return TRUE;
}
