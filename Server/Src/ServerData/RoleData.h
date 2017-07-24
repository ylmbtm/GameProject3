#ifndef __ROLE_DATA_OBJECT_H__
#define __ROLE_DATA_OBJECT_H__
#include "serverStruct.h"
#include "SharedMemory.h"
struct RoleDataObject : public ShareObject
{
	RoleDataObject()
	{
		
	}
	
	UINT64 m_u64ID;			//角色ID
	UINT64 m_u64AccountID;	//账号ID
	CHAR   m_szName[255];	//角色名
	UINT32 m_dwActorID;		//角色类型
	UINT32 m_dwLevel;		//等级
	UINT64 m_Money[10];		//所有的货币
	UINT64 m_Action[10];	//所有的体力
    UINT32 m_Actime[10];    //体力恢复时间
	UINT64 m_dwExp;			//经验
	UINT32 m_dwLangID;		//语言ID
	UINT64 m_u64Fight;      //战力
    UINT32 m_dwVipLvl;      //VIP等级
    UINT32 m_dwVipExp;      //VIP经验
    UINT32 m_dwCityCopy;    //主城副本类型

	BOOL Save(IDataBase *pDB)
	{ 
		char szSql[1024];
		sprintf(szSql, "REPLACE INTO player (id, account_id, name, actorid,level, exp, langid) VALUES(%lld, %lld, '%s', %d, %d, %lld,%d);", \
			m_u64ID, m_u64AccountID, m_szName, m_dwActorID, m_dwLevel, m_dwExp, m_dwLangID);

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