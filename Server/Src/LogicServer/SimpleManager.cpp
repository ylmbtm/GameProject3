#include "stdafx.h"
#include "SimpleManager.h"
#include "GameService.h"

CSimpleManager::CSimpleManager()
{

}

CSimpleManager::~CSimpleManager()
{

}

CSimpleManager* CSimpleManager::GetInstancePtr()
{
	static CSimpleManager _StaticMgr;

	return &_StaticMgr;
}

BOOL CSimpleManager::LoadData(CppMySQL3DB& tDBConnection)
{
	CppMySQLQuery QueryResult = tDBConnection.querySQL("SELECT * FROM player");
	while(!QueryResult.eof())
	{
		CSimpleInfo* pInfo	= new CSimpleInfo();
		pInfo->m_uRoleID	= QueryResult.getInt64Field("id");
		pInfo->m_uAccountID = QueryResult.getInt64Field("account_id");
		pInfo->m_strName	= QueryResult.getStringField("name");
		pInfo->m_dwCarrerID	= QueryResult.getIntField("carrerid");
		pInfo->m_uCreateTime = QueryResult.getInt64Field("createtime");
		pInfo->m_uLogonTime	= QueryResult.getInt64Field("logontime");
		pInfo->m_uLogoffTime = QueryResult.getInt64Field("logofftime");
		pInfo->m_uGuildID	= QueryResult.getInt64Field("guildid");
		pInfo->m_dwLevel	= QueryResult.getIntField("level");
		pInfo->m_dwVipLevel	= QueryResult.getIntField("viplevel");

		AddSimpleInfo(pInfo);

		QueryResult.nextRow();
	}

	return TRUE;
}

UINT64 CSimpleManager::GetRoleIDByName( std::string Name )
{
	std::map<std::string, UINT64>::iterator itor = m_mapName2ID.find(Name);
	if(itor != m_mapName2ID.end())
	{
		return itor->second;
	}

	return 0;
}

CSimpleInfo* CSimpleManager::GetSimpleInfoByID( UINT64 u64ID )
{
	std::map<UINT64, CSimpleInfo*>::iterator itor =  m_mapID2Simple.find(u64ID);
	if (itor != m_mapID2Simple.end())
	{
		return itor->second;
	}

	return NULL;
}

UINT64 CSimpleManager::GetCreateTime(UINT64 u64ID)
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->m_uCreateTime;
}

UINT64 CSimpleManager::GetLogonTime(UINT64 u64ID)
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->m_uLogonTime;
}

UINT64 CSimpleManager::GetLogoffTime( UINT64 u64ID )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->m_uLogoffTime;
}

UINT32 CSimpleManager::GetFightValue( UINT64 u64ID )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->m_dwFightValue;
}

BOOL CSimpleManager::SetFightValue( UINT64 u64ID, UINT32 dwFight, UINT32 dwLevel )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->m_dwFightValue = dwFight;
	pInfo->m_dwLevel = dwLevel;

	return TRUE;
}

BOOL CSimpleManager::SetPlayerName( UINT64 u64ID, std::string strName )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->m_strName = strName;
	return TRUE;
}

BOOL CSimpleManager::SetLogoffTime( UINT64 u64ID, UINT64 uTime )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->m_uLogoffTime = uTime;

	return TRUE;
}

BOOL CSimpleManager::SetCreateTime(UINT64 u64ID, UINT64 uTime)
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->m_uCreateTime = uTime;

	return TRUE;
}

BOOL CSimpleManager::SetLogonTime(UINT64 u64ID, UINT64 uTime)
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->m_uLogonTime = uTime;

	return TRUE;
}

BOOL CSimpleManager::SetVipLevel( UINT64 u64ID, UINT32 dwVipLvl )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->m_dwVipLevel = dwVipLvl;

	return TRUE;
}

BOOL CSimpleManager::SetGuildID( UINT64 u64ID, UINT64 guildid )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->m_uGuildID = guildid;

	return TRUE;
}

BOOL CSimpleManager::CheckNameExist(std::string strName)
{
	auto itor = m_mapName2ID.find(strName);
	if(itor != m_mapName2ID.end())
	{
		return TRUE;
	}

	return FALSE;
}

UINT64 CSimpleManager::GetGuildID( UINT64 u64ID )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);
	return  pInfo->m_uGuildID;
}

UINT32 CSimpleManager::GetTotalCount()
{
	return (UINT32)m_mapID2Simple.size();
}

BOOL CSimpleManager::GetRoleIDsByAccountID(UINT64 uAccountID, std::vector<UINT64>& vtRoleIDs)
{
	for (auto itor = m_mapID2Simple.begin(); itor != m_mapID2Simple.end(); ++itor)
	{
		CSimpleInfo* pInfo = itor->second;
		ERROR_RETURN_FALSE(pInfo != NULL);

		if (pInfo->m_uAccountID == uAccountID)
		{
			vtRoleIDs.push_back(pInfo->m_uRoleID);
		}
	}

	return TRUE;
}

CSimpleInfo* CSimpleManager::CreateSimpleInfo( UINT64 u64ID, UINT64 u64AccID, std::string strName, UINT32 dwCarrerID)
{
	CSimpleInfo* pInfo = new CSimpleInfo();
	pInfo->m_uRoleID = u64ID;
	pInfo->m_uAccountID = u64AccID;
	pInfo->m_strName = strName;
	pInfo->m_dwCarrerID = dwCarrerID;
	pInfo->m_uCreateTime = CommonFunc::GetCurrTime();
	pInfo->m_dwVipLevel = 0;
	pInfo->m_dwLevel = 0;
	pInfo->m_dwFightValue = 0;

	m_mapID2Simple.insert(std::make_pair(u64ID, pInfo));

	m_mapName2ID.insert(std::make_pair(strName, u64ID));

	return pInfo;
}

BOOL CSimpleManager::AddSimpleInfo(CSimpleInfo* pInfo)
{
	ERROR_RETURN_FALSE(pInfo != NULL);

	m_mapID2Simple.insert(std::make_pair(pInfo->m_uRoleID, pInfo));

	m_mapName2ID.insert(std::make_pair(pInfo->m_strName, pInfo->m_uRoleID));

	return TRUE;
}
