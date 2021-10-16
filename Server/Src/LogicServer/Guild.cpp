#include "stdafx.h"
#include "DBInterface/CppMysql.h"
#include "Guild.h"
#include "GameService.h"
#include "DataPool.h"
#include "../Message/Game_Define.pb.h"

CGuild::CGuild()
{
	m_pGuildData = NULL;
	m_u64LeaderID = 0;
}

CGuild::~CGuild()
{
	if(m_pGuildData != NULL)
	{
		m_pGuildData->Release();
	}

	m_pGuildData = NULL;
}

BOOL CGuild::Init()
{
	return TRUE;
}

BOOL CGuild::LoadGuildMember(CppMySQLQuery& QueryResult)
{
	UINT64 uRoleID = QueryResult.getInt64Field("roleid");

	MemberDataObject* pMemberObject = DataPool::CreateObject<MemberDataObject>(ESD_GUILD_MEMBER, FALSE);
	pMemberObject->m_uRoleID = uRoleID;
	pMemberObject->m_uGuildID = m_pGuildData->m_uGuid;
	pMemberObject->m_Pos = QueryResult.getIntField("pose");
	pMemberObject->m_uJoinTime = QueryResult.getInt64Field("join_time");

	m_mapMemberData.insert(std::make_pair(uRoleID, pMemberObject));

	if (pMemberObject->m_Pos == EGP_LEADER)
	{
		m_u64LeaderID = uRoleID;
	}

	return TRUE;
}

CHAR* CGuild::GetGuildName()
{
	return m_pGuildData->m_szName;
}

UINT64 CGuild::GetGuildID()
{
	return m_pGuildData->m_uGuid;
}

MemberDataObject* CGuild::GetGuildMember(UINT64 uID)
{
	auto itor = m_mapMemberData.find(uID);
	if(itor == m_mapMemberData.end())
	{
		return NULL;
	}

	return itor->second;
}

MemberDataObject* CGuild::GetLeader()
{
	if (m_u64LeaderID == 0)
	{
		return NULL;
	}

	auto itor = m_mapMemberData.find(m_u64LeaderID);
	if (itor == m_mapMemberData.end())
	{
		return NULL;
	}

	return itor->second;
}

MemberDataObject* CGuild::AddGuildMember(UINT64 uRoleID)
{
	MemberDataObject* pMemberObject = DataPool::CreateObject<MemberDataObject>(ESD_GUILD_MEMBER, TRUE);
	pMemberObject->Lock();
	pMemberObject->m_uRoleID = uRoleID;
	pMemberObject->m_uGuildID = m_pGuildData->m_uGuid;
	pMemberObject->m_Pos = EGP_MEMBER;
	pMemberObject->m_uJoinTime = CommonFunc::GetCurrTime();
	pMemberObject->Unlock();

	m_mapMemberData.insert(std::make_pair(uRoleID, pMemberObject));

	return pMemberObject;
}

BOOL CGuild::BroadMessageToAll(INT32 nMsgID, const google::protobuf::Message& pdata)
{
	for (auto itor = m_mapMemberData.begin(); itor != m_mapMemberData.end(); itor++)
	{
		MemberDataObject* pMemberObject = itor->second;
		ERROR_TO_CONTINUE(pMemberObject != NULL);

		CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pMemberObject->m_uRoleID);
		ERROR_TO_CONTINUE(pPlayer != NULL);

		if (!pPlayer->IsOnline())
		{
			continue;
		}

		pPlayer->SendMsgProtoBuf(nMsgID, pdata);
	}

	return TRUE;
}
