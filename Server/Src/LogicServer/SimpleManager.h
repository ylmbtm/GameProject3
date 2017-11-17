#ifndef __SIMPLE_MANAGER_H__
#define __SIMPLE_MANAGER_H__
#include "DBInterface/CppMysql.h"
struct CSimpleInfo
{
	CSimpleInfo()
	{
		u64RoleID		= 0;
		u64AccountID	= 0;
		uGuildID		= 0;
		dwCarrerID		= 0;
		dwLevel			= 0;
		dwVipLevel		= 0;
		dwFightValue	= 0;
		uLogoffTime		= 0;
		uLogonTime		= 0;
		uCreateTime		= 0;
		IsOnline		= FALSE;
	}
	UINT64	u64RoleID;
	UINT64	u64AccountID;
	UINT64	uGuildID;
	UINT32	dwCarrerID;
	std::string Name ;
	UINT32	dwLevel;
	UINT32	dwVipLevel;
	UINT32	dwFightValue;
	UINT64	uLogoffTime;
	UINT64	uLogonTime;
	UINT64	uCreateTime;
	BOOL	IsOnline;
};

class CSimpleManager
{
	CSimpleManager();
	~CSimpleManager();
public:
	static CSimpleManager* GetInstancePtr();

	CSimpleInfo* GetSimpleInfoByID(UINT64 u64ID);

	CSimpleInfo* CreateSimpleInfo(UINT64 u64ID, UINT64 u64AccID, std::string strName, UINT32 dwCarrerID);

	BOOL	AddSimpleInfo(CSimpleInfo* pInfo);

	BOOL	LoadSimpleData(CppMySQL3DB& tDBConnection);

	UINT64	GetRoleIDByName(std::string Name);

	UINT64	GetCreateTime(UINT64 u64ID);
	UINT64	GetLogonTime(UINT64 u64ID);
	UINT64	GetLogoffTime(UINT64 u64ID);

	BOOL	SetCreateTime(UINT64 u64ID, UINT64 dwTime);
	BOOL	SetLogonTime(UINT64 u64ID, UINT64 dwTime);
	BOOL	SetLogoffTime(UINT64 u64ID, UINT64 dwTime);

	UINT32	GetFightValue(UINT64 u64ID);

	BOOL	SetFightValue(UINT64 u64ID, UINT32 dwFight, UINT32 dwLevel);

	BOOL	SetPlayerName(UINT64 u64ID, std::string strName);

	BOOL	SetVipLevel(UINT64 u64ID, UINT32 dwVipLvl);

	BOOL	SetGuildID(UINT64 u64ID, UINT64 guildid);

	BOOL	CheckNameExist(std::string strName);

	UINT64	GetGuildID(UINT64 u64ID);

public:
	std::map<UINT64, CSimpleInfo*> m_mapID2Simple;

	std::map<std::string, UINT64>  m_mapName2ID;

};
#endif