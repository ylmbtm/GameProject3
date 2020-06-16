#include "stdafx.h"
#include "LogicSvrManager.h"
#include "CommonSocket.h"



LogicSvrManager::LogicSvrManager(void)
{
	m_dwRecommendSvrID = 0;

	m_IsRun = FALSE;
}

LogicSvrManager::~LogicSvrManager(void)
{
	m_IsRun = FALSE;

	m_setReviewVersion.clear();
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
		CLog::GetInstancePtr()->LogError("LogicSvrManager::Init Error: Can not open mysql database! Reason:%s", m_DBConnection.GetErrorMsg());
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

	m_IsRun = TRUE;

	m_pThread = new std::thread(&LogicSvrManager::SaveLogicServerInfo, this);

	ERROR_RETURN_FALSE(m_pThread != NULL);

	return TRUE;
}

BOOL LogicSvrManager::Uninit()
{
	m_IsRun = FALSE;

	m_pThread->join();

	delete m_pThread;

	m_pThread = NULL;

	for (auto itor = begin(); itor != end(); itor++)
	{
		LogicServerNode* pTempNode = itor->second;

		delete pTempNode;
	}

	clear();

	m_DBConnection.close();

	return TRUE;
}

BOOL LogicSvrManager::RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwPort, UINT32 dwHttpPort, UINT32 dwWatchPort, const std::string& strSvrName, const std::string& strInnderIp)
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
		pNode->m_strInnerAddr = strInnderIp;
		pNode->m_eChangeStatus = EUS_NEW_REG;
		pNode->m_ServerStatus = ESS_SVR_ONLINE;
		insert(std::make_pair(dwServerID, pNode));
		m_ArrChangedNode.push(pNode);
	}
	else
	{
		pNode->m_dwConnID = dwConnID;
		pNode->m_ServerStatus = ESS_SVR_ONLINE;
		if ((pNode->m_dwPort != dwPort) ||
		        (pNode->m_dwHttpPort != dwHttpPort) ||
		        (pNode->m_dwWatchPort != dwWatchPort) ||
		        (pNode->m_strSvrName != strSvrName))
		{
			pNode->m_dwServerID = dwServerID;
			pNode->m_strSvrName = strSvrName;
			pNode->m_strInnerAddr = strInnderIp;
			pNode->m_dwPort = dwPort;
			pNode->m_dwHttpPort = dwHttpPort;
			pNode->m_dwWatchPort = dwWatchPort;
			pNode->m_eChangeStatus = EUS_RE_REG;
			m_ArrChangedNode.push(pNode);
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

BOOL LogicSvrManager::UpdateLogicServerInfo(UINT32 dwServerID, UINT32 dwMaxOnline, UINT32 dwCurOnline, UINT32 dwTotal, UINT32 dwCacheNum, UINT32 dwStatus, const std::string& strSvrName)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	ERROR_RETURN_FALSE(pNode != NULL);

	if ((pNode->m_dwMaxOnline != dwMaxOnline) || (pNode->m_dwCurOnline != dwCurOnline) || (pNode->m_dwTotalNum != dwTotal))
	{
		pNode->m_dwMaxOnline = dwMaxOnline;
		pNode->m_dwCurOnline = dwCurOnline;
		pNode->m_dwTotalNum = dwTotal;
		pNode->m_dwCacheNum = dwCacheNum;
		pNode->m_ServerStatus = ESS_SVR_ONLINE;
		if (pNode->m_eChangeStatus == EUS_NONE)
		{
			pNode->m_eChangeStatus = EUS_UPDATE;
			m_ArrChangedNode.push(pNode);
		}
	}

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

BOOL LogicSvrManager::OnCloseConnect(UINT32 dwConnID)
{
	for (auto itor = begin(); itor != end(); itor++)
	{
		LogicServerNode* pNode = itor->second;
		if (pNode->m_dwConnID == dwConnID)
		{
			pNode->m_ServerStatus = ESS_SVR_OFFLINE;
			pNode->m_eChangeStatus = EUS_UPDATE;
		}
	}

	return TRUE;
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
	if (!m_DBConnection.ping())
	{
		m_DBConnection.reconnect();
	}

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
		pNode->m_ServerFlag = QueryResult.getIntField("svr_flag");
		pNode->m_CornerMark = QueryResult.getIntField("corner_mark");
		pNode->m_strOuterAddr = QueryResult.getStringField("outer_ip", "*");
		pNode->m_strInnerAddr = QueryResult.getStringField("inner_ip", "*");
		pNode->m_dwPort = QueryResult.getIntField("port", 0);
		pNode->m_uSvrOpenTime = QueryResult.getInt64Field("opentime");
		if (pNode->m_strOuterAddr.empty() || pNode->m_strOuterAddr == "*")
		{
			CLog::GetInstancePtr()->LogError("ReloadServerList Failed, Serverid:%d need a domain name or outer ip address!", dwSvrID);
			return FALSE;
		}

		if (pNode->m_strInnerAddr.empty() || pNode->m_strInnerAddr == "*")
		{
			CLog::GetInstancePtr()->LogError("ReloadServerList Failed, Serverid:%d has no inner ip address!", dwSvrID);
			return FALSE;
		}

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
	if (!m_DBConnection.ping())
	{
		m_DBConnection.reconnect();
	}

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


BOOL LogicSvrManager::SaveLogicServerInfo()
{
	LogicServerNode* pTempNode = NULL;

	CHAR szSql[SQL_BUFF_LEN] = { 0 };

	while (m_IsRun)
	{
		if (m_ArrChangedNode.size() <= 0)
		{
			CommonFunc::Sleep(100);
			continue;
		}



		while (m_ArrChangedNode.pop(pTempNode) && (pTempNode != NULL))
		{
			if (pTempNode->m_eChangeStatus == EUS_NONE)
			{
				continue;
			}

			if (!m_DBConnection.ping())
			{
				m_DBConnection.reconnect();
			}

			if (pTempNode->m_eChangeStatus == EUS_NEW_REG)
			{
				snprintf(szSql, SQL_BUFF_LEN, "replace into server_list(id, name, outer_ip,inner_ip, port,http_port,watch_port,svr_flag, corner_mark,min_version, max_version, check_chan, check_ip) values(%d, '%s', '%s','%s', %d, %d, %d, %d, %d, '%s','%s','%s','%s');",
				         pTempNode->m_dwServerID, pTempNode->m_strSvrName.c_str(), "127.0.0.1", pTempNode->m_strInnerAddr.c_str(), pTempNode->m_dwPort, pTempNode->m_dwHttpPort, pTempNode->m_dwWatchPort, ESF_GOOD, 0, "1.0.0", "9.0.0", "*", "*");
				if (m_DBConnection.execSQL(szSql) < 0)
				{
					CLog::GetInstancePtr()->LogError("LogicSvrManager::SaveLogicServerInfo Error :%s", m_DBConnection.GetErrorMsg());
					return FALSE;
				}

				memset(szSql, 0, SQL_BUFF_LEN);
				snprintf(szSql, SQL_BUFF_LEN, "replace into server_status(id, name, curr_online, max_online,total_cnt,cache_cnt,update_time,status, file_version) values(%d, '%s', %d, %d, %d, %d, %d, %d,%d);",
				         pTempNode->m_dwServerID, pTempNode->m_strSvrName.c_str(), pTempNode->m_dwCurOnline, pTempNode->m_dwMaxOnline, pTempNode->m_dwTotalNum, pTempNode->m_dwCacheNum, 0, pTempNode->m_ServerStatus, 0);
				if (m_DBConnection.execSQL(szSql) < 0)
				{
					CLog::GetInstancePtr()->LogError("LogicSvrManager::SaveLogicServerInfo Error :%s", m_DBConnection.GetErrorMsg());
				}
			}

			if (pTempNode->m_eChangeStatus == EUS_RE_REG)
			{
				snprintf(szSql, SQL_BUFF_LEN, "update server_list set name = '%s', port = %d ,http_port = %d,watch_port = %d, inner_ip ='%s' where id = %d;",
				         pTempNode->m_strSvrName.c_str(), pTempNode->m_dwPort, pTempNode->m_dwHttpPort, pTempNode->m_dwWatchPort, pTempNode->m_strInnerAddr.c_str(),  pTempNode->m_dwServerID);
				if (m_DBConnection.execSQL(szSql) < 0)
				{
					CLog::GetInstancePtr()->LogError("LogicSvrManager::RegisterLogicServer Error :%s", m_DBConnection.GetErrorMsg());
				}

				memset(szSql, 0, SQL_BUFF_LEN);
				snprintf(szSql, SQL_BUFF_LEN, "replace into server_status(id, name, curr_online, max_online,total_cnt,cache_cnt,update_time,status, file_version) values(%d, '%s', %d, %d, %d, %d, %d, %d,%d);",
				         pTempNode->m_dwServerID, pTempNode->m_strSvrName.c_str(), pTempNode->m_dwCurOnline, pTempNode->m_dwMaxOnline, pTempNode->m_dwTotalNum, pTempNode->m_dwCacheNum, 0, pTempNode->m_ServerStatus, 0);
				if (m_DBConnection.execSQL(szSql) < 0)
				{
					CLog::GetInstancePtr()->LogError("LogicSvrManager::RegisterLogicServer Error :%s", m_DBConnection.GetErrorMsg());
				}
			}

			if (pTempNode->m_eChangeStatus == EUS_UPDATE)
			{
				snprintf(szSql, SQL_BUFF_LEN, "replace into server_status(id, name, curr_online, max_online,total_cnt,cache_cnt, update_time,status, file_version) values(%d, '%s', %d, %d, %d, %d, %d, %d,%d);",
				         pTempNode->m_dwServerID, pTempNode->m_strSvrName.c_str(), pTempNode->m_dwCurOnline, pTempNode->m_dwMaxOnline, pTempNode->m_dwTotalNum, pTempNode->m_dwCacheNum, 0, pTempNode->m_ServerStatus, 0);
				if (m_DBConnection.execSQL(szSql) < 0)
				{
					CLog::GetInstancePtr()->LogError("LogicSvrManager::RegisterLogicServer Error :%s", m_DBConnection.GetErrorMsg());
				}
			}

			pTempNode->m_eChangeStatus = EUS_NONE;
		}
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

	if ((nVersion <= m_dwMinVersion) && (nVersion >= m_dwMaxVersion))
	{
		return FALSE;
	}

	return TRUE;
}
