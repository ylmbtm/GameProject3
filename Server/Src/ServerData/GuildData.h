#ifndef __GUILD_DATA_OBJECT_H__
#define __GUILD_DATA_OBJECT_H__

#include "ServerStruct.h"
#include "SharedMemory.h"
#include "ServerDefine.h"
struct GuildDataObject : public ShareObject
{
	GuildDataObject()
	{

	}

	UINT64 m_uGuid;			//格子的guid
	CHAR m_szName[GUILD_NAME_LEN];
	CHAR m_szNotice[GUILD_NOTICE_LEN];
	INT32  m_Level;

	BOOL Save(IDataBase* pDB)
	{


		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
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
	UINT32 m_dwJoinTime;



	BOOL Save(IDataBase* pDB)
	{


		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		char szSql[SQL_BUFF_LEN];
		sprintf_s(szSql, 1024, "update player set delete = %d");

		pDB->Execut(szSql);

		return TRUE;
	}
};

#endif //__GUILD_DATA_OBJECT_H__