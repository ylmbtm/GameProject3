#ifndef __BAG_DATA_OBJECT_H__
#define __BAG_DATA_OBJECT_H__

#include "serverStruct.h"
struct BagDataObject : public ShareObject
{
	BagDataObject()
	{
		
	}
	
	UINT64 m_uGuid;			//格子的guid
	UINT64 m_uRoleID;			//角色ID
	UINT32 m_BagType;       //背包类型
	UINT64 m_ItemGuid;		//物品guid
	UINT32 m_ItemID;        //道具ID
	INT32  m_nCount;       //个数
	BOOL   m_bBind;         //是否是邦定

	BOOL Save(IDataBase *pDB)
	{ 
	

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