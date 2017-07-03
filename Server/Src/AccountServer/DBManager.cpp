#include "stdafx.h"
#include "DBManager.h"
#include "Utility/CommonFunc.h"

CDBManager::CDBManager(void)
{
}

CDBManager::~CDBManager(void)
{
}

BOOL CDBManager::Init()
{
	std::string strCurDir = CommonFunc::GetCurrentDir();
	strCurDir+= "\\AccountData.db";

	m_DBConnection.open(strCurDir.c_str());

	return TRUE;

}

BOOL CDBManager::Uninit()
{
	m_DBConnection.close();

	return TRUE;
}

BOOL CDBManager::CreateAccount(UINT64 u64AccountID, const char *szAccount, const char *szPassword, UINT32 dwChannel, UINT32 dwCreateTime)
{
	if(0 != GetAccountID(szAccount))
	{
		return FALSE;
	}

    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "insert into account(id, name, password, channel, createtime) values('%lld','%s','%s', '%d', '%d')", u64AccountID, szAccount, szPassword, dwChannel, dwCreateTime);

	if(m_DBConnection.execDML(szSql) > 0)
	{
		return TRUE;
	}

	return FALSE;
}


UINT64 CDBManager::VerifyAccount( const char *szAccount, const char *szPassword )
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

UINT64 CDBManager::GetAccountID( const char *szAccount )
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
