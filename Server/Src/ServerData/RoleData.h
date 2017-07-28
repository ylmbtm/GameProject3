#ifndef __ROLE_DATA_OBJECT_H__
#define __ROLE_DATA_OBJECT_H__
#include "serverStruct.h"
#include "SharedMemory.h"
struct RoleDataObject : public ShareObject
{
	RoleDataObject()
	{
		m_u64ID			= 0;			//角色ID
		m_u64AccountID	= 0;			//账号ID
		m_ActorID		= 0;			//角色类型
		m_Level		= 0;			//等级
		m_Exp			= 0;			//经验
		m_nLangID		= 0;			//语言ID
		m_u64Fight		= 0;			//战力
		m_VipLvl		= 0;			//VIP等级
		m_VipExp		= 0;			//VIP经验
		m_CityCopyID	= 0;			//主城副本类型
	}
	
	UINT64 m_u64ID;			//角色ID
	UINT64 m_u64AccountID;	//账号ID
	CHAR   m_szName[255];	//角色名
	INT32 m_ActorID;		//角色类型
	INT32 m_Level;			//等级
	INT64 m_Money[10];		//所有的货币
	INT64 m_Action[10];		//所有的体力
    INT32 m_Actime[10];		//体力恢复时间
	INT64 m_Exp;			//经验
	INT32 m_nLangID;		//语言ID
	INT64 m_u64Fight;		//战力
    INT32 m_VipLvl;			//VIP等级
    INT32 m_VipExp;			//VIP经验
    INT32 m_CityCopyID;		//主城副本类型

	BOOL Save(IDataBase *pDB)
	{ 
		char szSql[1024];
		sprintf(szSql, "REPLACE INTO player (id, account_id, name, actorid,level, exp, langid) VALUES(%lld, %lld, '%s', %d, %d, %lld,%d);", \
			m_u64ID, m_u64AccountID, m_szName, m_ActorID, m_Level, m_Exp, m_nLangID);

		pDB->Execut(szSql);

		return TRUE;
	}

	BOOL Delete(IDataBase *pDB)
	{
		char szSql[1024];
		sprintf(szSql, "update player set delete = %d");

		pDB->Execut(szSql);

		return TRUE;
	}
};


#endif //__SERVER_STRUCT_H__