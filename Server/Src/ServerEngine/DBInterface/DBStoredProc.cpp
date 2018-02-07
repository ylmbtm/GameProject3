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
			MYSQL_BIND* pTemp = &m_pMybind[i];
			free(pTemp->buffer);
		}

		delete []m_pMybind;
	}
}

// set bool value.
void CDBStoredProcedure::set_bool( int idx_, bool bval_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(my_bool));
	}

	*(my_bool*)temp->buffer = bval_;
	temp->buffer_length = sizeof(my_bool);
	temp->buffer_type = MYSQL_TYPE_TINY;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set int8 value.
void CDBStoredProcedure::set_int8(int idx_, int8 i8_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(int8));
	}

	*(int8*)temp->buffer = i8_;
	temp->buffer_length = sizeof(int8);
	temp->buffer_type = MYSQL_TYPE_TINY;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set uint8 value.
void CDBStoredProcedure::set_uint8(int idx_, uint8 ui8_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(uint8));
	}

	*(uint8*)temp->buffer = ui8_;
	temp->buffer_length = sizeof(uint8);
	temp->buffer_type = MYSQL_TYPE_TINY;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set int16 value.
void CDBStoredProcedure::set_int16(int idx_, int16 i16_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(int16));
	}

	*(int16*)temp->buffer = i16_;
	temp->buffer_length = sizeof(int16);
	temp->buffer_type = MYSQL_TYPE_SHORT;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set uint16 value.
void CDBStoredProcedure::set_uint16(int idx_, uint16 ui16_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(uint16));
	}

	*(uint16*)temp->buffer = ui16_;
	temp->buffer_length = sizeof(uint16);
	temp->buffer_type = MYSQL_TYPE_SHORT;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set int32 value.
void CDBStoredProcedure::set_int32(int idx_, int32 i32_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(int32));
	}

	*(int32*)temp->buffer = i32_;
	temp->buffer_length = sizeof(int32);
	temp->buffer_type = MYSQL_TYPE_LONG;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set uint32 value.
void CDBStoredProcedure::set_uint32(int idx_, uint32 ui32_)
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(uint32));
	}

	*(uint32*)temp->buffer = ui32_;
	temp->buffer_length = sizeof(uint32);
	temp->buffer_type = MYSQL_TYPE_LONG;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set int64 value.
void CDBStoredProcedure::set_int64(int idx_, int64 i64_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(int64));
	}

	*(int64*)temp->buffer = i64_;
	temp->buffer_length = sizeof(int64);
	temp->buffer_type = MYSQL_TYPE_LONGLONG;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set uint64 value.
void CDBStoredProcedure::set_uint64(int idx_, uint64 ui64_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(uint64));
	}

	*(uint64*)temp->buffer = ui64_;
	temp->buffer_length = sizeof(uint64);
	temp->buffer_type = MYSQL_TYPE_LONGLONG;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set float value.
void CDBStoredProcedure::set_float(int idx_, float fval_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(float));
	}

	*(float*)temp->buffer = fval_;
	temp->buffer_length = sizeof(float);
	temp->buffer_type = MYSQL_TYPE_FLOAT;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set double value.
void CDBStoredProcedure::set_double(int idx_, double dval_ )
{
	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(sizeof(double));
	}

	*(double*)temp->buffer = dval_;
	temp->buffer_length = sizeof(double);
	temp->buffer_type = MYSQL_TYPE_DOUBLE;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set string.
void CDBStoredProcedure::set_string(int idx_, char const* str_, size_t size)
{
	if ( NULL == str_ )
	{
		return ;
	}

	MYSQL_BIND* temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(size);
	}

	strncpy((char*)temp->buffer, str_, size);
	temp->buffer_length = (unsigned long)size;
	temp->buffer_type = MYSQL_TYPE_STRING;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

void CDBStoredProcedure::set_tinyblob(int idx_, void const* ptr_, size_t size)
{
	if (NULL == ptr_)
	{
		return;
	}

	MYSQL_BIND* temp = &m_pMybind[idx_];
	if (temp->buffer == NULL)
	{
		temp->buffer = malloc(size);
	}

	memcpy((char*)temp->buffer, ptr_, size);
	temp->buffer_length = (unsigned long)size;
	temp->buffer_type = MYSQL_TYPE_TINY_BLOB;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set blob.
void CDBStoredProcedure::set_blob(int idx_, void const* ptr_, size_t size)
{
	if (NULL == ptr_)
	{
		return;
	}

	MYSQL_BIND* temp = &m_pMybind[idx_];
	if (temp->buffer == NULL)
	{
		temp->buffer = malloc(size);
	}

	memcpy((char*)temp->buffer, ptr_, size);
	temp->buffer_length = (unsigned long)size;
	temp->buffer_type = MYSQL_TYPE_BLOB;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

void CDBStoredProcedure::set_medium_blob(int idx_, void const* ptr_, size_t size)
{
	if (NULL == ptr_)
	{
		return;
	}

	MYSQL_BIND* temp = &m_pMybind[idx_];
	if (temp->buffer == NULL)
	{
		temp->buffer = malloc(size);
	}

	memcpy((char*)temp->buffer, ptr_, size);
	temp->buffer_length = (unsigned long)size;
	temp->buffer_type = MYSQL_TYPE_MEDIUM_BLOB;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}
