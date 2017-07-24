#ifndef __BAG_DATA_OBJECT_H__
#define __BAG_DATA_OBJECT_H__

#include "serverStruct.h"
struct BagDataObject : public ShareObject
{
	BagDataObject()
	{
		
	}
	
	UINT64 m_u64ID;			//角色ID
	UINT32 m_BagType;       //背包类型
	UINT64 m_uGuid;			//guid
	UINT32 m_ItemID;        //道具ID
	UINT32 m_dwPos;         //背包位置
	UINT32 m_dwCount;       //个数
	BOOL   m_bBind;         //是否是邦定

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

#endif //__BAG_DATA_OBJECT_H__