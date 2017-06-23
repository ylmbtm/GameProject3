#include "stdafx.h"
#include "DBProcManager.h"
#include "Utility/CommonFunc.h"
#include "ObjectID.h"


CDBProcManager::CDBProcManager(void)
{
}

CDBProcManager::~CDBProcManager(void)
{
}

BOOL CDBProcManager::Init()
{
	std::string strCurDir = CommonFunc::GetCurrentDir();
	strCurDir+= "\\AccountData.db";

	m_DBConnection.open(strCurDir.c_str());

	return TRUE;

}

BOOL CDBProcManager::Uninit()
{
	m_DBConnection.close();

	return TRUE;
}

BOOL CDBProcManager::CreateAccount(UINT64 u64AccountID, const char *szAccount, const char *szPassword, UINT32 dwChannel)
{
	if(0 != GetAccountID(szAccount))
	{
		return FALSE;
	}

    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "insert into account(id, name, password, channel) values('%d','%s','%s', '%d')", u64AccountID, szAccount, szPassword, dwChannel);

	if(m_DBConnection.execDML(szSql) > 0)
	{
		return TRUE;
	}

	return FALSE;
}


UINT64 CDBProcManager::VerifyAccount( const char *szAccount, const char *szPassword )
{
    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select * from account where name ='%s' AND password='%s'", szAccount, szPassword);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())  
	{  
		return QueryRes.getIntField("id", 0);
	}  
	
	return 0;
}

UINT64 CDBProcManager::GetAccountID( const char *szAccount )
{
    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select * from account where name ='%s'", szAccount);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())  
	{  
		return QueryRes.getIntField("id", 0);
	}  

	return 0;
}

