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
    m_pGlobalDataObject = DataPool::CreateObject<GlobalDataObject>(ESD_GLOBAL, FALSE);
    m_pGlobalDataObject->Lock();
    m_pGlobalDataObject->m_dwServerID = CGameService::GetInstancePtr()->GetServerID();

    CHAR szSql[SQL_BUFF_LEN] = { 0 };
    snprintf(szSql, SQL_BUFF_LEN, "select * from globaldata where serverid = %d", CGameService::GetInstancePtr()->GetServerID());

    CppMySQLQuery QueryResult = tDBConnection.querySQL(szSql);
    if(!QueryResult.eof())
    {
        m_pGlobalDataObject->m_u64Guid = QueryResult.getInt64Field("maxguid");
        m_pGlobalDataObject->m_dwMaxOnline = QueryResult.getIntField("maxonline");
        int nLen = 0;
        const unsigned char* pData = QueryResult.getBlobField("exdata", nLen);
        memcpy((void*)&m_pGlobalDataObject->m_exData, (void*)pData, nLen);
    }

    if(m_pGlobalDataObject->m_u64Guid <= 0)
    {
        m_pGlobalDataObject->m_u64Guid =  CGameService::GetInstancePtr()->GetServerID();
        m_pGlobalDataObject->m_u64Guid = (m_pGlobalDataObject->m_u64Guid << 48) + 1;
    }

    m_pGlobalDataObject->m_u64Guid += 100;

    m_pGlobalDataObject->Unlock();

    return TRUE;
}


UINT64 CGlobalDataManager::MakeNewGuid()
{
    m_pGlobalDataObject->Lock();
    m_pGlobalDataObject->m_u64Guid    += 1;
    m_pGlobalDataObject->Unlock();
    return m_pGlobalDataObject->m_u64Guid;
}

VOID CGlobalDataManager::SetMaxOnline(INT32 nNum)
{
    m_pGlobalDataObject->Lock();
    m_pGlobalDataObject->m_dwMaxOnline = nNum;
    m_pGlobalDataObject->Unlock();
}

INT32 CGlobalDataManager::GetMaxOnline()
{
    return m_pGlobalDataObject->m_dwMaxOnline;
}

BOOL CGlobalDataManager::SetExData(INT32 nIndex, INT32 dwData)
{
    ERROR_RETURN_FALSE(nIndex > 0);
    ERROR_RETURN_FALSE(nIndex < MAX_EXTRA_INDEX);

    m_pGlobalDataObject->Lock();
    m_pGlobalDataObject->m_exData[nIndex - 1] = dwData;
    m_pGlobalDataObject->Unlock();

    return TRUE;
}

INT32 CGlobalDataManager::GetExData(INT32 nIndex)
{
    ERROR_RETURN_VALUE(nIndex > 0, 0);
    ERROR_RETURN_VALUE(nIndex < MAX_EXTRA_INDEX, 0);

    return m_pGlobalDataObject->m_exData[nIndex - 1];
}
