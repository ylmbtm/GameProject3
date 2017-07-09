#include "stdafx.h"
#include "DBManager.h"
#include "Utility/CommonFunc.h"
#include "../Message/Msg_Login.pb.h"
#include "Utility/Log/Log.h"
#include "../Message/Msg_RetCode.pb.h"


CDBManager::CDBManager(void)
{
}

CDBManager::~CDBManager(void)
{
}

BOOL CDBManager::Init()
{
	std::string strCurDir = CommonFunc::GetCurrentDir();
	strCurDir+= "\\GameData.db";

	m_DBConnection.open(strCurDir.c_str());

	return TRUE;

}

BOOL CDBManager::Uninit()
{
	m_DBConnection.close();

	return TRUE;
}



BOOL CDBManager::Execut(std::string sql)
{
	m_DBConnection.execDML(sql.c_str());

	return TRUE;
}


BOOL CDBManager::GetRoleList(UINT64 u64AccountID, RoleListAck &Ack)
{
	CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select * from player where account_id = %lld", u64AccountID);
    CLog::GetInstancePtr()->LogError(szSql);
	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())  
	{  
		RoleItem *pNode = Ack.add_rolelist();
		pNode->set_id(QueryRes.getInt64Field("id", 0));
		pNode->set_name(QueryRes.getStringField("name"));
		pNode->set_roletype(QueryRes.getIntField("roletype", 0));
		pNode->set_level(QueryRes.getIntField("level", 0));
		QueryRes.nextRow();
	}  

	return TRUE;
}

BOOL CDBManager::GetRoleData(UINT64 u64ID, RoleLoginAck &Ack)
{
	CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "select * from player where id = %lld", u64ID);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	if(!QueryRes.eof())  
	{  
        Ack.set_retcode(MRC_SUCCESSED);
		Ack.set_roleid(QueryRes.getInt64Field("id", 0));
		Ack.set_name(QueryRes.getStringField("name"));
		Ack.set_roletype(QueryRes.getIntField("roletype", 0));
		Ack.set_level(QueryRes.getIntField("level", 0));
		Ack.set_exp(QueryRes.getInt64Field("exp", 0));
        Ack.set_accountid(QueryRes.getInt64Field("account_id", 0));
	}  

	return TRUE;
}
