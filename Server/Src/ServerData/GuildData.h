#ifndef __GUILD_DATA_OBJECT_H__
#define __GUILD_DATA_OBJECT_H__

#include "DBInterface/DBInterface.h"

struct GuildDataObject : public ShareObject
{
	GuildDataObject()
	{
		m_uGuid = 0;
		m_Level = 0;
		memset(m_szName, 0, GUILD_NAME_LEN);
		memset(m_szNotice, 0, GUILD_NOTICE_LEN);
	}

	UINT64 m_uGuid;			//格子的guid
	CHAR m_szName[GUILD_NAME_LEN];
	CHAR m_szNotice[GUILD_NOTICE_LEN];
	INT32  m_Level;

	BOOL Create(IDBInterface* pDB)
	{
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{


		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{
		return TRUE;
	}
};


struct MemberDataObject : public ShareObject
{
	MemberDataObject()
	{

	}

	UINT64 m_uRoleID;
	UINT64 m_uGuildID;			//公会的guid
	UINT32 m_Pos;
	UINT64 m_dwJoinTime;

	BOOL Create(IDBInterface* pDB)
	{
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{


		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{
		return TRUE;
	}
};

#endif //__GUILD_DATA_OBJECT_H__