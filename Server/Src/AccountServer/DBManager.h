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


	BOOL CreateAccount(UINT64 u64AccountID, const char* szAccount, const char* szPassword, UINT32 dwChannel, UINT32 dwCreateTime);

	UINT64 VerifyAccount(const char* szAccount, const char* szPassword);

	BOOL GetAccountData(const std::string strAccountName, UINT64& u64AccountID, std::string& strPwd, UINT32& dwChannel);

	CppSQLite3DB	m_DBConnection;
};
#endif