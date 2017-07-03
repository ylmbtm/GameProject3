#include <stdafx.h>
#include "SimpleMananger.h"
#include "..\GameServer\GameService.h"

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

BOOL CSimpleManager::LoadSimpleData()
{
    //以下是默认值
    m_u64MaxID = CGameService::GetInstancePtr()->GetServerID();
    m_u64MaxID = m_u64MaxID << 32 +1;

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

UINT32 CSimpleManager::GetPlayerLogoffTime( UINT64 u64ID )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo != NULL)
    {
        return pInfo->dwLogoffTime;
    }
    return 0;
}

UINT32 CSimpleManager::Get_FightValue( UINT64 u64ID )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo != NULL)
    {
        return pInfo->dwFightValue;
    }
    return 0;
}

BOOL CSimpleManager::Set_FightValue( UINT64 u64ID, UINT32 dwFight, UINT32 dwLevel )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo == NULL)
    {
        return FALSE;
    }

    pInfo->dwFightValue = dwFight;
    pInfo->dwLevel = dwLevel;

    return TRUE;
}

BOOL CSimpleManager::Set_PlayerName( UINT64 u64ID, std::string strName )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo == NULL)
    {
        return FALSE;
    }

    pInfo->Name = strName;
    return TRUE;
}

BOOL CSimpleManager::Set_LogoffTime( UINT64 u64ID, UINT32 dwTime )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo == NULL)
    {
        return FALSE;
    }

    pInfo->dwLogoffTime = dwTime;

    return TRUE;
}

BOOL CSimpleManager::Set_VipLevel( UINT64 u64ID, UINT32 dwVipLvl )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo == NULL)
    {
        return FALSE;
    }

    pInfo->dwVipLevel = dwVipLvl;

    return TRUE;
}

BOOL CSimpleManager::Set_LoginDay( UINT64 u64ID, UINT32 dwDay )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo == NULL)
    {
        return FALSE;
    }

    pInfo->dwLoginDay = dwDay;

    return TRUE;
}

BOOL CSimpleManager::Set_GuildID( UINT64 u64ID, UINT32 guildid )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo == NULL)
    {
        return FALSE;
    }

    pInfo->dwGuildID = guildid;

    return TRUE;
}

UINT32 CSimpleManager::Get_GuildID( UINT64 u64ID )
{
    CSimpleInfo *pInfo = GetSimpleInfoByID(u64ID);
    if(pInfo == NULL)
    {
        return 0;
    }
    return  pInfo->dwGuildID;
}

UINT64 CSimpleManager::MakeNewRoleID()
{
    return m_u64MaxID++;
}
