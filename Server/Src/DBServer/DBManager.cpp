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
