#include "stdafx.h"
#include "DBConnection.h"
#include "DBStoredProc.h"
#include "../CommonFunc.h"
#include "../Log.h"
#include <errmsg.h>

CDBConnection::CDBConnection( void )
	: m_pMySql( NULL ),
	  m_nPort( 3306 ),
	  m_nErrno( 0 ),
	  m_strError( "" )
{
}

CDBConnection::~CDBConnection( void )
{
	Close();
}

BOOL CDBConnection::Init()
{
	mysql_library_init( 0, NULL, NULL );
	mysql_thread_init();

	return true;
}

BOOL CDBConnection::Uninit()
{
	mysql_thread_end();
	mysql_library_end();
	return true;
}

// connect.
BOOL CDBConnection::Connect(char const* szHost, char const* szUser, char const* szPwd, char const* szDb, int nPort)
{
	Close();

	m_pMySql = mysql_init( NULL );

	if ( NULL != m_pMySql )
	{
		if ( NULL != mysql_real_connect( m_pMySql, szHost, szUser, szPwd, szDb, nPort, NULL, 0 ) )
		{
			m_strHost.assign( szHost );
			m_strUser.assign( szUser );
			m_strPwd.assign( szPwd );
			m_strDB.assign( szDb );
			m_nPort = nPort;

			mysql_autocommit( m_pMySql, 1 );
			return true;
		}
		else
		{
			m_nErrno = mysql_errno( m_pMySql );
			m_strError = mysql_error( m_pMySql );
			CLog::GetInstancePtr()->LogError("CDBConnection::Connect Failed [mysql_real_connect], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
			return false;
		}
	}

	return false;
}

// reconnect.
bool CDBConnection::Reconnect( void )
{
	if ( NULL != m_pMySql && 0 == mysql_ping( m_pMySql ) )
	{
		return true;
	}
	else
	{
		Close();
		m_pMySql = mysql_init( NULL );
		if ( NULL != m_pMySql )
		{
			if ( NULL != mysql_real_connect( m_pMySql, m_strHost.c_str(), m_strUser.c_str(), m_strPwd.c_str(), m_strDB.c_str(), m_nPort, NULL, 0 ) )
			{
				mysql_autocommit( m_pMySql, 1 );
				return true;
			}
			else
			{
				m_nErrno = mysql_errno( m_pMySql );
				m_strError = mysql_error( m_pMySql );
				CLog::GetInstancePtr()->LogError("CDBConnection::Connect Failed [mysql_real_connect], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
				return false;
			}
		}
	}

	return false;
}

// close.
void CDBConnection::Close( void )
{
	if ( NULL != m_pMySql )
	{
		mysql_close( m_pMySql );

		m_pMySql = NULL;
	}
}

BOOL CDBConnection::Execute(CDBStoredProcedure* pDBStoredProcedure)
{
	if((NULL == m_pMySql) || (pDBStoredProcedure == NULL))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	MYSQL_STMT* pMySqlStmt = mysql_stmt_init(m_pMySql);
	if(pMySqlStmt == NULL)
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );
		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_init], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
		return FALSE;
	}

	if(0 != mysql_stmt_prepare(pMySqlStmt, pDBStoredProcedure->m_strSql.c_str(), (unsigned long)pDBStoredProcedure->m_strSql.size()))
	{
		m_nErrno = mysql_errno( m_pMySql );

		m_strError = mysql_error( m_pMySql );

		mysql_stmt_close( pMySqlStmt );

		pMySqlStmt = NULL;

		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_prepare], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());

		return FALSE;
	}

	my_bool _bl = 1;
	mysql_stmt_attr_set( pMySqlStmt, STMT_ATTR_UPDATE_MAX_LENGTH, &_bl );

	if(mysql_stmt_param_count( pMySqlStmt ) != pDBStoredProcedure->m_nCount)
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );
		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_param_count], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
		return FALSE;
	}

	if(pDBStoredProcedure->m_pMybind != NULL)
	{
		if ( 0 != mysql_stmt_bind_param( pMySqlStmt, pDBStoredProcedure->m_pMybind ) )
		{
			m_nErrno = mysql_errno( m_pMySql );
			m_strError = mysql_error( m_pMySql );
			mysql_stmt_close( pMySqlStmt );
			pMySqlStmt = NULL;
			ASSERT_FAIELD;
			CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_bind_param], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
			return FALSE;
		}
	}

	if(pMySqlStmt == NULL)
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );
		ASSERT_FAIELD;
		return FALSE;
	}

	if (0 != mysql_stmt_execute( pMySqlStmt ))
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );
		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_execute], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
		ASSERT_FAIELD;
		return FALSE;
	}

	// 检查是否有结果集
	MYSQL_RES* pMySqlResult = mysql_stmt_result_metadata( pMySqlStmt );
	if(pMySqlResult == NULL)
	{
		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		return TRUE;
	}

	unsigned int server_status = m_pMySql->server_status;

	if ( server_status & SERVER_PS_OUT_PARAMS )
	{
		mysql_stmt_store_result( pMySqlStmt );
		pDBStoredProcedure->m_DBRecordSet.InitRecordSet(pMySqlStmt, pMySqlResult);
	}
	else
	{
		//zm :走到这里来，是不应该的， 更新存储过程最多可能带参数，不允许返回结果集
		ASSERT_FAIELD;
		//write_log( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!\n" );
		mysql_stmt_store_result( pMySqlStmt );
		mysql_free_result( pMySqlResult );
		mysql_stmt_free_result( pMySqlStmt );

		while ( 0 == mysql_stmt_next_result( pMySqlStmt ) )
		{
			pMySqlResult = mysql_stmt_result_metadata( pMySqlStmt );
			if ( NULL != pMySqlResult )
			{
				server_status = m_pMySql->server_status;
				mysql_stmt_store_result( pMySqlStmt );
				mysql_free_result( pMySqlResult );
				mysql_stmt_free_result( pMySqlStmt );
				if ( server_status & SERVER_PS_OUT_PARAMS )
				{
					//write_log( "此调用存储存在返回参数值，请检查sql语句和存储过程实现!\n" );
					break;
				}
				else
				{
					// //write_log( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!\n" );
				}
			}
		}
	}

	return TRUE;
}

// query.
BOOL CDBConnection::Query(CDBStoredProcedure* pDBStoredProcedure)
{
	if(pDBStoredProcedure->m_pMybind == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if ( NULL == m_pMySql || NULL == pDBStoredProcedure)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	MYSQL_STMT* pMySqlStmt = mysql_stmt_init( m_pMySql );
	if(NULL == pMySqlStmt)
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );
		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_init], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
		return FALSE;
	}

	if(0 != mysql_stmt_prepare(pMySqlStmt, pDBStoredProcedure->m_strSql.c_str(), (unsigned long)pDBStoredProcedure->m_strSql.size()))
	{
		m_nErrno = mysql_errno( m_pMySql );

		m_strError = mysql_error( m_pMySql );

		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_prepare], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
		return FALSE;
	}

	my_bool _bl = 1;
	mysql_stmt_attr_set( pMySqlStmt, STMT_ATTR_UPDATE_MAX_LENGTH, &_bl );

	if ( 0 != mysql_stmt_bind_param( pMySqlStmt, pDBStoredProcedure->m_pMybind ) )
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );
		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_bind_param], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
		return FALSE;
	}

	if(pMySqlStmt == NULL)
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );

		return FALSE;
	}

	if (0 != mysql_stmt_execute( pMySqlStmt ))
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );
		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_execute], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
		return FALSE;
	}

	// 检查是否有结果集
	MYSQL_RES* pMySqlResult = mysql_stmt_result_metadata( pMySqlStmt );
	if(pMySqlResult == NULL)
	{
		pMySqlResult  = NULL;
		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		return FALSE;
	}

	unsigned int server_status = m_pMySql->server_status;

	if(0 != mysql_stmt_store_result( pMySqlStmt ))
	{
		m_nErrno = mysql_errno( m_pMySql );
		m_strError = mysql_error( m_pMySql );
		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_store_result], ErrorNo:%d, ErrorMsg:%s", m_nErrno, m_strError.c_str());
		return FALSE;
	}

	if((int)mysql_stmt_num_rows(pMySqlStmt) <= 0)
	{
		return FALSE;
	}

	pDBStoredProcedure->m_DBRecordSet.InitRecordSet(pMySqlStmt, pMySqlResult);

	return TRUE;
}

BOOL CDBConnection::Query( std::string sql )
{
	return TRUE;
}

// process error.
bool CDBConnection::ProcError( char const* op_/* = NULL*/, char const* func_/* = NULL*/ )
{
	bool _ret = false;

	if ( NULL != op_ && NULL != func_ )
	{
		//     //write_log( "op = %s, func = %s.\n", op_, func_ );
	}
	else if ( NULL != op_ )
	{
		//     //write_log( "op = %s.\n", op_ );
	}

	switch ( m_nErrno )
	{
		case CR_SERVER_GONE_ERROR:
		{
			if ( Reconnect() )
			{
				_ret = true;
			}
			else
			{
				if ( NULL != m_pMySql )
				{
					m_nErrno = mysql_errno( m_pMySql );
					m_strError = mysql_error( m_pMySql );
					CommonFunc::Sleep( ERROR_SLEEP_TIME );
					_ret = ProcError( op_, __FUNCTION__ );
				}
			}
		}
		break;

		case CR_SERVER_LOST:
		{
			//write_log( "lost the connection to mysql server, errno = %d!\n", m_errno_ );

			if ( Reconnect() )
			{
				_ret = true;
			}
			else
			{
				if ( NULL != m_pMySql )
				{
					m_nErrno = mysql_errno( m_pMySql );
					m_strError = mysql_error( m_pMySql );
					CommonFunc::Sleep( ERROR_SLEEP_TIME );
					_ret = ProcError( op_, __FUNCTION__ );
				}
			}
		}
		break;

		case CR_INVALID_CONN_HANDLE:
		{
			//write_log( "invalid connection handle, errno = %d!\n", m_errno_ );

			if ( Reconnect() )
			{
				_ret = true;
			}
			else
			{
				if ( NULL != m_pMySql )
				{
					m_nErrno = mysql_errno( m_pMySql );
					m_strError = mysql_error( m_pMySql );
					CommonFunc::Sleep( ERROR_SLEEP_TIME );
					_ret = ProcError( op_, __FUNCTION__ );
				}
			}
		}
		break;

		case CR_SERVER_LOST_EXTENDED:
		{
			//write_log( "lost the connection to mysql server, errno = %d!\n", m_errno_ );

			if ( Reconnect() )
			{
				_ret = true;
			}
			else
			{
				if ( NULL != m_pMySql )
				{
					m_nErrno = mysql_errno( m_pMySql );
					m_strError = mysql_error( m_pMySql );
					CommonFunc::Sleep( ERROR_SLEEP_TIME );
					_ret = ProcError( op_, __FUNCTION__ );
				}
			}
		}
		break;

		default:
		{
			//write_log( "%s, errno = %d!\n", m_error_.c_str(), m_errno_ );
		}
		break;
	}

	return _ret;
}

bool CDBConnection::Ping()
{
	if (mysql_ping(m_pMySql) == 0)
	{
		return true;
	}

	return false;
}

// errno.
int CDBConnection::GetError( void ) const
{
	return m_nErrno;
}

BOOL CDBConnection::Execute( std::string sql )
{
	return TRUE;
}

BOOL CDBConnection::SetConnectParam( char const* szHost, char const* szUser, char const* szPwd, char const* szDb, int nPort )
{
	m_strHost.assign( szHost );

	m_strUser.assign( szUser );

	m_strPwd.assign( szPwd );

	m_strDB.assign( szDb );

	m_nPort = nPort;

	return TRUE;
}
