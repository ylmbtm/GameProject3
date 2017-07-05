#include <stdafx.h>
#include "SimpleMananger.h"
#include "..\GameServer\GameService.h"
#include "Sqlite\CppSQLite3.h"
#include "Utility\CommonFunc.h"

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

	CppSQLite3DB	DBConnection; 
	try
	{
		std::string strCurDir = CommonFunc::GetCurrentDir();
		strCurDir+= "\\GameData.db";
		DBConnection.open(strCurDir.c_str());
	}
	catch(CppSQLite3Exception& e)  
	{  
		printf("%s",e.errorMessage());  
		return FALSE;
	}  

	CppSQLite3Query TableNames = DBConnection.execQuery("SELECT * FROM player");
	while(!TableNames.eof())
	{
		CSimpleInfo *pInfo = CreateSimpleInfo(TableNames.getInt64Field("id"),
			TableNames.getInt64Field("account_id"), TableNames.getStringField("name"), TableNames.getInt64Field("roletype"));

		if(pInfo->u64RoleID > m_u64MaxID)
		{
			m_u64MaxID = pInfo->u64RoleID;
		}

		TableNames.nextRow();
	}
	//DBConnection.close();

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

BOOL CSimpleManager::CheckNameExist(std::string strName)
{
	std::map<std::string, UINT64>::iterator itor = m_mapName2ID.find(strName);
	if(itor != m_mapName2ID.end())
	{
		return TRUE;
	}
	
	return FALSE;
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

CSimpleInfo* CSimpleManager::CreateSimpleInfo(UINT64 u64ID, UINT64 u64AccID, std::string strName, UINT32 dwRoleType)
{
	CSimpleInfo *pInfo = new CSimpleInfo();
	pInfo->u64RoleID = u64ID;
	pInfo->u64AccountID = u64AccID;
	pInfo->Name = strName;
	pInfo->dwRoleType = dwRoleType;
	pInfo->IsOnline = TRUE;

	m_mapID2Simple.insert(std::make_pair(u64ID, pInfo));
	m_mapName2ID.insert(std::make_pair(strName, u64ID));

	return pInfo;
}
