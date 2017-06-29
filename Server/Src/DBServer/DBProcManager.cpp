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



BOOL CDBProcManager::CreateRole(UINT64 u64ID, UINT64 u64AccountID, char *pszName, INT32 nRoleType, INT32 nLevel)
{
	CHAR szSql[MAX_SQL_LEN];

	sprintf(szSql, "insert into player(id, account_id, name, roletype, level) values('%lld','%lld', '%s','%d','%d')",u64ID, u64AccountID, pszName, nRoleType, nLevel);

	if(m_DBConnection.execDML(szSql) <= 0)
	{
		return FALSE;
	}

	return TRUE;
}
