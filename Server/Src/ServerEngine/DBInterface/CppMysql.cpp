#include "stdafx.h"
#include <winsock2.h>
#include "CppMysql.h"
#include <stdlib.h>
#include <Strsafe.h>

CppMySQLQuery::CppMySQLQuery()
{
	m_MysqlRes = NULL;
	_field = NULL;
	_row = NULL;
	_row_count = 0;
	_field_count = 0;
}

CppMySQLQuery::CppMySQLQuery(CppMySQLQuery& rQuery)
{
	*this = rQuery;
}

CppMySQLQuery& CppMySQLQuery::operator=(CppMySQLQuery& rQuery)
{
	if (this == &rQuery)
	{
		return *this;
	}

	m_MysqlRes = rQuery.m_MysqlRes;
	_row = NULL;
	_row_count = 0;
	_field_count = 0;
	_field = NULL;
	if ( m_MysqlRes != NULL )
	{
		//定位游标位置到第一个位置
		mysql_data_seek(m_MysqlRes, 0);
		_row =  mysql_fetch_row( m_MysqlRes );
		_row_count = mysql_num_rows( m_MysqlRes );
		//得到字段数量
		_field_count = mysql_num_fields( m_MysqlRes );
	}
	rQuery.m_MysqlRes = NULL;
	rQuery._field = NULL;
	rQuery._row = NULL;
	rQuery._row_count = 0;
	rQuery._field_count = 0;
	return *this;
}

CppMySQLQuery::~CppMySQLQuery()
{
	freeRes();
}

void CppMySQLQuery::freeRes()
{
	if ( m_MysqlRes != NULL )
	{
		mysql_free_result(m_MysqlRes);
		m_MysqlRes = NULL;
	}
}

int CppMySQLQuery::numRow()
{
	return _row_count;
}

int CppMySQLQuery::numFields()
{
	return _field_count;
}

int CppMySQLQuery::seekRow(int offerset)
{
	if (offerset < 0)
	{
		offerset = 0;
	}

	if (offerset >= _row_count)
	{
		offerset = _row_count - 1;
	}

	mysql_data_seek(m_MysqlRes, offerset);

	_row = mysql_fetch_row(m_MysqlRes);

	return offerset;
}

int CppMySQLQuery::fieldIndex(const char* szField)
{
	if ( NULL == m_MysqlRes )
	{
		return -1;
	}

	if ( NULL == szField )
	{
		return -1;
	}

	mysql_field_seek(m_MysqlRes, 0);//定位到第0列
	u_int i = 0;
	while ( i < _field_count )
	{
		_field = mysql_fetch_field( m_MysqlRes );
		if ( _field != NULL && strcmp(_field->name, szField) == 0 )//找到
		{
			return i;
		}

		i++;
	}

	return -1;
}

const char* CppMySQLQuery::fieldName(int nCol)
{
	if ( m_MysqlRes == NULL )
	{
		return NULL;
	}

	mysql_field_seek(m_MysqlRes, nCol);

	_field = mysql_fetch_field(m_MysqlRes);
	if ( _field != NULL )
	{
		return _field->name;
	}

	return  NULL;
}

int CppMySQLQuery::getIntField(int nField, int nNullValue/*=0*/)
{
	if ( NULL == m_MysqlRes )
	{
		return nNullValue;
	}

	if ( nField + 1 > (int)_field_count )
	{
		return nNullValue;
	}

	if ( NULL == _row )
	{
		return nNullValue;
	}

	return atoi(_row[nField]);
}

int CppMySQLQuery::getIntField(const char* szField, int nNullValue/*=0*/)
{
	if ( NULL == m_MysqlRes || NULL == szField )
	{
		return nNullValue;
	}

	if ( NULL == _row )
	{
		return nNullValue;
	}

	const char* filed = getStringField(szField);
	if ( NULL == filed )
	{
		return nNullValue;
	}

	return atoi(filed);
}

const char* CppMySQLQuery::getStringField(int nField, const char* szNullValue/*=""*/)
{
	if ( NULL == m_MysqlRes )
	{
		return szNullValue;
	}

	if ( nField + 1 > (int)_field_count )
	{
		return szNullValue;
	}

	if ( NULL == _row )
	{
		return szNullValue;
	}

	if(_row[nField] == NULL)
	{
		return szNullValue;
	}

	return _row[nField];
}

const char* CppMySQLQuery::getStringField(const char* szField, const char* szNullValue/*=""*/)
{
	if ( NULL == m_MysqlRes )
	{
		return szNullValue;
	}

	int nField = fieldIndex(szField);
	if ( nField == -1 )
	{
		return szNullValue;
	}

	return getStringField(nField, szNullValue);
}

INT64 CppMySQLQuery::getInt64Field(int nField, INT64 nNullValue /*= 0*/)
{
	if ( NULL == m_MysqlRes )
	{
		return nNullValue;
	}

	if ( nField + 1 > (int)_field_count )
	{
		return nNullValue;
	}

	if ( NULL == _row )
	{
		return nNullValue;
	}

	return _atoi64(_row[nField]);
}

INT64 CppMySQLQuery::getInt64Field(const char* szField, INT64 nNullValue /*= 0*/)
{
	if ( NULL == m_MysqlRes || NULL == szField )
	{
		return nNullValue;
	}

	if ( NULL == _row )
	{
		return nNullValue;
	}

	const char* filed = getStringField(szField);
	if ( NULL == filed )
	{
		return nNullValue;
	}

	return _atoi64(filed);
}

double CppMySQLQuery::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	const char* field = getStringField(nField);
	if ( NULL == field )
	{
		return fNullValue;
	}

	return atof(field);
}

double CppMySQLQuery::getFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	const char* field = getStringField(szField);
	if ( NULL == field )
	{
		return fNullValue;
	}

	return atof(field);
}

void CppMySQLQuery::nextRow()
{
	if ( NULL == m_MysqlRes )
	{
		return;
	}

	_row = mysql_fetch_row(m_MysqlRes);
}

bool CppMySQLQuery::eof()
{
	return (_row == NULL);
}

CppMySQL3DB::CppMySQL3DB()
{
	_db_ptr = NULL;
}

CppMySQL3DB::~CppMySQL3DB()
{
	if ( _db_ptr != NULL )
	{
		close();
	}
}

bool CppMySQL3DB::open(const char* host, const char* user, const char* passwd, const char* db,
                       unsigned int port /*= 0*/, unsigned long client_flag /*= 0*/)
{
	bool ret = false;
	_db_ptr = mysql_init(NULL);
	if( NULL == _db_ptr )
	{
		goto EXT;
	}

	//如果连接失败，返回NULL。对于成功的连接，返回值与第1个参数的值相同。
	if ( NULL == mysql_real_connect( _db_ptr, host, user, passwd, db, port, NULL, client_flag) )
	{
		goto EXT;
	}
	//选择制定的数据库失败
	//0表示成功，非0值表示出现错误。
	if ( mysql_select_db( _db_ptr, db ) != 0 )
	{
		mysql_close(_db_ptr);
		_db_ptr = NULL;
		goto EXT;
	}
	ret = true;
EXT:
	//初始化mysql结构失败
	if ( ret == false && _db_ptr != NULL )
	{
		mysql_close( _db_ptr );
		_db_ptr = NULL;
	}
	return ret;
}

void CppMySQL3DB::close()
{
	if ( _db_ptr != NULL )
	{
		mysql_close( _db_ptr );
		_db_ptr = NULL;
	}
}

MYSQL* CppMySQL3DB::getMysql()
{
	return _db_ptr;
}

/* 处理返回多行的查询，返回影响的行数 */
CppMySQLQuery& CppMySQL3DB::querySQL(const char* sql)
{
	if ( !mysql_real_query( _db_ptr, sql, strlen(sql) ) )
	{
		_db_query.m_MysqlRes = mysql_store_result( _db_ptr );
		//   _db_query._row =  mysql_fetch_row( _db_query._mysql_res );
		//   _db_query._row_count = mysql_num_rows( _db_query._mysql_res );
		//   //得到字段数量
		//   _db_query._field_count = mysql_num_fields( _db_query._mysql_res );
	}
	return _db_query;
}

/* 执行非返回结果查询 */
int CppMySQL3DB::execSQL(const char* sql)
{
	if( !mysql_real_query( _db_ptr, sql, strlen(sql) ) )
	{
		//得到受影响的行数
		return (int)mysql_affected_rows(_db_ptr) ;
	}

	return -1;
}

/* 测试mysql服务器是否存活 */
int CppMySQL3DB::ping()
{
	if( mysql_ping(_db_ptr) == 0 )
	{
		return 0;
	}

	return -1;
}

/* 关闭mysql 服务器 */
int CppMySQL3DB::shutDown()
{
	if( mysql_shutdown(_db_ptr, SHUTDOWN_DEFAULT) == 0 )
	{
		return 0;
	}

	return -1;
}

/* 主要功能:重新启动mysql 服务器 */
int CppMySQL3DB::reboot()
{
	if(!mysql_reload(_db_ptr))
	{
		return 0;
	}

	return -1;
}

/*
* 说明:事务支持InnoDB or BDB表类型
*/
/* 主要功能:开始事务 */
int CppMySQL3DB::startTransaction()
{
	if(!mysql_real_query(_db_ptr, "START TRANSACTION", (unsigned long)strlen("START TRANSACTION") ))
	{
		return 0;
	}

	return -1;
}

/* 主要功能:提交事务 */
int CppMySQL3DB::commit()
{
	if(!mysql_real_query( _db_ptr, "COMMIT", (unsigned long)strlen("COMMIT") ) )
	{
		return 0;
	}

	return -1;
}

/* 主要功能:回滚事务 */
int CppMySQL3DB::rollback()
{
	if(!mysql_real_query(_db_ptr, "ROLLBACK", (unsigned long)strlen("ROLLBACK") ) )
	{
		return 0;
	}

	return -1;
}

/* 得到客户信息 */
const char* CppMySQL3DB::getClientInfo()
{
	return mysql_get_client_info();
}

/* 主要功能:得到客户版本信息 */
const unsigned long  CppMySQL3DB::getClientVersion()
{
	return mysql_get_client_version();
}

/* 主要功能:得到主机信息 */
const char* CppMySQL3DB::getHostInfo()
{
	return mysql_get_host_info(_db_ptr);
}

/* 主要功能:得到服务器信息 */
const char* CppMySQL3DB::getServerInfo()
{
	return mysql_get_server_info( _db_ptr );
}

/*主要功能:得到服务器版本信息*/
const unsigned long  CppMySQL3DB::getServerVersion()
{
	return mysql_get_server_version(_db_ptr);
}

/*主要功能:得到 当前连接的默认字符集*/
const char*   CppMySQL3DB::getCharacterSetName()
{
	return mysql_character_set_name(_db_ptr);
}

/* 建立新数据库 */
int CppMySQL3DB::createDB(const char* name)
{
	char temp[1024];
	sprintf(temp, "CREATE DATABASE %s", name);
	if(!mysql_real_query( _db_ptr, temp, strlen(temp)) )
	{
		return 0;
	}

	return -1;
}

/* 删除制定的数据库*/
int CppMySQL3DB::dropDB(const char*  name)
{
	char temp[1024];

	sprintf(temp, "DROP DATABASE %s", name);
	if(!mysql_real_query( _db_ptr, temp, strlen(temp)) )
	{
		return 0;
	}

	return -1;
}

