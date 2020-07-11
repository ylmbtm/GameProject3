#ifndef __BAG_DATA_OBJECT_H__
#define __BAG_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
struct BagDataObject : public ShareObject
{
	BagDataObject()
	{
		m_uGuid = 0;
		m_uRoleID = 0;
		m_ItemGuid = 0;
		m_ItemID = 0;
		m_nCount = 0;
		m_bBind = 0;
		m_nStatus = 0;
	}

	UINT64	m_uGuid;		//格子的guid
	UINT64	m_uRoleID;		//角色ID
	UINT64	m_ItemGuid;		//物品guid
	UINT32	m_ItemID;		//道具ID
	INT64	m_nCount;		//个数
	BOOL	m_bBind;		//是否是邦定
	UINT32	m_nStatus;		//是否删除

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("INSERT INTO bag (guid, roleid, itemguid, itemid, count, status) \
			VALUES(?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint64(2, m_ItemGuid);
		csp.set_int32(3,  m_ItemID);
		csp.set_int64(4,  m_nCount);
		csp.set_int32(5,  m_nStatus);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO bag (guid, roleid, itemguid, itemid, count, status) \
			VALUES(?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint64(2, m_ItemGuid);
		csp.set_int32(3, m_ItemID);
		csp.set_int64(4, m_nCount);
		csp.set_int32(5, m_nStatus);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from bag where guid = ?");
		csp.set_uint64(0, m_uGuid);
		return pDB->Execute(&csp);
	}
};

#endif //__BAG_DATA_OBJECT_H__