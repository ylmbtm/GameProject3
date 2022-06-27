#include "stdafx.h"
#include "AccountManager.h"
#include <regex>

CAccountObjectMgr::CAccountObjectMgr()
{
    m_IsRun         = FALSE;
    m_pThread       = NULL;
    m_bCrossChannel = FALSE;
    m_u64MaxID      = 0;
}

CAccountObjectMgr::~CAccountObjectMgr()
{
    m_IsRun         = FALSE;
    m_pThread       = NULL;
    m_bCrossChannel = FALSE;
}

BOOL CAccountObjectMgr::LoadCacheAccount()
{
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_ip");
    INT32 nPort         = CConfigFile::GetInstancePtr()->GetIntValue("mysql_acc_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_user");
    std::string strPwd  = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_pwd");
    std::string strDb   = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_db_name");
    m_bCrossChannel     = CConfigFile::GetInstancePtr()->GetIntValue("account_cross_channel");

    CppMySQL3DB tDBConnection;
    if(!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
    {
        CLog::GetInstancePtr()->LogError("LoadCacheAccount Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError("LoadCacheAccount Error: Host:[%s]-User:[%s]-Pwd:[%s]-DBName:[%s]", strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str());
        return FALSE;
    }

    CppMySQLQuery QueryResult = tDBConnection.querySQL("select * from account");
    CAccountObject* pTempObject = NULL;
    while(!QueryResult.eof())
    {
        pTempObject = AddAccountObject(QueryResult.getInt64Field("id"),
                                       QueryResult.getStringField("name"),
                                       QueryResult.getIntField("channel"));

        ERROR_RETURN_FALSE(pTempObject != NULL);

        pTempObject->m_strPassword    = QueryResult.getStringField("password");
        pTempObject->m_uCreateTime    = CommonFunc::DateStringToTime(QueryResult.getStringField("create_time"));
        pTempObject->m_uSealTime      = CommonFunc::DateStringToTime(QueryResult.getStringField("seal_end_time"));
        pTempObject->m_nLastSvrID[0] = QueryResult.getIntField("lastsvrid1");
        pTempObject->m_nLastSvrID[1] = QueryResult.getIntField("lastsvrid2");
        if(m_u64MaxID < (UINT64)QueryResult.getInt64Field("id"))
        {
            m_u64MaxID = (UINT64)QueryResult.getInt64Field("id");
        }

        QueryResult.nextRow();
    }

    tDBConnection.close();

    return TRUE;
}

CAccountObject* CAccountObjectMgr::GetAccountObjectByID( UINT64 AccountID )
{
    return GetByKey(AccountID);
}

CAccountObject* CAccountObjectMgr::CreateAccountObject(const std::string& strName, const std::string& strPwd, INT32 nChannel)
{
    m_u64MaxID += 1;

    CAccountObject* pObj = InsertAlloc(m_u64MaxID);
    ERROR_RETURN_NULL(pObj != NULL);

    pObj->m_strName         = strName;
    pObj->m_strPassword     = strPwd;
    pObj->m_ID              = m_u64MaxID;
    pObj->m_nChannel        = nChannel;
    pObj->m_nLoginCount     = 1;
    pObj->m_uCreateTime     = CommonFunc::GetCurrTime();

    if (m_bCrossChannel)
    {
        m_mapNameObj.insert(std::make_pair(strName, pObj));
    }
    else
    {
        m_mapNameObj.insert(std::make_pair(strName + CommonConvert::IntToString(nChannel), pObj));
    }

    m_ArrChangedAccount.push(pObj);

    return pObj;
}

BOOL CAccountObjectMgr::ReleaseAccountObject(UINT64 AccountID )
{
    return Delete(AccountID);
}

BOOL CAccountObjectMgr::SealAccount(UINT64& uAccountID, const std::string& strName, INT32 nChannel, BOOL bSeal, INT32 nSealTime, INT32& nLastSvrID)
{
    CAccountObject* pAccObj = NULL;
    if (uAccountID == 0)
    {
        pAccObj = GetAccountObject(strName, nChannel);
    }
    else
    {
        pAccObj = GetAccountObjectByID(uAccountID);
    }

    if (pAccObj == NULL)
    {
        CLog::GetInstancePtr()->LogError("CAccountObjectMgr::SealAccount Error Cannot find account uAccountID:%lld, strName:%s", uAccountID, strName.c_str());
        return FALSE;
    }

    if (bSeal)
    {
        pAccObj->m_uSealTime = CommonFunc::GetCurrTime() + nSealTime;
        uAccountID = pAccObj->m_ID;
        nLastSvrID = pAccObj->m_nLastSvrID[0];
    }
    else
    {
        pAccObj->m_uSealTime = 0;
    }

    m_ArrChangedAccount.push(pAccObj);

    return TRUE;
}

BOOL CAccountObjectMgr::SetLastServer(UINT64 uAccountID, INT32 ServerID)
{
    ERROR_RETURN_FALSE(uAccountID != 0);

    CAccountObject* pAccObj = GetAccountObjectByID(uAccountID);
    ERROR_RETURN_FALSE(pAccObj != NULL);

    if (pAccObj->m_nLastSvrID[0] == ServerID)
    {
        pAccObj->m_nLoginCount += 1;
        m_ArrChangedAccount.push(pAccObj);
        return TRUE;
    }

    pAccObj->m_nLastSvrID[1] = pAccObj->m_nLastSvrID[0];
    pAccObj->m_nLastSvrID[0] = ServerID;
    m_ArrChangedAccount.push(pAccObj);

    return TRUE;
}

CAccountObject* CAccountObjectMgr::AddAccountObject(UINT64 uAccountID, const CHAR* pStrName, INT32 nChannel)
{
    CAccountObject* pObj = InsertAlloc(uAccountID);
    ERROR_RETURN_NULL(pObj != NULL);

    pObj->m_strName     = pStrName;
    pObj->m_ID          = uAccountID;
    pObj->m_nChannel   = nChannel;

    if (m_bCrossChannel)
    {
        m_mapNameObj.insert(std::make_pair(pObj->m_strName, pObj));
    }
    else
    {
        m_mapNameObj.insert(std::make_pair(pObj->m_strName + CommonConvert::IntToString(nChannel), pObj));
    }

    return pObj;
}

BOOL CAccountObjectMgr::SaveAccountThread()
{
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_ip");
    INT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_acc_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_user");
    std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_pwd");
    std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_acc_svr_db_name");

    CppMySQL3DB tDBConnection;
    if (!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
    {
        CLog::GetInstancePtr()->LogError("SaveAccountChange Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError("SaveAccountChange Error: Host:[%s]-User:[%s]-Pwd:[%s]-DBName:[%s]", strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str());
        return FALSE;
    }

    while(TRUE)
    {
        CAccountObject* pAccount = NULL;

        CHAR szSql[SQL_BUFF_LEN] = { 0 };

        if (m_ArrChangedAccount.size() <= 0)
        {
            if (!IsRun())
            {
                return TRUE;
            }

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

        while (m_ArrChangedAccount.pop(pAccount) && (pAccount != NULL))
        {
            snprintf(szSql, SQL_BUFF_LEN, "replace into account(id, name, password, lastsvrid1, lastsvrid2, channel, create_time, seal_end_time, logincount) values('%lld','%s','%s','%d','%d', '%d', '%s','%s',%d)",
                     pAccount->m_ID, pAccount->m_strName.c_str(), pAccount->m_strPassword.c_str(), pAccount->m_nLastSvrID[0], pAccount->m_nLastSvrID[1], pAccount->m_nChannel, CommonFunc::TimeToString(pAccount->m_uCreateTime).c_str(), CommonFunc::TimeToString(pAccount->m_uSealTime).c_str(), pAccount->m_nLoginCount);

            if (tDBConnection.execSQL(szSql) > 0)
            {
                continue;
            }

            CLog::GetInstancePtr()->LogError("CAccountMsgHandler::SaveAccountChange Failed! Reason: %s", tDBConnection.GetErrorMsg());
            break;
        }
    }

    return TRUE;
}

BOOL CAccountObjectMgr::Init()
{
    ERROR_RETURN_FALSE(LoadCacheAccount());

    m_IsRun = TRUE;

    m_pThread = new std::thread(&CAccountObjectMgr::SaveAccountThread, this);

    ERROR_RETURN_FALSE(m_pThread != NULL);

    return TRUE;
}

BOOL CAccountObjectMgr::Uninit()
{
    m_IsRun = FALSE;

    if (m_pThread != NULL)
    {
        m_pThread->join();

        delete m_pThread;

        m_pThread = NULL;
    }

    m_mapNameObj.clear();

    Clear();

    return TRUE;
}

BOOL CAccountObjectMgr::IsRun()
{
    return m_IsRun;
}

BOOL CAccountObjectMgr::CheckAccountName(const std::string& strName, bool bFromChannel)
{
    if (strName.size() < 1)
    {
        return FALSE;
    }

    if (CommonConvert::HasSymbol(strName.c_str(), (const char*)"\'\" \\\r\n%%"))
    {
        return FALSE;
    }

    if (bFromChannel)
    {
        return TRUE;
    }

    if (!std::regex_match(strName.c_str(), std::regex("([a-zA-Z0-9]+)")))
    {
        return FALSE;
    }

    return TRUE;
}

CAccountObject* CAccountObjectMgr::GetAccountObject(const std::string& name, INT32 nChannel )
{
    if (m_bCrossChannel)
    {
        auto itor = m_mapNameObj.find(name);
        if (itor != m_mapNameObj.end())
        {
            return itor->second;
        }
    }
    else
    {
        auto itor = m_mapNameObj.find(name + CommonConvert::IntToString(nChannel));
        if (itor != m_mapNameObj.end())
        {
            return itor->second;
        }
    }

    return NULL;
}
