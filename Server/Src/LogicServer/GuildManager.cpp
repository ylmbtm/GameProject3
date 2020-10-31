#include "stdafx.h"
#include "GuildManager.h"
#include "GameService.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../Message/Game_Define.pb.h"

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

BOOL CGuildManager::LoadData(CppMySQL3DB& tDBConnection)
{
	CppMySQLQuery QueryResult = tDBConnection.querySQL("SELECT * FROM guild");
	while(!QueryResult.eof())
	{
		CGuild* pGuild = new CGuild();
		pGuild->m_pGuildData = DataPool::CreateObject<GuildDataObject>(ESD_GUILD, FALSE);
		pGuild->m_pGuildData->m_uGuid = QueryResult.getInt64Field("id");
		pGuild->m_pGuildData->m_Level = QueryResult.getIntField("level");
		CommonConvert::StrCopy(pGuild->m_pGuildData->m_szName, QueryResult.getStringField("name"), GUILD_NAME_LEN);
		CommonConvert::StrCopy(pGuild->m_pGuildData->m_szNotice, QueryResult.getStringField("notice"), GUILD_NOTICE_LEN);
		m_mapGulidData.insert(std::make_pair(pGuild->m_pGuildData->m_uGuid, pGuild));
		QueryResult.nextRow();
	}

	CGuild* pCurGuild = NULL;
	QueryResult = tDBConnection.querySQL("SELECT * FROM guild_member order by guildid");
	while (!QueryResult.eof())
	{
		UINT64 uGuildId = QueryResult.getInt64Field("id");

		if (pCurGuild == NULL )
		{
			pCurGuild = GetGuildByID(uGuildId);
		}
		else if (pCurGuild->GetGuildID() != uGuildId)
		{
			pCurGuild = GetGuildByID(uGuildId);
		}

		pCurGuild->LoadGuildMember(QueryResult);

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
	pGuild->m_pGuildData = DataPool::CreateObject<GuildDataObject>(ESD_GUILD, TRUE);
	pGuild->m_pGuildData->Lock();
	pGuild->m_pGuildData->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	CommonConvert::StrCopy(pGuild->m_pGuildData->m_szName, strName.c_str(), GUILD_NAME_LEN);
	pGuild->m_pGuildData->Unlock();
	m_mapGulidData.insert(std::make_pair(pGuild->m_pGuildData->m_uGuid, pGuild));

	MemberDataObject* pMemberObj = DataPool::CreateObject<MemberDataObject>(ESD_GUILD_MEMBER, TRUE);
	pMemberObj->Lock();
	pMemberObj->m_uRoleID = uRoleID;
	pMemberObj->m_uJoinTime = CommonFunc::GetCurrTime();
	pMemberObj->m_uGuildID = pGuild->m_pGuildData->m_uGuid;
	pMemberObj->m_Pos = EGP_LEADER;
	pMemberObj->Unlock();
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
	ERROR_RETURN_NULL(pGuild != NULL);

	return pGuild->GetLeader();
}

