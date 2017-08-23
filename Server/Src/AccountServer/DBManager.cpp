#include "stdafx.h"
#include "DBManager.h"
#include "CommonFunc.h"

CDBManager::CDBManager(void)
{
}

CDBManager::~CDBManager(void)
{
}

BOOL CDBManager::Init()
{
	std::string strCurDir = CommonFunc::GetCurrentDir();
	strCurDir += "\\AccountData.db";

	m_DBConnection.open(strCurDir.c_str());

	return TRUE;

}

BOOL CDBManager::Uninit()
{
	m_DBConnection.close();

	return TRUE;
}

BOOL CDBManager::CreateAccount(UINT64 u64AccountID, const char* szAccount, const char* szPassword, UINT32 dwChannel, UINT32 dwCreateTime)
{
	CHAR szSql[1024];

	sprintf_s(szSql, 1024, "insert into account(id, name, password, channel, createtime) values('%lld','%s','%s', '%d', '%d')", u64AccountID, szAccount, szPassword, dwChannel, dwCreateTime);

	if(m_DBConnection.execDML(szSql) > 0)
	{
		return TRUE;
	}

	return FALSE;
}


UINT64 CDBManager::VerifyAccount( const char* szAccount, const char* szPassword )
{
	CHAR szSql[1024];

	sprintf_s(szSql, 1024, "select * from account where name ='%s' AND password='%s'", szAccount, szPassword);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())
	{
		return QueryRes.getIntField("id", 0);
	}

	return 0;
}

BOOL CDBManager::GetAccountData(const std::string strAccountName, UINT64& u64AccountID, std::string& strPwd, UINT32& dwChannel)
{
	CHAR szSql[1024];

	sprintf_s(szSql, 1024, "select * from account where name ='%s'", strAccountName.c_str());

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())
	{
		u64AccountID = QueryRes.getInt64Field("id");
		strPwd = QueryRes.getStringField("password");
		dwChannel = QueryRes.getIntField("channel");
		return TRUE;
	}

	return FALSE;
}
