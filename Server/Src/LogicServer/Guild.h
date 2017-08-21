#ifndef __GUILD_HEADER_H__
#define __GUILD_HEADER_H__
#include "../ServerData/GuildData.h"

class CGuild
{
public:
	CGuild();
	~CGuild();
public:
	BOOL Init();

	BOOL LoadGuildData();

	std::string& GetGuildName();

public:
	MemberDataObject* GetGuidMember(UINT64 uID);

	MemberDataObject* GetLeader();

public:


	GuildDataObject* m_pGuildData;

	std::map<UINT64, MemberDataObject*> m_mapMemberData;

};
#endif