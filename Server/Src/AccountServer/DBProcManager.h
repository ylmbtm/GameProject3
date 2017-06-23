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

	UINT64 GetMaxAccountID();

	BOOL CreateAccount(UINT64 u64AccountID, const char *szAccount, const char *szPassword, UINT32 dwChannel);

	UINT64 VerifyAccount(const char *szAccount, const char *szPassword);

	UINT64 GetAccountID(const char *szAccount);

	CppSQLite3DB	m_DBConnection;

	UINT64			m_64MaxAccountID;
};
#endif