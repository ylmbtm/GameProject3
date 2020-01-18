#include "stdafx.h"
#include "GlobalDataMgr.h"
#include "GameService.h"
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

BOOL CGlobalDataManager::LoadData(CppMySQL3DB& tDBConnection)
{
	UINT64 dwMaxGuid = 0;
	UINT32 dwMaxOnline = 0;

	CHAR szSql[SQL_BUFF_LEN] = { 0 };
	snprintf(szSql, SQL_BUFF_LEN, "select * from globaldata where serverid = %d", CGameService::GetInstancePtr()->GetServerID());

	CppMySQLQuery QueryResult = tDBConnection.querySQL(szSql);
	if(!QueryResult.eof())
	{
		dwMaxGuid = QueryResult.getInt64Field("maxguid");
		dwMaxOnline = QueryResult.getIntField("maxonline");
	}

	if(dwMaxGuid == 0)
	{
		dwMaxGuid  =  CGameService::GetInstancePtr()->GetServerID();
		dwMaxGuid = (dwMaxGuid << 48) + 1;
	}
	dwMaxGuid += 100;
	m_pGlobalDataObject = DataPool::CreateObject<GlobalDataObject>(ESD_GLOBAL, FALSE);
	m_pGlobalDataObject->m_dwServerID = CGameService::GetInstancePtr()->GetServerID();
	m_pGlobalDataObject->m_u64Guid	  = dwMaxGuid;
	m_pGlobalDataObject->m_dwMaxOnline = dwMaxOnline;

	return TRUE;
}


UINT64 CGlobalDataManager::MakeNewGuid()
{
	m_pGlobalDataObject->Lock();
	m_pGlobalDataObject->m_u64Guid	  += 1;
	m_pGlobalDataObject->Unlock();
	return m_pGlobalDataObject->m_u64Guid;
}

UINT32 CGlobalDataManager::GetMaxOnline()
{
	return m_pGlobalDataObject->m_dwMaxOnline;
}
