#ifndef __DB_STORED_PROCEDURE__
#define __DB_STORED_PROCEDURE__

#include <my_global.h>
#include <mysql.h>
#include "DBRecordSet.h"

class CDBStoredProcedure
{
public:
	CDBStoredProcedure(char const *pzProcedure, int nParam);
	~CDBStoredProcedure( void );

public:
    void set_bool( size_t idx_, my_bool bval_ );
    void set_int8( size_t idx_, int8 i8_ );
    void set_uint8( size_t idx_, uint8 ui8_ );
    void set_int16( size_t idx_, int16 i16_ );
    void set_uint16( size_t idx_, uint16 ui16_ );
    void set_int32( size_t idx_, int32 i32_ );
    void set_uint32( size_t idx_, uint32 ui32_ );
    void set_int64( size_t idx_, int64 i64_ );
    void set_uint64( size_t idx_, uint64 ui64_ );
    void set_float( size_t idx_, float fval_ );
    void set_double( size_t idx_, double dval_ );
    void set_string( size_t idx_, char const *str_, size_t sz_ );
    void set_blob( size_t idx_, void const *ptr_, size_t sz_ );
	void set_medium_blob( size_t idx_, void const *ptr_, size_t sz_ );

	CDBRecordSet		  m_DBRecordSet;

    MYSQL_BIND           *m_pMybind;
    size_t                m_nCount;   // count of MYSQL_BIND
	std::string           m_strSql;

	
};

#endif // __DB_STORED_PROCEDURE__