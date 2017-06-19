#ifndef __DB_PROC_MANAGER__
#define __DB_PROC_MANAGER__

#include "Sqlite/CppSQLite3.h"

class CDBProcManager
{
public:
	CDBProcManager(void);
	~CDBProcManager(void);

	BOOL Init();

	BOOL Uninit();

	UINT64 GetCharID(char *szCharName);

	UINT64 GenarateCharID(UINT32 dwAccountID);

	CppSQLite3DB	m_DBConnection;
};
#endif