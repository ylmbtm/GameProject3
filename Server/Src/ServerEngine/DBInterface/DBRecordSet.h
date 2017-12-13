
#ifndef _SQL_RESULT_H_
#define _SQL_RESULT_H_

#include <my_global.h>
#include <mysql.h>
#undef snprintf

class CDBRecordSet
{
public:
	CDBRecordSet( void );
	~CDBRecordSet( void );

public:
	void SetFieldType(int nIndex, enum_field_types fdType);
	BOOL InitRecordSet(MYSQL_STMT* pMySqlStmt, MYSQL_RES* pResult);
	BOOL ClearRecordSet();


public:
	// next.
	BOOL MoveNext( void );

	// count.
	int32 GetRowCount( void );

	bool get_bool( size_t idx_ );
	int8 get_int8( size_t idx_ );
	uint8 get_uint8( size_t idx_ );
	int16 get_int16( size_t idx_ );
	uint16 get_uint16( size_t idx_ );
	int32 get_int32( size_t idx_ );
	uint32 get_uint32( size_t idx_ );
	int64 get_int64( size_t idx_ );
	uint64 get_uint64( size_t idx_ );
	float get_float( size_t idx_ );
	double get_double( size_t idx_ );
	char* get_string( size_t idx_ );
	std::pair<size_t, void const*> get_blob( size_t idx_ );
	std::pair<size_t, void const*> get_medium_blob( size_t idx_ );

	bool get_bool( char* pname);
	int8 get_int8( char* pname );
	uint8 get_uint8( char* pname );
	int16 get_int16( char* pname );
	uint16 get_uint16( char* pname);
	int32 get_int32( char* pname);
	uint32 get_uint32(char* pname );
	int64 get_int64( char* pname );
	uint64 get_uint64(char* pname );
	float get_float( char* pname );
	double get_double( char* pname );
	char* get_string(char* pname );
	std::pair<size_t, void const*> get_blob( char* pname );
	std::pair<size_t, void const*> get_medium_blob( char* pname );


	unsigned long nLen;
private:
	int                          m_nRowCount;
	int							 m_nFieldNum;

	std::map<std::string, int>   m_NameToIdx;


	MYSQL_STMT*					m_pMySqlStmt;
	MYSQL_RES*					m_pResult;
	MYSQL_BIND*					m_pBinds;
};

#endif // _SQL_RESULT_H_