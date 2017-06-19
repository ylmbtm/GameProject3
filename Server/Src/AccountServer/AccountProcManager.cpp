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

BOOL CDBProcManager::CreateAccount( char *szAccount, char *szPassword )
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


BOOL CDBProcManager::LoadAccountCharInfo( UINT32 dwAccountID, StCharLoginAck &Ack )
{
    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select * from t_charinfo where F_AccountID = '%d'", dwAccountID);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	int i = 0;
	
	while(!QueryRes.eof())
	{
		Ack.CharPickInfo[i].dwFeature = QueryRes.getIntField("F_Feature", 0);
		strncpy(Ack.CharPickInfo[i].szCharName, QueryRes.getStringField("F_Name", ""), 32);
		Ack.CharPickInfo[i].dwLevel = QueryRes.getIntField("F_Level", 0);
		Ack.CharPickInfo[i].u64CharID = QueryRes.getInt64Field("F_CharID");
		i++;
		QueryRes.nextRow();
	}

	Ack.nCount = i;

	return TRUE;
}

UINT32 CDBProcManager::VerifyAccount( char *szAccount, char *szPassword )
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

BOOL CDBProcManager::CreateNewChar(StCharNewCharReq &Req,  StCharNewCharAck &Ack)
{
	if(GetCharID(Req.szCharName) != 0)
	{
		return FALSE;
	}

	UINT64 u64NewCharID = GenarateCharID(Req.dwAccountID);

    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "insert into t_charinfo(F_CharID, F_AccountID, F_Name, F_Feature) values('%lld','%d', '%s','%d')",u64NewCharID, Req.dwAccountID, Req.szCharName, Req.dwFeature);

	if(m_DBConnection.execDML(szSql) <= 0)
	{
		return FALSE;
	}
	
	sprintf(szSql, "select * from t_charinfo where F_Name ='%s'", Req.szCharName);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())
	{
		Ack.CharPickInfo.dwFeature = QueryRes.getIntField("F_Feature", 0);
		strncpy(Ack.CharPickInfo.szCharName, QueryRes.getStringField("F_Name", ""), 32);
		Ack.CharPickInfo.dwLevel = QueryRes.getIntField("F_Level", 0);
		Ack.CharPickInfo.u64CharID = QueryRes.getInt64Field("F_CharID");

		return TRUE;
	}

	return FALSE;
}

UINT32 CDBProcManager::GetMaxAccountID()
{

	return 0;
}

UINT64 CDBProcManager::GetCharID( char *szCharName )
{
    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select * from t_charinfo where F_Name ='%s'", szCharName);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())  
	{  
		return QueryRes.getInt64Field("F_CharID", 0);
	}  

	return 0;
}

UINT32 CDBProcManager::GetAccountID( char *szAccount )
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

BOOL CDBProcManager::DelChar( StCharDelCharReq &Req)
{
    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "delete from t_charinfo where F_CharID='%lld'", Req.u64CharID);

	if(m_DBConnection.execDML(szSql) <= 0)
	{
		return FALSE;
	}

	return TRUE;
}

UINT64 CDBProcManager::GenarateCharID( UINT32 dwAccountID )
{
    CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select max(F_CharID) as F_CharID from t_charinfo where F_AccountID ='%d'", dwAccountID);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	UINT64 dwTempID = 0;
	while(!QueryRes.eof())  
	{  
		dwTempID = QueryRes.getInt64Field("F_CharID", 0);
		break;
	}  

	UINT32 dwIndex = GET_ROLE_INDEX(dwTempID) + 1;

	return MAKE_PLAYER_ID(dwIndex, dwAccountID);
}

