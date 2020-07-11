#ifndef __GUILD_DATA_OBJECT_H__
#define __GUILD_DATA_OBJECT_H__

#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"

struct GuildDataObject : public ShareObject
{
	GuildDataObject()
	{
		m_uGuid = 0;
		m_Level = 0;
		m_uCreateTime = 0;
		memset(m_szName, 0, GUILD_NAME_LEN);
		memset(m_szNotice, 0, GUILD_NOTICE_LEN);
	}

	INT32   m_Level;                            //等级
	UINT64  m_uGuid;			                //guid
	CHAR    m_szName[GUILD_NAME_LEN];           //名字
	CHAR    m_szNotice[GUILD_NOTICE_LEN];       //公告
	UINT64  m_ApplyRoles[GUILD_MAX_APPLY_NUM];  //申请id
	UINT64  m_uCreateTime;

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO guild (id, name, level, notice, create_time) \
			VALUES(?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_string(1, m_szName, strlen(m_szName));
		csp.set_int32(2, m_Level);
		csp.set_string(3, m_szNotice, strlen(m_szNotice));
		csp.set_uint64(4, m_uCreateTime);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO guild (id, name, level, notice, create_time) \
			VALUES(?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_string(1, m_szName, strlen(m_szName));
		csp.set_int32(2, m_Level);
		csp.set_string(3, m_szNotice, strlen(m_szNotice));
		csp.set_uint64(4, m_uCreateTime);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from guild where id = ?");
		csp.set_uint64(0, m_uGuid);
		return pDB->Execute(&csp);
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
	UINT64 m_uJoinTime;

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO guild_member (guildid, roleid, join_time, pose) \
			VALUES(?,?,?,?);");
		csp.set_uint64(0, m_uGuildID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint64(2, m_uJoinTime);
		csp.set_int32(3, m_Pos);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO guild_member (guildid, roleid, join_time, pose) \
			VALUES(?,?,?,?);");
		csp.set_uint64(0, m_uGuildID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint64(2, m_uJoinTime);
		csp.set_int32(3, m_Pos);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from guild_member where guildid = ? and roleid = ?");
		csp.set_uint64(0, m_uGuildID);
		csp.set_uint64(1, m_uRoleID);
		return pDB->Execute(&csp);
	}
};

#endif //__GUILD_DATA_OBJECT_H__