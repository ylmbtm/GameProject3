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
		m_uFightValue	= 0;
		m_uLogoffTime	= 0;
		m_uLogonTime	= 0;
		m_uCreateTime	= 0;
		m_strName		= "";
		m_IsDelete = FALSE;
	}
	UINT64	m_uRoleID;
	UINT64	m_uAccountID;
	UINT64	m_uGuildID;
	UINT32	m_dwCarrerID;
	UINT32	m_dwLevel;
	UINT32	m_dwVipLevel;
	UINT64	m_uFightValue;
	UINT64	m_uLogoffTime;
	UINT64	m_uLogonTime;
	UINT64	m_uCreateTime;
	std::string m_strName;
	BOOL    m_IsDelete; //是否被删除
	UINT32  m_nLogonStatus;  //0:表示正常，1:表示正在db服取数据
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

	UINT64	GetFightValue(UINT64 u64ID);

	BOOL	SetFightValue(UINT64 u64ID, UINT64 uFight, UINT32 dwLevel);

	BOOL	SetName(UINT64 u64ID, std::string strName);

	BOOL	SetVipLevel(UINT64 u64ID, UINT32 dwVipLvl);

	BOOL	SetGuildID(UINT64 u64ID, UINT64 guildid);

	BOOL	SetRoleDelete(UINT64 u64ID, BOOL bDelete);

	BOOL	CheckNameExist(std::string strName);

	BOOL    CheckNameFormat(std::string strName);

	UINT64	GetGuildID(UINT64 u64ID);

	UINT32  GetTotalCount();

	BOOL	GetRoleIDsByAccountID(UINT64 uAccountID, std::vector<UINT64>& vtRoleIDs);

public:
	std::map<UINT64, CSimpleInfo*> m_mapID2Simple;

	std::map<std::string, UINT64>  m_mapName2ID;

};
#endif