#ifndef __SIMPLE_MANAGER_H__
#define __SIMPLE_MANAGER_H__

struct CSimpleInfo {
	UINT64 u64RoleID;	
	UINT64 u64AccountID;	
	UINT32 dwGuildID;	
	UINT32 dwActorID;	
	std::string Name ;	
	UINT32 dwLevel;	
	UINT32 dwVipLevel;	
	UINT32 dwFightValue;	
	UINT32 dwLogoffTime;	
	UINT32 dwLoginDay;	
	BOOL IsOnline;	
};

class CSimpleManager 
{
    CSimpleManager();
    ~CSimpleManager();
public:
    static CSimpleManager* GetInstancePtr();

    BOOL LoadSimpleData();

    CSimpleInfo* GetSimpleInfoByID(UINT64 u64ID);

    UINT64 GetRoleIDByName(std::string Name);

    UINT32 GetPlayerLogoffTime(UINT64 u64ID);

    UINT32 Get_FightValue(UINT64 u64ID);

    BOOL Set_FightValue(UINT64 u64ID, UINT32 dwFight, UINT32 dwLevel);

    BOOL Set_PlayerName(UINT64 u64ID, std::string strName);

    BOOL Set_LogoffTime(UINT64 u64ID, UINT32 dwTime);

    BOOL Set_VipLevel(UINT64 u64ID, UINT32 dwVipLvl);

    BOOL Set_LoginDay(UINT64 u64ID, UINT32 dwDay);

    BOOL Set_GuildID(UINT64 u64ID, UINT32 guildid);

	BOOL CheckNameExist(std::string strName);

    UINT32 Get_GuildID(UINT64 u64ID);

    CSimpleInfo* CreateSimpleInfo(UINT64 u64ID, UINT64 u64AccID, std::string strName, UINT32 dwActorID);

public:
    std::map<UINT64, CSimpleInfo*> m_mapID2Simple;
    std::map<std::string, UINT64>  m_mapName2ID;

};
#endif