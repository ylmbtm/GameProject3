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
	strCurDir+= "\\GameData.db";

	m_DBConnection.open(strCurDir.c_str());

	return TRUE;

}

BOOL CDBProcManager::Uninit()
{
	m_DBConnection.close();

	return TRUE;
}

BOOL CDBProcManager::CreateAccount( const char *szAccount, const char *szPassword )
{
	if(0 != GetAccountID(szAccount))
	{
		return FALSE;
	}

    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "insert into t_accountinfo(F_AccountName, F_Password) values('%s', '%s')", szAccount, szPassword);

	if(m_DBConnection.execDML(szSql) > 0)
	{
		return TRUE;
	}

	return FALSE;
}


UINT32 CDBProcManager::VerifyAccount( const char *szAccount, const char *szPassword )
{
    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select * from t_accountinfo where F_AccountName ='%s' AND F_Password='%s'", szAccount, szPassword);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())  
	{  
		return QueryRes.getIntField("F_AccountID", 0);
	}  
	
	return 0;
}


UINT32 CDBProcManager::GetMaxAccountID()
{

	return 0;
}

UINT32 CDBProcManager::GetAccountID( const char *szAccount )
{
    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select * from t_accountinfo where F_AccountName ='%s'", szAccount);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())  
	{  
		return QueryRes.getIntField("F_AccountID", 0);
	}  

	return 0;
}

