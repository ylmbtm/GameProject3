#include "stdafx.h"
#include "DBStoredProc.h"
#include <cstdio>
#include <errmsg.h>


CDBStoredProcedure::CDBStoredProcedure(char const *pzProcedure, int nParam)
{
	m_nCount = nParam;

	m_pMybind = NULL;

	m_strSql = pzProcedure;

	if(nParam > 0)
	{
		m_pMybind = new MYSQL_BIND[nParam];

		memset(m_pMybind, 0, sizeof(MYSQL_BIND)*nParam);
	}
}

CDBStoredProcedure::~CDBStoredProcedure( void )
{
	if(m_pMybind != NULL)
	{
		for(size_t i = 0; i < m_nCount; ++i)
		{
			MYSQL_BIND *pTemp = &m_pMybind[i];

			delete pTemp->buffer;
		}

		delete []m_pMybind;
	}
}

// set bool value.
void CDBStoredProcedure::set_bool( size_t idx_, my_bool bval_ )
{
    MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new my_bool;
	}

	*(my_bool*)temp->buffer = bval_;
	temp->buffer_length = sizeof(my_bool);
	temp->buffer_type = MYSQL_TYPE_TINY;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set int8 value.
void CDBStoredProcedure::set_int8( size_t idx_, int8 i8_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new int8;
	}

	*(int8*)temp->buffer = i8_;
	temp->buffer_length = sizeof(int8);
	temp->buffer_type = MYSQL_TYPE_TINY;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set uint8 value.
void CDBStoredProcedure::set_uint8( size_t idx_, uint8 ui8_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new uint8;
	}

	*(uint8*)temp->buffer = ui8_;
	temp->buffer_length = sizeof(ui8_);
	temp->buffer_type = MYSQL_TYPE_TINY;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set int16 value.
void CDBStoredProcedure::set_int16( size_t idx_, int16 i16_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new int16;
	}

	*(int16*)temp->buffer = i16_;
	temp->buffer_length = sizeof(i16_);
	temp->buffer_type = MYSQL_TYPE_SHORT;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set uint16 value.
void CDBStoredProcedure::set_uint16( size_t idx_, uint16 ui16_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new uint16;
	}

	*(uint16*)temp->buffer = ui16_;
	temp->buffer_length = sizeof(ui16_);
	temp->buffer_type = MYSQL_TYPE_SHORT;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set int32 value.
void CDBStoredProcedure::set_int32( size_t idx_, int32 i32_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new int32;
	}

	*(int32*)temp->buffer = i32_;
	temp->buffer_length = sizeof(i32_);
	temp->buffer_type = MYSQL_TYPE_LONG;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set uint32 value.
void CDBStoredProcedure::set_uint32( size_t idx_, uint32 ui32_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new uint32;
	}

	*(uint32*)temp->buffer = ui32_;
	temp->buffer_length = sizeof(ui32_);
	temp->buffer_type = MYSQL_TYPE_LONG;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set int64 value.
void CDBStoredProcedure::set_int64( size_t idx_, int64 i64_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new int64;
	}

	*(int64*)temp->buffer = i64_;
	temp->buffer_length = sizeof(i64_);
	temp->buffer_type = MYSQL_TYPE_LONGLONG;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set uint64 value.
void CDBStoredProcedure::set_uint64( size_t idx_, uint64 ui64_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new uint64;
	}

	*(uint64*)temp->buffer = ui64_;
	temp->buffer_length = sizeof(uint64);
	temp->buffer_type = MYSQL_TYPE_LONGLONG;
	temp->is_unsigned = UNSIGNED_FLAG;
	temp->is_null_value = 0;
}

// set float value.
void CDBStoredProcedure::set_float( size_t idx_, float fval_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new float;
	}

	*(float*)temp->buffer = fval_;
	temp->buffer_length = sizeof(float);
	temp->buffer_type = MYSQL_TYPE_FLOAT;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set double value.
void CDBStoredProcedure::set_double( size_t idx_, double dval_ )
{
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = new double;
	}

	*(double*)temp->buffer = dval_;
	temp->buffer_length = sizeof(dval_);
	temp->buffer_type = MYSQL_TYPE_DOUBLE;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set string.
void CDBStoredProcedure::set_string( size_t idx_, char const *str_, size_t sz_ )
{
    if ( NULL == str_ )
    {
		return ;
	}
	
	MYSQL_BIND *temp = &m_pMybind[idx_];
	if(temp->buffer == NULL)
	{
		temp->buffer = malloc(256);
	}

	strncpy((char*)temp->buffer, str_, 256);
	temp->buffer_length = sz_;
	temp->buffer_type = MYSQL_TYPE_STRING;
	temp->is_unsigned = 0;
	temp->is_null_value = 0;
}

// set blob.
void CDBStoredProcedure::set_blob( size_t idx_, void const *ptr_, size_t sz_ )
{
 /*   if ( NULL != ptr_ )
    {
        param_t *_ptr = &m_params_[idx_];

        _ptr->buf.assign( (char*)ptr_, sz_ );
        _ptr->type = MYSQL_TYPE_BLOB;
        _ptr->is_unsigned = 0;
        _ptr->is_null_val = 0;
    }*/
}

// set blob.
void CDBStoredProcedure::set_medium_blob( size_t idx_, void const *ptr_, size_t sz_ )
{
/*	if ( NULL != ptr_ )
	{
		param_t *_ptr = &m_params_[idx_];

		_ptr->buf.assign( (char*)ptr_, sz_ );
		_ptr->type = MYSQL_TYPE_MEDIUM_BLOB;
		_ptr->is_unsigned = 0;
		_ptr->is_null_val = 0;
	}*/
}
