#ifndef __SIMPLE_MANAGER_H__
#define __SIMPLE_MANAGER_H__

struct CSimpleInfo {
    UINT64 u64RoleID; //玩家ID
    UINT64 u64AccountID; //玩家ID
	UINT32 dwGuildID;  //公会ID
	UINT32 dwRoleType; //角色类
    std::string Name ;//玩家名字
    UINT32 dwLevel; //玩家等级
    UINT32 dwVipLevel;    //玩家的VIP等级
	UINT32 dwFightValue; //玩家的战力
    UINT32 dwLogoffTime; //离线时间
    UINT32 dwLoginDay; //登录日期
    BOOL IsOnline;   //是否在线
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

    UINT32 Get_GuildID(UINT64 u64ID);

    UINT64 MakeNewRoleID();

public:
    std::map<UINT64, CSimpleInfo*> m_mapID2Simple;
    std::map<std::string, UINT64>  m_mapName2ID;
    UINT64 m_u64MaxID;
};
#endif