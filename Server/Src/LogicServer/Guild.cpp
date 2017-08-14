#include <stdafx.h>
#include "Guild.h"
#include "..\GameServer\GameService.h"
#include "Sqlite\CppSQLite3.h"
#include "Utility\CommonFunc.h"
#include "Utility\Log\Log.h"
#include "DataPool.h"

CGuild::CGuild()
{
	m_pGuildData = NULL;
}

CGuild::~CGuild()
{

}

BOOL CGuild::Init()
{
	return TRUE;
}

BOOL CGuild::LoadGuildData()
{
	return TRUE;
}

std::string& CGuild::GetGuildName()
{
	return m_pGuildData->m_strName;
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
