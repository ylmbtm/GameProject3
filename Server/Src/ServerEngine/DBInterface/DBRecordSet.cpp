#include "stdafx.h"
#include "DBRecordSet.h"

CDBRecordSet::CDBRecordSet( void )
{
	m_nRowCount		= 0;
	m_pMySqlStmt	= 0;
	m_pResult		= 0;
	m_nFieldNum		= 0;
	m_pBinds		= 0;
}

CDBRecordSet::~CDBRecordSet( void )
{
	ClearRecordSet();
	m_nRowCount = 0;
	m_pMySqlStmt = 0;
	m_pResult = 0;
	m_nFieldNum = 0;
	m_pBinds = 0;
}

BOOL CDBRecordSet::MoveNext( void )
{
	if(0 == mysql_stmt_fetch(m_pMySqlStmt))
	{
		return TRUE;
	}

	return FALSE;
}

int32 CDBRecordSet::GetRowCount(void)
{
	if(m_pMySqlStmt == NULL)
	{
		return 0;
	}

	m_nRowCount  = (int32)mysql_stmt_num_rows(m_pMySqlStmt);

	return m_nRowCount;
}

bool CDBRecordSet::get_bool( size_t idx_ )
{
	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}

	bool Ret = false;
	switch (pTemp->buffer_type)
	{
		case MYSQL_TYPE_TINY:
			Ret = ( 0 != ( *(uint8*)pTemp->buffer ) );
			break;
		case MYSQL_TYPE_SHORT:
			Ret = ( 0 != ( *(uint16*)pTemp->buffer ) );
			break;
		case MYSQL_TYPE_INT24:
			Ret = ( 0 != ( *(uint32*)pTemp->buffer ) );
			break;
		case MYSQL_TYPE_LONG:
			Ret = ( 0 != ( *(uint32*)pTemp->buffer ) );
			break;
		case MYSQL_TYPE_LONGLONG:
			Ret = ( 0 != ( *(uint64*)pTemp->buffer ) );
			break;
		case MYSQL_TYPE_FLOAT:
			Ret = ( 0 != ( *(uint32*)pTemp->buffer ) );
			break;
		case MYSQL_TYPE_DOUBLE:
			Ret = ( 0 != ( *(uint64*)pTemp->buffer ) );
			break;
	}

	return Ret;
}

bool CDBRecordSet::get_bool(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_bool(nIdx);
}

int8 CDBRecordSet::get_int8( size_t idx_ )
{
	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}

	int8 Ret = 0;
	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			Ret = (int8)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			Ret = (int8)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			Ret = (int8)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			Ret = (int8)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			Ret = (int8)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			Ret = (int8)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			Ret = (int8)( *(double*)pTemp->buffer );
			break;
	}

	return Ret;
}

int8 CDBRecordSet::get_int8(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_int8(nIdx);
}

uint8 CDBRecordSet::get_uint8( size_t idx_ )
{
	uint8 _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}

	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (uint8)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (uint8)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (uint8)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (uint8)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (uint8)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (uint8)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (uint8)( *(double*)pTemp->buffer );
			break;
	}

	return _ret;
}

uint8 CDBRecordSet::get_uint8(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_uint8(nIdx);
}

int16 CDBRecordSet::get_int16( size_t idx_ )
{
	int16 _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}
	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (int16)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (int16)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (int16)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (int16)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (int16)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (int16)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (int16)( *(double*)pTemp->buffer );
			break;
	}

	return _ret;
}

int16 CDBRecordSet::get_int16(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_int16(nIdx);
}

uint16 CDBRecordSet::get_uint16( size_t idx_ )
{
	uint16 _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}
	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (uint16)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (uint16)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (uint16)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (uint16)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (uint16)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (uint16)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (uint16)( *(double*)pTemp->buffer );
			break;
	}

	return _ret;
}

uint16 CDBRecordSet::get_uint16(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_uint16(nIdx);
}

int32 CDBRecordSet::get_int32( size_t idx_ )
{
	int32 _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}
	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (int32)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (int32)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (int32)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (int32)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (int32)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (int32)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (int32)( *(double*)pTemp->buffer );
			break;
	}

	return _ret;
}

int32 CDBRecordSet::get_int32(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_int32(nIdx);
}

uint32 CDBRecordSet::get_uint32( size_t idx_ )
{
	uint32 _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}

	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (uint32)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (uint32)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (uint32)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (uint32)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (uint32)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (uint32)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (uint32)( *(double*)pTemp->buffer );
			break;
	}

	return _ret;
}

uint32 CDBRecordSet::get_uint32(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_uint32(nIdx);
}

int64 CDBRecordSet::get_int64( size_t idx_ )
{
	int64 _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}
	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (int64)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (int64)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (int64)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (int64)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (int64)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (int64)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (int64)( *(double*)pTemp->buffer );
			break;
	}

	return _ret;
}

int64 CDBRecordSet::get_int64(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_int64(nIdx);
}

uint64 CDBRecordSet::get_uint64( size_t idx_ )
{
	uint64 _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}
	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (uint64)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (uint64)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (uint64)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (uint64)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (uint64)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (uint64)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (uint64)( *(double*)pTemp->buffer );
			break;
	}
	return _ret;
}

uint64 CDBRecordSet::get_uint64(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_uint64(nIdx);
}

float CDBRecordSet::get_float( size_t idx_ )
{
	float _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}
	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (float)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (float)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (float)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (float)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (float)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (float)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (float)( *(double*)pTemp->buffer );
			break;
	}

	return _ret;
}

float CDBRecordSet::get_float(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_float(nIdx);
}

double CDBRecordSet::get_double( size_t idx_ )
{
	double _ret = 0;

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return 0;
	}
	switch ( pTemp->buffer_type )
	{
		case MYSQL_TYPE_TINY:
			_ret = (double)( *(uint8*)pTemp->buffer );
			break;
		case MYSQL_TYPE_SHORT:
			_ret = (double)( *(uint16*)pTemp->buffer );
			break;
		case MYSQL_TYPE_INT24:
			_ret = (double)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONG:
			_ret = (double)( *(uint32*)pTemp->buffer );
			break;
		case MYSQL_TYPE_LONGLONG:
			_ret = (double)( *(uint64*)pTemp->buffer );
			break;
		case MYSQL_TYPE_FLOAT:
			_ret = (double)( *(float*)pTemp->buffer );
			break;
		case MYSQL_TYPE_DOUBLE:
			_ret = (double)( *(double*)pTemp->buffer );
			break;
	}

	return _ret;
}

double CDBRecordSet::get_double(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_double(nIdx);
}

char* CDBRecordSet::get_string( size_t idx_ )
{
	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return NULL;
	}

	return (char*)pTemp->buffer;
}

char* CDBRecordSet::get_string(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_string(nIdx);
}

std::pair<size_t, void const*> CDBRecordSet::get_blob( size_t idx_ )
{
	std::pair<size_t, void const*> _ret( 0, "" );

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return _ret;
	}

	_ret.first		= pTemp->length_value;
	_ret.second	= pTemp->buffer;

	return _ret;
}

std::pair<size_t, void const*> CDBRecordSet::get_blob(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_blob(nIdx);
}

// get medium blob.
std::pair<size_t, void const*> CDBRecordSet::get_medium_blob( size_t idx_ )
{
	std::pair<size_t, void const*> _ret( 0, "" );

	MYSQL_BIND* pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return _ret;
	}

	_ret.first		= pTemp->length_value;
	_ret.second		= pTemp->buffer;

	return _ret;
}

std::pair<size_t, void const*> CDBRecordSet::get_medium_blob(char* pname)
{
	int nIdx = m_NameToIdx[pname];
	return get_medium_blob(nIdx);
}

void CDBRecordSet::SetFieldType( int nIndex, enum_field_types fdType )
{
	MYSQL_BIND* pTemp = &m_pBinds[nIndex];

	int BufferSize = 0;

	switch ( fdType )
	{
		case MYSQL_TYPE_TINY:
			BufferSize = 1;
			break;
		case MYSQL_TYPE_SHORT:
			BufferSize = 2;
			break;
		case MYSQL_TYPE_INT24:
			BufferSize = 4;
			break;
		case MYSQL_TYPE_LONG:
			BufferSize = 4;
			break;
		case MYSQL_TYPE_LONGLONG:
			BufferSize = 8;
			break;
		case MYSQL_TYPE_FLOAT:
			BufferSize = 4;
			break;
		case MYSQL_TYPE_DOUBLE:
			BufferSize = 8;
			break;
		case MYSQL_TYPE_STRING:
			BufferSize = 255;
			break;
		case MYSQL_TYPE_VAR_STRING:
			BufferSize = 65535;
			break;
		case MYSQL_TYPE_TINY_BLOB:
			BufferSize = 256;
			break;
		case MYSQL_TYPE_BLOB:
			BufferSize = 65535;
			break;
		case MYSQL_TYPE_MEDIUM_BLOB:
			BufferSize = 16777215;
			break;
		case MYSQL_TYPE_LONG_BLOB:
			BufferSize = 4026531840;
			break;
	}

	nLen = 0;
	pTemp->buffer			= malloc(BufferSize);
	pTemp->buffer_length	= BufferSize;
	pTemp->buffer_type      = fdType;
	pTemp->length			= &nLen;

	return ;
}

BOOL CDBRecordSet::InitRecordSet(MYSQL_STMT* pMySqlStmt, MYSQL_RES* pResult)
{
	if((pMySqlStmt == NULL) || (pResult == NULL))
	{
		return FALSE;
	}

	m_pMySqlStmt = pMySqlStmt;
	m_pResult    = pResult;

	m_nRowCount  = (int)mysql_stmt_num_rows(m_pMySqlStmt);
	if(m_nRowCount <= 0)
	{
		return TRUE;
	}

	if(m_pBinds != NULL)
	{
		return TRUE;
	}

	m_nFieldNum = mysql_stmt_field_count( m_pMySqlStmt );
	if ( m_nFieldNum > 0 )
	{
		m_pBinds = new MYSQL_BIND[m_nFieldNum];
		memset(m_pBinds, 0, sizeof(MYSQL_BIND)*m_nFieldNum);
	}

	if ( NULL == m_pBinds )
	{
		return FALSE;
	}


	MYSQL_FIELD* pField = mysql_fetch_field( m_pResult );
	int nIndex = 0;
	while ( NULL != pField )
	{
		m_NameToIdx[pField->name] = nIndex;

		SetFieldType(nIndex, pField->type);

		pField = mysql_fetch_field( m_pResult );

		++nIndex;
	}

	if (0 != mysql_stmt_bind_result(pMySqlStmt, m_pBinds))
	{
		//int nErrno = mysql_errno( m_pMySql );
		//char *pStrError = mysql_error( m_pMySql );
		pMySqlStmt = NULL;
		//CLog::GetInstancePtr()->LogError("CDBConnection::Execute Failed [mysql_stmt_store_result], ErrorNo:%d, ErrorMsg:%s", nErrno, pStrError);
		mysql_stmt_close( pMySqlStmt );
		return FALSE;
	}

	return TRUE;
}


BOOL CDBRecordSet::ClearRecordSet()
{
	for (int i = 0; i < m_nFieldNum; i++)
	{
		MYSQL_BIND* pTemp = &m_pBinds[i];
		free(pTemp->buffer);
		pTemp->buffer = NULL;
	}

	delete[]m_pBinds;

	m_pBinds = NULL;

	if(m_pResult != NULL)
	{
		mysql_free_result( m_pResult );

		m_pResult = NULL;
	}

	if(m_pMySqlStmt != NULL)
	{
		mysql_stmt_free_result( m_pMySqlStmt );

		m_pMySqlStmt = NULL;
	}


	m_nFieldNum = 0;

	return TRUE;
}
