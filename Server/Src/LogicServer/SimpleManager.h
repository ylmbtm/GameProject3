#ifndef __SIMPLE_MANAGER_H__
#define __SIMPLE_MANAGER_H__
#include "DBInterface/CppMysql.h"
struct CSimpleInfo
{
	CSimpleInfo()
	{
		m_uRoleID		= 0;
		m_uAccountID	= 0;
		m_uGuildID		= 0;
		m_dwCarrerID	= 0;
		m_dwLevel		= 0;
		m_dwVipLevel	= 0;
		m_dwFightValue	= 0;
		m_uLogoffTime	= 0;
		m_uLogonTime	= 0;
		m_uCreateTime	= 0;
		m_bOnline		= FALSE;
		m_strName		= "";
	}
	UINT64	m_uRoleID;
	UINT64	m_uAccountID;
	UINT64	m_uGuildID;
	UINT32	m_dwCarrerID;
	UINT32	m_dwLevel;
	UINT32	m_dwVipLevel;
	UINT32	m_dwFightValue;
	UINT64	m_uLogoffTime;
	UINT64	m_uLogonTime;
	UINT64	m_uCreateTime;
	BOOL	m_bOnline;
	std::string m_strName;
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

	BOOL	LoadData(CppMySQL3DB& tDBConnection);

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

	UINT32  GetTotalCount();

	UINT32  GetOnline();

	BOOL	SetOnline(UINT64 u64ID, BOOL bOnline);

	BOOL	GetRoleIDsByAccountID(UINT64 uAccountID, std::vector<UINT64>& vtRoleIDs);

public:
	std::map<UINT64, CSimpleInfo*> m_mapID2Simple;

	std::map<std::string, UINT64>  m_mapName2ID;

};
#endif