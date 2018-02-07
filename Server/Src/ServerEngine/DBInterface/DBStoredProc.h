#ifndef __DB_STORED_PROCEDURE__
#define __DB_STORED_PROCEDURE__

#include "DBRecordSet.h"

class CDBStoredProcedure
{
public:
	CDBStoredProcedure(char const* pzProcedure);
	~CDBStoredProcedure( void );

public:
	void set_bool( int idx_, bool bval_ );
	void set_int8(int idx_, int8 i8_ );
	void set_uint8(int idx_, uint8 ui8_ );
	void set_int16(int idx_, int16 i16_ );
	void set_uint16(int idx_, uint16 ui16_ );
	void set_int32(int idx_, int32 i32_ );
	void set_uint32(int idx_, uint32 ui32_ );
	void set_int64(int idx_, int64 i64_ );
	void set_uint64(int idx_, uint64 ui64_ );
	void set_float(int idx_, float fval_ );
	void set_double(int idx_, double dval_ );
	void set_string(int idx_, char const* str_, size_t size );
	void set_tinyblob(int idx_, void const* ptr_, size_t size);
	void set_blob(int idx_, void const* ptr_, size_t size);
	void set_medium_blob(int idx_, void const* ptr_, size_t size);

	CDBRecordSet		  m_DBRecordSet;

	MYSQL_BIND*           m_pMybind;
	size_t                m_nCount;
	std::string           m_strSql;
};

#endif // __DB_STORED_PROCEDURE__