
#ifndef _SQL_CONN_H_
#define _SQL_CONN_H_

#include <my_global.h>
#include <mysql.h>
#include <cstdio>

#ifdef _DEBUG
	#pragma comment(lib, "libmysql.lib")
#else
	#pragma comment(lib, "libmysql.lib")
#endif

#define ERROR_SLEEP_TIME 3000 // 执行SQL语句时出错，再次尝试执行间隔时间（毫秒）

class CDBStoredProcedure;
class sql_result;

class CDBConnection
{
public:
    CDBConnection( void );
    ~CDBConnection( void );

	BOOL Init();

	BOOL Uninit();

    BOOL Connect(char const *szHost, char const *szUser,char const *szPwd,char const *szDb,int nPort);

    void Close( void );

    BOOL Execute( CDBStoredProcedure *pDBStoredProcedure);

    BOOL Query( CDBStoredProcedure *pDBStoredProcedure);

    int GetError( void ) const;

protected:
    bool Reconnect( void );

    bool ProcError( char const *op_ = NULL, char const *func_ = NULL );

private:
    MYSQL       *m_pMySql;
	std::string  m_strHost;
    std::string  m_strUser;
	std::string  m_strPwd;
    std::string  m_strDB;
    int          m_nPort;
    int          m_nErrno;
    std::string  m_strError;
};

#endif // _SQL_CONN_H_