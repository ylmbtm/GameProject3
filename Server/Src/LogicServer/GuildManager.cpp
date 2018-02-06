#include "stdafx.h"
#include "GuildManager.h"
#include "GameService.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

CGuildManager::CGuildManager()
{

}

CGuildManager::~CGuildManager()
{

}

CGuildManager* CGuildManager::GetInstancePtr()
{
	static CGuildManager _StaticMgr;

	return &_StaticMgr;
}

BOOL CGuildManager::LoadAllGuildData(CppMySQL3DB& tDBConnection)
{
	CppMySQLQuery QueryResult = tDBConnection.querySQL("SELECT * FROM guild");
	while(!QueryResult.eof())
	{
		CGuild* pGuid = new CGuild();
		pGuid->m_pGuildData = g_pGuildDataObjectPool->NewObject(FALSE);
		pGuid->m_pGuildData->m_uGuid = QueryResult.getInt64Field("id");
		m_mapGulidData.insert(std::make_pair(pGuid->m_pGuildData->m_uGuid, pGuid));
		QueryResult.nextRow();
	}

	return TRUE;
}

CGuild* CGuildManager::GetGuildByID( UINT64 u64ID )
{
	std::map<UINT64, CGuild*>::iterator itor =  m_mapGulidData.find(u64ID);
	if (itor != m_mapGulidData.end())
	{
		return itor->second;
	}

	return NULL;
}

CGuild* CGuildManager::CreateGuild(UINT64 uRoleID, std::string& strName, INT32 nIcon)
{
	CGuild* pGuild = new CGuild();
	pGuild->m_pGuildData = g_pGuildDataObjectPool->NewObject(TRUE);
	pGuild->m_pGuildData->lock();
	pGuild->m_pGuildData->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pGuild->m_pGuildData->unlock();

	MemberDataObject* pMemberObj = g_pMemberDataObjectPool->NewObject(TRUE);
	pMemberObj->lock();


	pMemberObj->unlock();
	pGuild->m_mapMemberData.insert(std::make_pair(uRoleID, pMemberObj));
	return pGuild;
}

BOOL CGuildManager::RemoveGuild(UINT64 uGuildID)
{
	return TRUE;
}

CGuild* CGuildManager::GetGuildByName(std::string strName)
{
	for(auto itor = m_mapGulidData.begin(); itor != m_mapGulidData.end(); itor++)
	{
		CGuild* pGuild  = itor->second;
		if(pGuild->GetGuildName() == strName)
		{
			return pGuild;
		}
	}

	return NULL;
}

MemberDataObject* CGuildManager::GetGuildLeader(UINT64 uGuildID)
{
	auto itor = m_mapGulidData.find(uGuildID);
	if(itor == m_mapGulidData.end())
	{
		return NULL;
	}

	CGuild* pGuild = itor->second;

	return pGuild->GetLeader();
}

