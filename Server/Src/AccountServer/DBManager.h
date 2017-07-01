#ifndef __DB_PROC_MANAGER__
#define __DB_PROC_MANAGER__

#include "Sqlite/CppSQLite3.h"

class CDBManager
{
public:
	CDBManager(void);
	~CDBManager(void);

	BOOL Init();

	BOOL Uninit();


	BOOL CreateAccount(UINT64 u64AccountID, const char *szAccount, const char *szPassword, UINT32 dwChannel, UINT32 dwCreateTime);

	UINT64 VerifyAccount(const char *szAccount, const char *szPassword);

	UINT64 GetAccountID(const char *szAccount);

	CppSQLite3DB	m_DBConnection;
};
#endif