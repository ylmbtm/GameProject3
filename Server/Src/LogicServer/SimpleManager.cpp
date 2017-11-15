#include "stdafx.h"
#include "SimpleManager.h"
#include "GameService.h"
#include "CommonFunc.h"
#include "Log.h"

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

BOOL CSimpleManager::LoadSimpleData(CppMySQL3DB& tDBConnection)
{
	CppMySQLQuery QueryResult = tDBConnection.querySQL("SELECT * FROM player");
	while(!QueryResult.eof())
	{
		CSimpleInfo* pInfo	= new CSimpleInfo();
		pInfo->u64RoleID	= QueryResult.getInt64Field("id");
		pInfo->u64AccountID = QueryResult.getInt64Field("account_id");
		pInfo->Name			= QueryResult.getStringField("name");
		pInfo->dwCarrerID	= QueryResult.getIntField("carrerid");
		pInfo->uCreateTime	= QueryResult.getInt64Field("createtime");
		pInfo->uLogonTime	= QueryResult.getInt64Field("logontime");
		pInfo->uLogoffTime	= QueryResult.getInt64Field("logofftime");
		pInfo->uGuildID		= QueryResult.getInt64Field("guildid");
		pInfo->dwLevel		= QueryResult.getInt64Field("level");
		pInfo->dwVipLevel	= QueryResult.getInt64Field("viplevel");

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

	return pInfo->uCreateTime;
}

UINT64 CSimpleManager::GetLogonTime(UINT64 u64ID)
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->uLogonTime;
}

UINT64 CSimpleManager::GetLogoffTime( UINT64 u64ID )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->uLogoffTime;
}

UINT32 CSimpleManager::GetFightValue( UINT64 u64ID )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->dwFightValue;
}

BOOL CSimpleManager::SetFightValue( UINT64 u64ID, UINT32 dwFight, UINT32 dwLevel )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->dwFightValue = dwFight;
	pInfo->dwLevel = dwLevel;

	return TRUE;
}

BOOL CSimpleManager::SetPlayerName( UINT64 u64ID, std::string strName )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->Name = strName;
	return TRUE;
}

BOOL CSimpleManager::SetLogoffTime( UINT64 u64ID, UINT64 uTime )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->uLogoffTime = uTime;

	return TRUE;
}

BOOL CSimpleManager::SetCreateTime(UINT64 u64ID, UINT64 uTime)
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->uCreateTime = uTime;

	return TRUE;
}

BOOL CSimpleManager::SetLogonTime(UINT64 u64ID, UINT64 uTime)
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->uLogonTime = uTime;

	return TRUE;
}

BOOL CSimpleManager::SetVipLevel( UINT64 u64ID, UINT32 dwVipLvl )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->dwVipLevel = dwVipLvl;

	return TRUE;
}

BOOL CSimpleManager::SetGuildID( UINT64 u64ID, UINT64 guildid )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->uGuildID = guildid;

	return TRUE;
}

BOOL CSimpleManager::CheckNameExist(std::string strName)
{
	std::map<std::string, UINT64>::iterator itor = m_mapName2ID.find(strName);
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
	return  pInfo->uGuildID;
}

CSimpleInfo* CSimpleManager::CreateSimpleInfo( UINT64 u64ID, UINT64 u64AccID, std::string strName, UINT32 dwCarrerID)
{
	CSimpleInfo* pInfo = new CSimpleInfo();
	pInfo->u64RoleID = u64ID;
	pInfo->u64AccountID = u64AccID;
	pInfo->Name = strName;
	pInfo->dwCarrerID = dwCarrerID;
	pInfo->IsOnline = TRUE;
	pInfo->uCreateTime = CommonFunc::GetCurrTime();
	pInfo->dwVipLevel = 0;
	pInfo->dwLevel = 0;
	pInfo->dwFightValue = 0;

	m_mapID2Simple.insert(std::make_pair(u64ID, pInfo));

	m_mapName2ID.insert(std::make_pair(strName, u64ID));

	return pInfo;
}

BOOL CSimpleManager::AddSimpleInfo(CSimpleInfo* pInfo)
{
	ERROR_RETURN_FALSE(pInfo != NULL);

	m_mapID2Simple.insert(std::make_pair(pInfo->u64RoleID, pInfo));

	m_mapName2ID.insert(std::make_pair(pInfo->Name, pInfo->u64RoleID));

	return TRUE;
}
