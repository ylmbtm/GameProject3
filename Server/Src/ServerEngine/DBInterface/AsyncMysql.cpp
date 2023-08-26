#include "stdafx.h"
#include "AsyncMysql.h"
#include <stdlib.h>
#include <errmsg.h>

AsyncMySQLResult::AsyncMySQLResult()
{
    m_nCurRowIndex = 0;
}

AsyncMySQLResult::AsyncMySQLResult(CppMySQLQuery& rQuery)
{
}

AsyncMySQLResult::~AsyncMySQLResult()
{
}

INT32 AsyncMySQLResult::GetRowCount()
{
    return (INT32)m_vtResultData.size();
}

INT32 AsyncMySQLResult::GetFieldCount()
{
    return (INT32)m_mapFieldName.size();
}

INT32 AsyncMySQLResult::GetFieldIndex(std::string strFieldName)
{
    auto itor = m_mapFieldName.find(strFieldName);
    if (itor != m_mapFieldName.end())
    {
        return itor->second;
    }

    return -1;
}

INT32 AsyncMySQLResult::GetIntField(const char* szField, int nNullValue)
{
    if ( NULL == szField )
    {
        return nNullValue;
    }

    INT32 nFieldIndex = GetFieldIndex(szField);

    if (nFieldIndex < 0)
    {
        return nNullValue;
    }

    return GetIntField(nFieldIndex, nNullValue);
}

int AsyncMySQLResult::GetIntField(int nField, int nNullValue/*=0*/)
{
    if (m_nCurRowIndex >= m_vtResultData.size() || m_nCurRowIndex < 0)
    {
        return nNullValue;
    }

    if (nField >= m_mapFieldName.size() || nField < 0)
    {
        return nNullValue;
    }

    return atoi(m_vtResultData.at(m_nCurRowIndex).at(nField).c_str());
}

const char* AsyncMySQLResult::GetStringField(int nField, const char* szNullValue/*=""*/)
{
    if (m_nCurRowIndex >= m_vtResultData.size() || m_nCurRowIndex < 0)
    {
        return szNullValue;
    }

    if (nField >= m_mapFieldName.size() || nField < 0)
    {
        return szNullValue;
    }

    return m_vtResultData.at(m_nCurRowIndex).at(nField).c_str();
}

const char* AsyncMySQLResult::GetStringField(const char* szField, const char* szNullValue/*=""*/)
{
    INT32 nFieldIndex = GetFieldIndex(szField);

    if (nFieldIndex < 0)
    {
        return szNullValue;
    }

    return GetStringField(nFieldIndex, szNullValue);
}

INT64 AsyncMySQLResult::GetInt64Field(int nField, INT64 nNullValue /*= 0*/)
{
    if (m_nCurRowIndex >= m_vtResultData.size() || m_nCurRowIndex < 0)
    {
        return nNullValue;
    }

    if (nField >= m_mapFieldName.size() || nField < 0)
    {
        return nNullValue;
    }

#ifdef WIN32
    return _atoi64(m_vtResultData.at(m_nCurRowIndex).at(nField).c_str());
#else
    return atoll(m_vtResultData.at(m_nCurRowIndex).at(nField).c_str());
#endif
}

INT64 AsyncMySQLResult::GetInt64Field(const char* szField, INT64 nNullValue /*= 0*/)
{
    if (NULL == szField)
    {
        return nNullValue;
    }

    INT32 nFieldIndex = GetFieldIndex(szField);
    if (nFieldIndex < 0)
    {
        return nNullValue;
    }

    return GetInt64Field(nFieldIndex, nNullValue);
}

double AsyncMySQLResult::GetFloatField(int nField, double fNullValue/*=0.0*/)
{
    const char* field = GetStringField(nField);
    if ( NULL == field )
    {
        return fNullValue;
    }

    return atof(field);
}

double AsyncMySQLResult::GetFloatField(const char* szField, double fNullValue/*=0.0*/)
{
    const char* field = GetStringField(szField);
    if ( NULL == field )
    {
        return fNullValue;
    }

    return atof(field);
}

void AsyncMySQLResult::NextRow()
{
    m_nCurRowIndex++;
}



const char* AsyncMySQLResult::GetBlobField(int nField, int& nLen)
{
    nLen = 0;

    if (m_nCurRowIndex >= m_vtResultData.size() || m_nCurRowIndex < 0)
    {
        return NULL;
    }

    if (nField >= m_mapFieldName.size() || nField < 0)
    {
        return NULL;
    }

    nLen = (INT32)m_vtResultData.at(m_nCurRowIndex).at(nField).size();

    return m_vtResultData.at(m_nCurRowIndex).at(nField).c_str();
}

const char* AsyncMySQLResult::GetBlobField(const char* szField, int& nLen)
{
    nLen = 0;

    if (m_nCurRowIndex >= m_vtResultData.size() || m_nCurRowIndex < 0)
    {
        return NULL;
    }

    INT32 nFieldIndex = GetFieldIndex(szField);
    if (nFieldIndex < 0)
    {
        return NULL;
    }

    if (nFieldIndex >= m_mapFieldName.size())
    {
        return NULL;
    }

    nLen = (INT32)m_vtResultData.at(m_nCurRowIndex).at(nFieldIndex).size();

    return m_vtResultData.at(m_nCurRowIndex).at(nFieldIndex).c_str();
}

BOOL AsyncMySQLResult::Eof()
{
    if (m_nCurRowIndex >= m_vtResultData.size() || m_nCurRowIndex < 0)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL AsyncMySQLResult::TransFromQuery(CppMySQLQuery& query)
{
    m_vtResultData.clear();
    m_mapFieldName.clear();

    m_vtResultData.reserve(query.numRow());

    INT32 nColNum = query.numFields();

    for (int i = 0; i < nColNum; i++)
    {
        std::string strFieldName = query.fieldName(i);
        m_mapFieldName.insert(std::make_pair(strFieldName, i));
    }

    INT32 m_nCurRowIndex = 0;

    while (!query.eof())
    {
        m_vtResultData.at(m_nCurRowIndex).reserve(nColNum);
        for (int i = 0; i < nColNum; i++)
        {
            m_vtResultData.at(m_nCurRowIndex).push_back(query.getStringField(i));
        }

        m_nCurRowIndex++;
        query.nextRow();
    }

    m_nCurRowIndex = 0;

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
AsyncMySQLDB::AsyncMySQLDB(void)
{
    m_IsRun = FALSE;
}

AsyncMySQLDB::~AsyncMySQLDB(void)
{
    m_IsRun = FALSE;
}

AsyncMySQLDB* AsyncMySQLDB::GetInstancePtr()
{
    static AsyncMySQLDB _AsyncMySQLDB;

    return &_AsyncMySQLDB;
}

BOOL AsyncMySQLDB::Init(const char* host, const char* user, const char* passwd, const char* db, unsigned int port, const char* charSetName)
{
    m_strHost = host;
    m_strUser = user;
    m_strPwd = passwd;
    m_strDB = db;
    m_nPort = port;
    m_strCharSet = charSetName;

    m_IsRun = TRUE;

    m_pThread = new std::thread(&AsyncMySQLDB::ProcessAsyncSqlThread, this);

    ERROR_RETURN_FALSE(m_pThread != NULL);

    return TRUE;
}

BOOL AsyncMySQLDB::Uninit()
{
    m_IsRun = FALSE;

    if (m_pThread != NULL)
    {
        m_pThread->join();

        delete m_pThread;

        m_pThread = NULL;
    }

    return TRUE;
}


BOOL AsyncMySQLDB::ProcessAsyncSqlThread()
{
    CppMySQL3DB         tDBConnection;
    if (!tDBConnection.open(m_strHost.c_str(), m_strUser.c_str(), m_strPwd.c_str(), m_strDB.c_str(), m_nPort, m_strCharSet.c_str()))
    {
        CLog::GetInstancePtr()->LogError("ProceeAsyncSqlThread Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError("ProceeAsyncSqlThread Error: Host:[%s]-User:[%s]-Pwd:[%s]-DBName:[%s]", m_strHost.c_str(), m_strUser.c_str(), m_strPwd.c_str(), m_strDB.c_str());
        return FALSE;
    }

    AsyncQueryParam* pTmpNode;

    while (m_IsRun)
    {
        if (m_ArrWaitNode.size() <= 0)
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

        while (m_ArrWaitNode.pop(pTmpNode))
        {
            if (pTmpNode == NULL)
            {
                continue;
            }

            if (pTmpNode->m_eSqlType == EST_querySQL)
            {
                CppMySQLQuery queryResult = tDBConnection.querySQL(pTmpNode->m_strSql.c_str());
                if (tDBConnection.GetErrorNo() != 0)
                {
                    pTmpNode->m_nQueryError = tDBConnection.GetErrorNo();
                }
                else
                {
                    pTmpNode->m_SqlResult.TransFromQuery(queryResult);
                }

                m_ArrFinishNode.push(pTmpNode);
            }
            else  if (pTmpNode->m_eSqlType == EST_execSQL)
            {
                INT32 nRows = tDBConnection.execSQL(pTmpNode->m_strSql.c_str());
                if (nRows <= 0)
                {
                    pTmpNode->m_nQueryError = tDBConnection.GetErrorNo();
                }
            }
            else
            {
                CLog::GetInstancePtr()->LogError("ProceeAsyncSqlThread Error: Invalid SqlType!!");
                continue;
            }
        }
    }
    return TRUE;
}

BOOL AsyncMySQLDB::Update()
{
    if (m_ArrFinishNode.size() <= 0)
    {
        return TRUE;
    }

    AsyncQueryParam* pTmpNode;
    while (m_ArrFinishNode.pop(pTmpNode))
    {
        if (pTmpNode == NULL)
        {
            continue;
        }

        if (pTmpNode->m_pCallBack != NULL)
        {
            pTmpNode->m_pCallBack(&pTmpNode->m_SqlResult, pTmpNode->m_nUserData);
        }

        delete pTmpNode;
    }

    return TRUE;
}

BOOL AsyncMySQLDB::AsyncQuerySql(std::string strSql, QueryCallBack pCallBack, INT64 nUserData)
{
    if (strSql.empty())
    {
        return FALSE;
    }

    AsyncQueryParam* pAsyncParam = new AsyncQueryParam(strSql, EST_querySQL, pCallBack, nUserData);

    m_ArrWaitNode.push(pAsyncParam);

    return TRUE;
}

BOOL AsyncMySQLDB::AsyuExecSQL(std::string strSql, QueryCallBack pCallBack, INT64 nUesrData)
{
    if (strSql.empty() || pCallBack == NULL)
    {
        return FALSE;
    }

    AsyncQueryParam* pAsyncParam = new AsyncQueryParam(strSql, EST_execSQL, pCallBack, nUesrData);

    m_ArrWaitNode.push(pAsyncParam);

    return TRUE;
}
