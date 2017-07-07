#ifndef __ROLE_DATA_OBJECT_H__
#define __ROLE_DATA_OBJECT_H__

#include "serverStruct.h"
struct RoleDataObject : public ShareObject
{
	RoleDataObject()
	{
		
	}
	
	UINT64 m_u64ID;			//角色ID
	UINT64 m_u64AccountID;	//账号ID
	CHAR   m_szName[255];	//角色名
	UINT32 m_RoleType;		//角色类型
	UINT32 m_dwLevel;		//等级
	UINT64 m_Money[10];		//所有的货币
	UINT64 m_dwExp;			//经验
	UINT32 m_dwLangID;		//语言ID
	UINT64 m_u64Fight;      //战力

	BOOL Save(IDataBase *pDB)
	{
		char szSql[1024];
		sprintf(szSql, "REPLACE INTO player (id, account_id, name, roletype,level, exp, langid) VALUES(%lld, %lld, '%s', %d, %d, %lld,%d);", \
			m_u64ID, m_u64AccountID, m_szName, m_RoleType, m_dwLevel, m_dwExp, m_dwLangID);

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


struct CopyDataObject : public ShareObject
{
	CopyDataObject()
	{
		UINT64 m_u64ID = 0;
		UINT32 m_dwChaper = 0;
		UINT32 m_dwCopyType = 0;
		UINT32 m_dwBattleTimes = 0;
		UINT32 m_dwResetTimes = 0;
		UINT32 m_dwStar = 0;
	}

	UINT64 m_u64ID;			//角色ID
	UINT32 m_dwChaper;
	UINT32 m_dwCopyType;
	UINT32 m_dwBattleTimes;
	UINT32 m_dwResetTimes;
	UINT32 m_dwStar;

	BOOL Save(IDataBase *pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase *pDB)
	{
		return TRUE;
	}
};


#endif //__SERVER_STRUCT_H__