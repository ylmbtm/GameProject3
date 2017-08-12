#ifndef __GUILD_DATA_OBJECT_H__
#define __GUILD_DATA_OBJECT_H__

#include "serverStruct.h"
#include "SharedMemory.h"
struct GuildDataObject : public ShareObject
{
	GuildDataObject()
	{

	}

	UINT64 m_uGuid;			//格子的guid
	std::string m_strName;
	std::string m_strNotice;
	INT32  m_Level;

	BOOL Save(IDataBase* pDB)
	{


		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		char szSql[1024];
		sprintf(szSql, "update player set delete = %d");

		pDB->Execut(szSql);

		return TRUE;
	}
};


struct MemberDataObject : public ShareObject
{
	MemberDataObject()
	{

	}

	UINT64 m_uGuid;			//格子的guid
	INT32 Pos;   //! 角色
	INT32 Contribute;    //! 军团贡献
	UINT32 EnterTime;  //! 加入时间
	UINT32 BattleTimes;    //! 攻打军团副本次数
	UINT32 BattleDamage;  //! 攻打军团副本最高伤害

	BOOL Save(IDataBase* pDB)
	{


		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		char szSql[1024];
		sprintf(szSql, "update player set delete = %d");

		pDB->Execut(szSql);

		return TRUE;
	}
};

#endif //__GUILD_DATA_OBJECT_H__