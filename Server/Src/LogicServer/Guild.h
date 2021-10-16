#ifndef __GUILD_HEADER_H__
#define __GUILD_HEADER_H__
#include "GuildData.h"

class CGuild
{
public:
	CGuild();
	~CGuild();
public:
	BOOL Init();

	BOOL LoadGuildMember(CppMySQLQuery& QueryResult);

	CHAR* GetGuildName();

	UINT64 GetGuildID();

	MemberDataObject* GetGuildMember(UINT64 uID);

	MemberDataObject* GetLeader();

	MemberDataObject* AddGuildMember(UINT64 uRoleID);

	BOOL BroadMessageToAll(INT32 nMsgID, const google::protobuf::Message& pdata);

public:

	UINT64		m_u64LeaderID;

	GuildDataObject* m_pGuildData;

	std::map<UINT64, MemberDataObject*> m_mapMemberData;

};
#endif