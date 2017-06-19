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

	UINT32 GetMaxAccountID();

	BOOL CreateAccount(const char *szAccount, const char *szPassword);

	UINT32 VerifyAccount(const char *szAccount, const char *szPassword);

	UINT32 GetAccountID(const char *szAccount);

	CppSQLite3DB	m_DBConnection;

	UINT32			m_dwMaxAccountID;
};
#endif