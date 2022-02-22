#include "stdafx.h"
#include "LogicSvrManager.h"
#include "CommonSocket.h"

LogicSvrManager::LogicSvrManager(void)
{
    m_IsRun = FALSE;
}

LogicSvrManager::~LogicSvrManager(void)
{
    m_IsRun = FALSE;
}

BOOL LogicSvrManager::Init()
{
    if(!ReloadServerList())
    {
        return FALSE;
    }

    m_IsRun = TRUE;

    m_pThread = new std::thread(&LogicSvrManager::SaveLogicServerThread, this);

    ERROR_RETURN_FALSE(m_pThread != NULL);

    return TRUE;
}

BOOL LogicSvrManager::Uninit()
{
    m_IsRun = FALSE;

    if (m_pThread != NULL)
    {
        m_pThread->join();

        delete m_pThread;

        m_pThread = NULL;
    }

    for (auto itor = begin(); itor != end(); itor++)
    {
        LogicServerNode* pTempNode = itor->second;

        delete pTempNode;
    }

    clear();

    return TRUE;
}

BOOL LogicSvrManager::RegisterLogicServer(INT32 nConnID, INT32 nServerID, INT32 nPort, INT32 nHttpPort, const std::string& strSvrName, const std::string& strInnderIp)
{
    LogicServerNode* pNode = GetLogicServerInfo(nServerID);
    if(pNode == NULL)
    {
        pNode = new LogicServerNode();
        pNode->m_nServerID = nServerID;
        pNode->m_nConnID   = nConnID;
        pNode->m_nPort     = nPort;
        pNode->m_nHttpPort = nHttpPort;
        pNode->m_strSvrName = strSvrName;
        pNode->m_strInnerAddr = strInnderIp;
        pNode->m_uSvrOpenTime = CommonFunc::GetCurrTime();
        pNode->m_eChangeStatus = EUS_NEW_REG;
        pNode->m_ServerStatus = ESS_SVR_ONLINE;
        insert(std::make_pair(nServerID, pNode));
        m_ArrChangedNode.push(pNode);
    }
    else
    {
        pNode->m_nConnID = nConnID;
        if ((pNode->m_nPort != nPort) ||
                (pNode->m_nHttpPort != nHttpPort) ||
                (pNode->m_ServerStatus != ESS_SVR_ONLINE) ||
                (pNode->m_strSvrName != strSvrName))
        {
            pNode->m_nServerID = nServerID;
            pNode->m_strSvrName = strSvrName;
            pNode->m_strInnerAddr = strInnderIp;
            pNode->m_nPort = nPort;
            pNode->m_nHttpPort = nHttpPort;
            pNode->m_eChangeStatus = EUS_RE_REG;
            pNode->m_ServerStatus = ESS_SVR_ONLINE;
            m_ArrChangedNode.push(pNode);
        }
    }

    return TRUE;
}

BOOL LogicSvrManager::UnregisterLogicServer(INT32 nConnID, INT32 nServerID)
{
    LogicServerNode* pNode = GetLogicServerInfo(nServerID);
    if(pNode == NULL)
    {
        return TRUE;
    }

    pNode->m_nConnID = 0;

    return TRUE;
}

BOOL LogicSvrManager::UpdateLogicServerInfo(INT32 nServerID, INT32 nMaxOnline, INT32 nCurOnline, INT32 nTotal, INT32 nCacheNum, INT32 nStatus, INT32 nErrorCount, const std::string& strSvrName)
{
    LogicServerNode* pNode = GetLogicServerInfo(nServerID);
    ERROR_RETURN_FALSE(pNode != NULL);

    if ((pNode->m_nMaxOnline != nMaxOnline) || (pNode->m_nCurOnline != nCurOnline) || (pNode->m_nTotalNum != nTotal) || (pNode->m_nCacheNum != nCacheNum) || (pNode->m_nErrorCnt != nErrorCount))
    {
        pNode->m_nMaxOnline = nMaxOnline;
        pNode->m_nCurOnline = nCurOnline;
        pNode->m_nTotalNum  = nTotal;
        pNode->m_nCacheNum  = nCacheNum;
        pNode->m_nErrorCnt  = nErrorCount;
        pNode->m_ServerStatus = ESS_SVR_ONLINE;
        if (pNode->m_eChangeStatus == EUS_NONE)
        {
            pNode->m_eChangeStatus = EUS_UPDATE;
            m_ArrChangedNode.push(pNode);
        }
    }

    return TRUE;
}

INT32 LogicSvrManager::GetLogicConnID(INT32 nServerID)
{
    LogicServerNode* pNode = GetLogicServerInfo(nServerID);
    if(pNode == NULL)
    {
        return 0;
    }

    return pNode->m_nConnID;
}


LogicServerNode* LogicSvrManager::GetLogicServerInfo(INT32 nServerID)
{
    auto itor = find(nServerID);
    if(itor != end())
    {
        return itor->second;
    }

    return NULL;
}

BOOL LogicSvrManager::OnCloseConnect(INT32 nConnID)
{
    for (auto itor = begin(); itor != end(); itor++)
    {
        LogicServerNode* pNode = itor->second;
        if (pNode->m_nConnID == nConnID)
        {
            pNode->m_ServerStatus = ESS_SVR_OFFLINE;
            if (pNode->m_eChangeStatus == EUS_NONE)
            {
                pNode->m_eChangeStatus = EUS_UPDATE;
            }

            m_ArrChangedNode.push(pNode);
        }
    }

    return TRUE;
}

LogicServerNode* LogicSvrManager::GetSuggestServer(BOOL bReview, INT32 nChannel, UINT32 dwIpaddr)
{
    LogicServerNode* pMaxAvalible = NULL;
    LogicServerNode* pMaxSuggest  = NULL;

    for(auto itor = begin(); itor != end(); itor++)
    {
        LogicServerNode* pNode = itor->second;
        if (pNode->m_ServerStatus != ESS_SVR_ONLINE)
        {
            continue;
        }

        if (bReview && pNode->m_ServerFlag != ESF_REVIEW)
        {
            continue;
        }

        if (!pNode->CheckChannel(nChannel))
        {
            continue;
        }

        if (pNode->m_ServerFlag >= ESF_BUSY && pNode->m_ServerFlag <= ESF_FULL)
        {
            if (pMaxAvalible == NULL)
            {
                pMaxAvalible = pNode;
            }

            continue;
        }

        if (pNode->m_ServerFlag != ESF_GOOD)
        {
            continue;
        }

        if (pMaxAvalible == NULL || pMaxAvalible->m_nServerID < pNode->m_nServerID)
        {
            pMaxAvalible = pNode;
        }

        if (pNode->m_CornerMark != ECM_SURGEST)
        {
            continue;
        }

        if (pMaxSuggest == NULL || pMaxSuggest->m_nServerID < pNode->m_nServerID)
        {
            pMaxSuggest = pNode;
        }
    }

    if (pMaxSuggest != NULL)
    {
        return pMaxSuggest;
    }

    return pMaxAvalible;
}

BOOL LogicSvrManager::ReloadServerList(INT32 nServerID)
{
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_ip");
    INT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_gm_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_user");
    std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_pwd");
    std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_db_name");

    CppMySQL3DB tDBConnection;
    if (!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
    {
        CLog::GetInstancePtr()->LogError("LogicSvrManager::ReloadServerList Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError("LogicSvrManager::ReloadServerList Error: Host:[%s]-User:[%s]-Pwd:[%s]-DBName:[%s]", strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str());
        return FALSE;
    }

    CHAR szSql[SQL_BUFF_LEN] = { 0 };
    if (nServerID == 0)
    {
        snprintf(szSql, SQL_BUFF_LEN, "select * from server_list");
    }
    else
    {
        snprintf(szSql, SQL_BUFF_LEN, "select * from server_list where id = %d", nServerID);
    }

    CppMySQLQuery QueryResult = tDBConnection.querySQL(szSql);
    while(!QueryResult.eof())
    {
        INT32 nSvrID = QueryResult.getIntField("id");
        LogicServerNode* pNode = GetLogicServerInfo(nSvrID);
        if(pNode == NULL)
        {
            pNode = new LogicServerNode();
            pNode->m_nServerID = nSvrID;
            pNode->m_ServerStatus = ESS_SVR_OFFLINE;
            insert(std::make_pair(nSvrID, pNode));
        }
        pNode->m_strSvrName = QueryResult.getStringField("name");
        pNode->m_ServerFlag = QueryResult.getIntField("svr_flag");
        pNode->m_CornerMark = QueryResult.getIntField("corner_mark");
        pNode->m_strOuterAddr = QueryResult.getStringField("outer_ip", "*");
        pNode->m_strInnerAddr = QueryResult.getStringField("inner_ip", "*");
        pNode->m_nPort = QueryResult.getIntField("port", 0);
        pNode->m_uSvrOpenTime = QueryResult.getInt64Field("opentime");
        if (pNode->m_strOuterAddr.empty() || pNode->m_strOuterAddr == "*")
        {
            CLog::GetInstancePtr()->LogError("ReloadServerList Failed, Serverid:%d need a domain name or outer ip address!", nSvrID);
            return FALSE;
        }

        if (pNode->m_strInnerAddr.empty() || pNode->m_strInnerAddr == "*")
        {
            CLog::GetInstancePtr()->LogError("ReloadServerList Failed, Serverid:%d has no inner ip address!", nSvrID);
            return FALSE;
        }

        std::string strMinVersion = QueryResult.getStringField("min_version", "0");
        if(strMinVersion.empty())
        {
            pNode->m_nMinVersion = 0;
        }
        else
        {
            pNode->m_nMinVersion = CommonConvert::VersionToInt(strMinVersion);
        }

        std::string strMaxVersion = QueryResult.getStringField("max_version", "0");
        if (strMaxVersion.empty())
        {
            pNode->m_nMaxVersion = 0;
        }
        else
        {
            pNode->m_nMaxVersion = CommonConvert::VersionToInt(strMaxVersion);
        }

        std::string strCheckChannel = QueryResult.getStringField("check_chan", "*");
        if(strCheckChannel.empty() || strCheckChannel == "*")
        {
            pNode->m_CheckChannelList.clear();
        }
        else
        {
            pNode->m_CheckChannelList.clear();
            std::vector<std::string> vtValue;
            CommonConvert::SpliteString(strCheckChannel, ";", vtValue);
            for(int i = 0; i < vtValue.size(); i++)
            {
                INT32 nTemp = CommonConvert::StringToInt(vtValue[i].c_str());
                if (nTemp > 0)
                {
                    pNode->m_CheckChannelList.insert(nTemp);
                }
            }
        }

        std::string strCheckIp = QueryResult.getStringField("check_ip", "*");
        if(strCheckIp.empty() || strCheckIp == "*")
        {
            pNode->m_CheckIpList.clear();
        }
        else
        {
            pNode->m_CheckIpList.clear();
            std::vector<std::string> vtValue;
            CommonConvert::SpliteString(strCheckIp,  ";", vtValue);
            for(int i = 0; i < vtValue.size(); i++)
            {
                INT32 nTemp = CommonSocket::IpAddrStrToInt(vtValue[i].c_str());
                if (nTemp > 0)
                {
                    pNode->m_CheckIpList.insert(nTemp);
                }
            }
        }

        QueryResult.nextRow();
    }

    return TRUE;
}

BOOL LogicSvrManager::SaveLogicServerThread()
{
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_ip");
    INT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_gm_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_user");
    std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_pwd");
    std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_db_name");

    CppMySQL3DB tDBConnection;
    if (!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
    {
        CLog::GetInstancePtr()->LogError("LogicSvrManager::Init Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError("LogicSvrManager::Init Error: Host:[%s]-User:[%s]-Pwd:[%s]-DBName:[%s]", strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str());
        return FALSE;
    }

    LogicServerNode* pTempNode = NULL;

    CHAR szSql[SQL_BUFF_LEN] = { 0 };

    while (m_IsRun)
    {
        if (m_ArrChangedNode.size() <= 0)
        {
            CommonFunc::Sleep(100);
            continue;
        }

        if (!tDBConnection.ping())
        {
            if (!tDBConnection.reconnect())
            {
                CommonFunc::Sleep(1000);
                continue;
            }
        }

        while (m_ArrChangedNode.pop(pTempNode) && (pTempNode != NULL))
        {
            if (pTempNode->m_eChangeStatus == EUS_NONE)
            {
                continue;
            }

            if (pTempNode->m_eChangeStatus == EUS_NEW_REG)
            {
                snprintf(szSql, SQL_BUFF_LEN, "insert into server_list(id, name, outer_ip,inner_ip, port,http_port,svr_flag, corner_mark,opentime,min_version, max_version, check_chan, check_ip) values(%d, '%s', '%s','%s', %d, %d, %d, %d, %lld,'%s','%s','%s','%s');",
                         pTempNode->m_nServerID, pTempNode->m_strSvrName.c_str(), "127.0.0.1", pTempNode->m_strInnerAddr.c_str(), pTempNode->m_nPort, pTempNode->m_nHttpPort, pTempNode->m_ServerFlag, pTempNode->m_CornerMark, pTempNode->m_uSvrOpenTime, "1.0.0", "9.0.0", "*", "*");
                if (tDBConnection.execSQL(szSql) < 0)
                {
                    CLog::GetInstancePtr()->LogError("LogicSvrManager::SaveLogicServerInfo Error :%s", tDBConnection.GetErrorMsg());
                    return FALSE;
                }

                memset(szSql, 0, SQL_BUFF_LEN);
                snprintf(szSql, SQL_BUFF_LEN, "replace into server_status(id, name, curr_online, max_online,total_cnt,cache_cnt,dberr_cnt,update_time,status, file_version) values(%d, '%s', %d, %d, %d, %d, %d, unix_timestamp(now()), %d,%d);",
                         pTempNode->m_nServerID, pTempNode->m_strSvrName.c_str(), pTempNode->m_nCurOnline, pTempNode->m_nMaxOnline, pTempNode->m_nTotalNum, pTempNode->m_nCacheNum, pTempNode->m_nErrorCnt, pTempNode->m_ServerStatus, 0);
                if (tDBConnection.execSQL(szSql) < 0)
                {
                    CLog::GetInstancePtr()->LogError("LogicSvrManager::SaveLogicServerInfo Error :%s", tDBConnection.GetErrorMsg());
                }
            }

            if (pTempNode->m_eChangeStatus == EUS_RE_REG)
            {
                snprintf(szSql, SQL_BUFF_LEN, "update server_list set name = '%s', port = %d ,http_port = %d, inner_ip ='%s' where id = %d;",
                         pTempNode->m_strSvrName.c_str(), pTempNode->m_nPort, pTempNode->m_nHttpPort, pTempNode->m_strInnerAddr.c_str(),  pTempNode->m_nServerID);
                if (tDBConnection.execSQL(szSql) < 0)
                {
                    CLog::GetInstancePtr()->LogError("LogicSvrManager::RegisterLogicServer Error :%s", tDBConnection.GetErrorMsg());
                }

                memset(szSql, 0, SQL_BUFF_LEN);
                snprintf(szSql, SQL_BUFF_LEN, "replace into server_status(id, name, curr_online, max_online,total_cnt,cache_cnt,dberr_cnt,update_time,status, file_version) values(%d, '%s', %d, %d, %d, %d, %d, unix_timestamp(now()), %d,%d);",
                         pTempNode->m_nServerID, pTempNode->m_strSvrName.c_str(), pTempNode->m_nCurOnline, pTempNode->m_nMaxOnline, pTempNode->m_nTotalNum, pTempNode->m_nCacheNum, pTempNode->m_nErrorCnt, pTempNode->m_ServerStatus, 0);
                if (tDBConnection.execSQL(szSql) < 0)
                {
                    CLog::GetInstancePtr()->LogError("LogicSvrManager::RegisterLogicServer Error :%s", tDBConnection.GetErrorMsg());
                }
            }

            if (pTempNode->m_eChangeStatus == EUS_UPDATE)
            {
                snprintf(szSql, SQL_BUFF_LEN, "replace into server_status(id, name, curr_online, max_online,total_cnt,cache_cnt,dberr_cnt, update_time,status, file_version) values(%d, '%s', %d, %d, %d, %d, %d, unix_timestamp(now()), %d,%d);",
                         pTempNode->m_nServerID, pTempNode->m_strSvrName.c_str(), pTempNode->m_nCurOnline, pTempNode->m_nMaxOnline, pTempNode->m_nTotalNum, pTempNode->m_nCacheNum, pTempNode->m_nErrorCnt, pTempNode->m_ServerStatus, 0);
                if (tDBConnection.execSQL(szSql) < 0)
                {
                    CLog::GetInstancePtr()->LogError("LogicSvrManager::RegisterLogicServer Error :%s", tDBConnection.GetErrorMsg());
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

    if (m_CheckIpList.find(dwIPaddr) != m_CheckIpList.end())
    {
        return TRUE;
    }

    return FALSE;
}

BOOL LogicServerNode::CheckChannel( INT32 nChannel )
{
    if(m_CheckChannelList.size() <= 0)
    {
        return TRUE;
    }

    if (m_CheckChannelList.find(nChannel) != m_CheckChannelList.end())
    {
        return TRUE;
    }

    return FALSE;
}

BOOL LogicServerNode::CheckVersion( std::string strVersion )
{
    if(m_nMinVersion == 0)
    {
        return TRUE;
    }

    INT32 nVersion = CommonConvert::VersionToInt(strVersion);
    if(nVersion == 0)
    {
        return FALSE;
    }

    if ((nVersion <= m_nMinVersion) && (nVersion >= m_nMaxVersion))
    {
        return FALSE;
    }

    return TRUE;
}
