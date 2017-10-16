#include <stdafx.h>
#include "GlobalDataMgr.h"
#include "GameService.h"
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

BOOL CGlobalDataManager::LoadGlobalData(CppMySQL3DB& tDBConnection)
{
	UINT64 dwMaxGuid = 0;

	CHAR szSql[SQL_BUFF_LEN];
	sprintf_s(szSql, SQL_BUFF_LEN, "select * from globaldata where serverid = %d", CGameService::GetInstancePtr()->GetServerID());

	CppMySQLQuery QueryResult = tDBConnection.querySQL("SELECT * FROM globaldata");
	if(!QueryResult.eof())
	{
		dwMaxGuid = QueryResult.getInt64Field("maxguid");
	}

	if(dwMaxGuid == 0)
	{
		dwMaxGuid  =  CGameService::GetInstancePtr()->GetServerID();
		dwMaxGuid = (dwMaxGuid << 48) + 1;
	}
	dwMaxGuid += 100;
	m_pGlobalDataObject = g_pGlobalDataObjectPool->NewObject(FALSE);
	m_pGlobalDataObject->lock();
	m_pGlobalDataObject->m_dwServerID = CGameService::GetInstancePtr()->GetServerID();
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
