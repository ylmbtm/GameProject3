#include <stdafx.h>
#include "GlobalDataMgr.h"
#include "GameService.h"
#include "Sqlite/CppSQLite3.h"
#include "CommonFunc.h"
#include "Log.h"
#include "DataPool.h"

CGlobalDataManager::CGlobalDataManager()
{

}

CGlobalDataManager::~CGlobalDataManager()
{

}

CGlobalDataManager* CGlobalDataManager::GetInstancePtr()
{
	static CGlobalDataManager _StaticMgr;

	return &_StaticMgr;
}

BOOL CGlobalDataManager::LoadGlobalData()
{
	CppSQLite3DB	DBConnection;
	try
	{
		std::string strCurDir = CommonFunc::GetCurrentDir();
		strCurDir += "\\GameData.db";
		DBConnection.open(strCurDir.c_str());
	}
	catch(CppSQLite3Exception& e)
	{
		CLog::GetInstancePtr()->LogError("Error : File:%s", e.errorMessage());
		return FALSE;
	}

	UINT32 dwServerID  = 0;
	UINT64 dwMaxGuid = 0;

	CppSQLite3Query TableNames = DBConnection.execQuery("SELECT * FROM globaldata");
	if(!TableNames.eof())
	{
		dwServerID = TableNames.getIntField("serverid");
		dwMaxGuid = TableNames.getInt64Field("maxguid");
	}

	if((dwServerID == 0) || (dwMaxGuid == 0))
	{
		dwServerID = CGameService::GetInstancePtr()->GetServerID();
		dwMaxGuid  =  dwServerID;
		dwMaxGuid = (dwMaxGuid << 48) + 1;
	}
	dwMaxGuid += 100;
	m_pGlobalDataObject = g_pGlobalDataObjectPool->NewObject(TRUE);
	m_pGlobalDataObject->lock();
	m_pGlobalDataObject->m_dwServerID = dwServerID;
	m_pGlobalDataObject->m_u64Guid	  = dwMaxGuid;
	m_pGlobalDataObject->unlock();

	return TRUE;
}


UINT64 CGlobalDataManager::MakeNewGuid()
{
	m_pGlobalDataObject->lock();
	m_pGlobalDataObject->m_u64Guid	  += 1;
	m_pGlobalDataObject->unlock();
	return m_pGlobalDataObject->m_u64Guid;
}
