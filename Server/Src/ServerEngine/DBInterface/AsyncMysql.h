#ifndef __MYSQL_ASYNC_QUERY_H__
#define __MYSQL_ASYNC_QUERY_H__

#include "CppMysql.h"
#include "../LockFreeQueue.h"

class AsyncQueryParam;
class AsyncMySQLResult;
enum DBSqlType
{
    EST_querySQL,
    EST_execSQL,
};

typedef std::function<void(const  AsyncMySQLResult*, INT64)> QueryCallBack;

void DefaultExecCallBack(const AsyncMySQLResult* pSqlResult, INT64 nUesrData)
{
    return;
}

class AsyncMySQLResult
{
    friend class AsyncMySQLDB;
public:
    AsyncMySQLResult();

    AsyncMySQLResult(CppMySQLQuery& rQuery);

    virtual ~AsyncMySQLResult();

    INT32 GetRowCount();

    INT32 GetFieldCount();

    INT32 GetFieldIndex(std::string strFieldName);

    INT32 GetIntField(int nField, int nNullValue = 0);
    INT32 GetIntField(const char* szField, int nNullValue = 0);

    INT64 GetInt64Field(int nField, INT64 nNullValue = 0);
    INT64 GetInt64Field(const char* szField, INT64 nNullValue = 0);

    double GetFloatField(int nField, double fNullValue = 0.0);
    double GetFloatField(const char* szField, double fNullValue = 0.0);

    const char* GetStringField(int nField, const char* szNullValue = "");
    const char* GetStringField(const char* szField, const char* szNullValue = "");

    const char* GetBlobField(int nField, int& nLen);
    const char* GetBlobField(const char* szField, int& nLen);

    BOOL Eof();

    VOID NextRow();

    BOOL TransFromQuery(CppMySQLQuery& query);

private:
    std::map<std::string, INT32> m_mapFieldName;

    std::vector<std::vector<std::string>> m_vtResultData;

    INT32   m_nCurRowIndex;
};

class AsyncQueryParam
{
public:
    AsyncQueryParam()
    {

    }
    AsyncQueryParam(std::string strSql, DBSqlType eSqlType, QueryCallBack callBack, INT64 nUserData)
    {
        m_strSql = strSql;
        m_pCallBack = callBack;
        m_nUserData = nUserData;
        m_eSqlType = eSqlType;
    }

    ~AsyncQueryParam()
    {
    }


public:
    DBSqlType m_eSqlType;
    INT64 m_nUserData;
    std::string m_strSql;
    INT32 m_nQueryError = 0;
    QueryCallBack m_pCallBack;
    AsyncMySQLResult m_SqlResult;
};

class AsyncMySQLDB
{
public:
    AsyncMySQLDB(void);
    ~AsyncMySQLDB(void);

public:
    static AsyncMySQLDB* GetInstancePtr();

public:
    BOOL    Init(const char* host, const char* user, const char* passwd, const char* db, unsigned int port, const char* charSetName = "utf8mb4");

    BOOL    Uninit();

    BOOL    ProcessAsyncSqlThread();

    BOOL    Update();

    BOOL    AsyncQuerySql(std::string strSql, QueryCallBack fun, INT64 nUesrData = 0);

    BOOL    AsyuExecSQL(std::string strSql, QueryCallBack fun = DefaultExecCallBack, INT64 nUesrData = 0);

public:
    BOOL                m_IsRun;

    std::thread*        m_pThread;

    ArrayLockFreeQueue<AsyncQueryParam*, 1024>  m_ArrWaitNode;
    ArrayLockFreeQueue<AsyncQueryParam*, 1024>  m_ArrFinishNode;

    std::string  m_strHost;
    std::string  m_strUser;
    std::string  m_strPwd;
    std::string  m_strDB;
    std::string  m_strCharSet; //字符集
    int          m_nPort;

};

#endif //__MYSQL_ASYNC_QUERY_H__