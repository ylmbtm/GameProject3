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

	BOOL CreateRole(UINT64 u64ID, UINT64 u64AccountID, char *pszName, INT32 nRoleType, INT32 nLevel);


	CppSQLite3DB	m_DBConnection;
};
#endif