#include "stdafx.h"
#include "DBStoredProc.h"


CDBStoredProcedure::CDBStoredProcedure(char const* pzProcedure)
{
	m_pMybind = NULL;

	m_strSql = pzProcedure;

	m_nCount = CommonConvert::CountSymbol((char*)pzProcedure, '?');

	if(m_nCount > 0)
	{
		m_pMybind = new MYSQL_BIND[m_nCount];

		memset(m_pMybind, 0, sizeof(MYSQL_BIND)*m_nCount);
	}
}

CDBStoredProcedure::~CDBStoredProcedure( void )
{
	if(m_pMybind != NULL)
	{
		for(size_t i = 0; i < m_nCount; ++i)
		{
			MYSQL_BIND* pBind = &m_pMybind[i];

			free(pBind->buffer);
		}

		delete []m_pMybind;
		m_pMybind = NULL;
	}
}

// set bool value.
void CDBStoredProcedure::set_bool( int idx_, bool bval_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(my_bool));
	}

	*(my_bool*)pBind->buffer = bval_;
	pBind->buffer_length = sizeof(my_bool);
	pBind->buffer_type = MYSQL_TYPE_TINY;
	pBind->is_unsigned = UNSIGNED_FLAG;
}

// set int8 value.
void CDBStoredProcedure::set_int8(int idx_, int8 i8_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(int8));
	}

	*(int8*)pBind->buffer = i8_;
	pBind->buffer_length = sizeof(int8);
	pBind->buffer_type = MYSQL_TYPE_TINY;
}

// set uint8 value.
void CDBStoredProcedure::set_uint8(int idx_, uint8 ui8_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(uint8));
	}

	*(uint8*)pBind->buffer = ui8_;
	pBind->buffer_length = sizeof(uint8);
	pBind->buffer_type = MYSQL_TYPE_TINY;
	pBind->is_unsigned = UNSIGNED_FLAG;
}

// set int16 value.
void CDBStoredProcedure::set_int16(int idx_, int16 i16_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(int16));
	}

	*(int16*)pBind->buffer = i16_;
	pBind->buffer_length = sizeof(int16);
	pBind->buffer_type = MYSQL_TYPE_SHORT;
}

// set uint16 value.
void CDBStoredProcedure::set_uint16(int idx_, uint16 ui16_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(uint16));
	}

	*(uint16*)pBind->buffer = ui16_;
	pBind->buffer_length = sizeof(uint16);
	pBind->buffer_type = MYSQL_TYPE_SHORT;
	pBind->is_unsigned = UNSIGNED_FLAG;
}

// set int32 value.
void CDBStoredProcedure::set_int32(int idx_, int32 i32_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(int32));
	}

	*(int32*)pBind->buffer = i32_;
	pBind->buffer_length = sizeof(int32);
	pBind->buffer_type = MYSQL_TYPE_LONG;
}

// set uint32 value.
void CDBStoredProcedure::set_uint32(int idx_, uint32 ui32_)
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(uint32));
	}

	*(uint32*)pBind->buffer = ui32_;
	pBind->buffer_length = sizeof(uint32);
	pBind->buffer_type = MYSQL_TYPE_LONG;
	pBind->is_unsigned = UNSIGNED_FLAG;
}

// set int64 value.
void CDBStoredProcedure::set_int64(int idx_, int64 i64_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(int64));
	}

	*(int64*)pBind->buffer = i64_;
	pBind->buffer_length = sizeof(int64);
	pBind->buffer_type = MYSQL_TYPE_LONGLONG;
}

// set uint64 value.
void CDBStoredProcedure::set_uint64(int idx_, uint64 ui64_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(uint64));
	}

	*(uint64*)pBind->buffer = ui64_;
	pBind->buffer_length = sizeof(uint64);
	pBind->buffer_type = MYSQL_TYPE_LONGLONG;
	pBind->is_unsigned = UNSIGNED_FLAG;
}

// set float value.
void CDBStoredProcedure::set_float(int idx_, float fval_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(float));
	}

	*(float*)pBind->buffer = fval_;
	pBind->buffer_length = sizeof(float);
	pBind->buffer_type = MYSQL_TYPE_FLOAT;
}

// set double value.
void CDBStoredProcedure::set_double(int idx_, double dval_ )
{
	ERROR_RETURN_NONE(idx_ < m_nCount);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if(pBind->buffer == NULL)
	{
		pBind->buffer = malloc(sizeof(double));
	}

	*(double*)pBind->buffer = dval_;
	pBind->buffer_length = sizeof(double);
	pBind->buffer_type = MYSQL_TYPE_DOUBLE;
}

// set string.
void CDBStoredProcedure::set_string(int idx_, char const* str_, size_t size)
{
	ERROR_RETURN_NONE(NULL != str_);
	ERROR_RETURN_NONE(idx_ < m_nCount);
	ERROR_RETURN_NONE(size < 2048);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if (pBind->buffer == NULL)
	{
		pBind->buffer = malloc(2048);
	}

	memset(pBind->buffer, 0, 2048);

	pBind->buffer_length = 2048;
	pBind->length_value = size;
	pBind->length = &pBind->length_value;
	memcpy((char*)pBind->buffer, str_, size);
	pBind->buffer_type = MYSQL_TYPE_STRING;
}

void CDBStoredProcedure::set_tinyblob(int idx_, void const* ptr_, size_t size)
{
	ERROR_RETURN_NONE(NULL != ptr_);
	ERROR_RETURN_NONE(idx_ < m_nCount);
	ERROR_RETURN_NONE(size < 255);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if (pBind->buffer == NULL)
	{
		pBind->buffer = malloc(255);
	}

	memset(pBind->buffer, 0, 255);
	pBind->buffer_length = 255;
	pBind->length_value = size;
	pBind->length = &pBind->length_value;
	memcpy((char*)pBind->buffer, ptr_, size);
	pBind->buffer_type = MYSQL_TYPE_TINY_BLOB;
}

// set blob.
void CDBStoredProcedure::set_blob(int idx_, void const* ptr_, size_t size)
{
	ERROR_RETURN_NONE(NULL != ptr_);
	ERROR_RETURN_NONE(idx_ < m_nCount);
	ERROR_RETURN_NONE(size < 65 * 1024);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if (pBind->buffer == NULL)
	{
		pBind->buffer = malloc(65 * 1024);
	}

	memset(pBind->buffer, 0, (int)65 * 1024);
	pBind->buffer_length = 65 * 1024;
	pBind->length_value = size;
	pBind->length = &pBind->length_value;
	memcpy((char*)pBind->buffer, ptr_, size);
	pBind->buffer_type = MYSQL_TYPE_BLOB;
}

void CDBStoredProcedure::set_medium_blob(int idx_, void const* ptr_, size_t size)
{
	ERROR_RETURN_NONE(NULL != ptr_);
	ERROR_RETURN_NONE(idx_ < m_nCount);
	ERROR_RETURN_NONE(size < 1024 * 1024);
	MYSQL_BIND* pBind = &m_pMybind[idx_];
	if (pBind->buffer == NULL)
	{
		pBind->buffer = malloc(1024 * 1024);
	}

	memset(pBind->buffer, 0, 1024 * 1024);
	pBind->buffer_length = 1024 * 1024;
	pBind->length_value = size;
	pBind->length = &pBind->length_value;
	memcpy((char*)pBind->buffer, ptr_, size);
	pBind->buffer_type = MYSQL_TYPE_MEDIUM_BLOB;
}
