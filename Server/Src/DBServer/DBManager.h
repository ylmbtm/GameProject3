#ifndef __DB_PROC_MANAGER__
#define __DB_PROC_MANAGER__
#include "Sqlite/CppSQLite3.h"
#include "DBConnection.h"
#include "DBStoredProcMgr.h"
#include "../ServerData/serverStruct.h"

class CDBManager : public IDataBase
{
public:
	CDBManager(void);

	~CDBManager(void);

	BOOL Init();

	BOOL Uninit();

	BOOL Execut(std::string sql);

	CppSQLite3DB	m_DBConnection;
	//真实的mysql
	//CDBConnection   m_DBConnection;

	//CDBStoredProcedureMgr m_DBProceduceMgr;
};
#endif