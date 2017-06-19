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



