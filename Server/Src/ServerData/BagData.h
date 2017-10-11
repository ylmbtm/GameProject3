#ifndef __BAG_DATA_OBJECT_H__
#define __BAG_DATA_OBJECT_H__

#include "DBInterface/DBInterface.h"

#include "ServerDefine.h"

struct BagDataObject : public ShareObject
{
	BagDataObject()
	{

	}

	UINT64 m_uGuid;			//格子的guid
	UINT64 m_uRoleID;		//角色ID
	UINT32 m_BagType;       //背包类型
	UINT64 m_ItemGuid;		//物品guid
	UINT32 m_ItemID;        //道具ID
	INT32  m_nCount;       //个数
	BOOL   m_bBind;         //是否是邦定

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
		char szSql[SQL_BUFF_LEN];
		sprintf_s(szSql, 1024, "update player set delete = %d");

		pDB->Execute(szSql);

		return TRUE;
	}
};

#endif //__BAG_DATA_OBJECT_H__