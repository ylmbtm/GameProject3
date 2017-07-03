#include "stdafx.h"
#include "DBManager.h"
#include "Utility/CommonFunc.h"
#include "../Message/Msg_Login.pb.h"


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

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())  
	{  
		RoleListNode *pNode = Ack.add_rolenode();
		pNode->set_id(QueryRes.getInt64Field("id", 0));
		pNode->set_name(QueryRes.getStringField("name"));
		pNode->set_roletype(QueryRes.getIntField("roletype", 0));
		pNode->set_level(QueryRes.getIntField("level", 0));
		pNode->set_exp(QueryRes.getInt64Field("exp", 0));

		QueryRes.nextRow();
	}  

	return TRUE;
}
