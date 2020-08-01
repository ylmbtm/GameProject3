#ifndef __GUILD_MANAGER_H__
#define __GUILD_MANAGER_H__

#include "DBInterface/CppMysql.h"
#include "Guild.h"

class CGuildManager
{
	CGuildManager();
	~CGuildManager();
public:
	static CGuildManager* GetInstancePtr();

	BOOL    LoadData(CppMySQL3DB& tDBConnection);

	CGuild* GetGuildByID(UINT64 u64ID);

	CGuild* CreateGuild(UINT64 uRoleID, std::string& strName, INT32 nIcon);

	BOOL	RemoveGuild(UINT64 uGuildID);

	CGuild* GetGuildByName(std::string strName);

	MemberDataObject* GetGuildLeader(UINT64 uGuildID);

public:
	std::map<UINT64, CGuild*> m_mapGulidData;

};
#endif