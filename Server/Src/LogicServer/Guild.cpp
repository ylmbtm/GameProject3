#include <stdafx.h>
#include "Guild.h"
#include "GameService.h"
#include "CommonFunc.h"
#include "Log.h"
#include "DataPool.h"

CGuild::CGuild()
{
	m_pGuildData = NULL;
}

CGuild::~CGuild()
{
	if(m_pGuildData != NULL)
	{
		m_pGuildData->release();
	}
}

BOOL CGuild::Init()
{
	return TRUE;
}

BOOL CGuild::LoadGuildData()
{
	return TRUE;
}

CHAR* CGuild::GetGuildName()
{
	return m_pGuildData->m_szName;
}

MemberDataObject* CGuild::GetGuidMember(UINT64 uID)
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
	return NULL;
}
