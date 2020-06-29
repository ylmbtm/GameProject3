#ifndef __COUNTER_DATA_OBJECT_H__
#define __COUNTER_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"

struct CounterDataObject : public ShareObject
{
	CounterDataObject()
	{
		m_uRoleID = 0;      //角色ID
		m_uCounterID = 0;   //计数器ID
		m_dwIndex = 0;      //子索引
		m_uValue = 0;       //计数器值
		m_uTime = 0;        //参与时间
		m_dwParam1 = 0;     //参数1
		m_dwParam2 = 0;     //参数2
	}

	UINT64 m_uRoleID;       //角色ID
	UINT32 m_uCounterID;    //计数器ID
	UINT32 m_dwIndex;       //子索引
	INT64  m_uValue;        //计数器值
	UINT64 m_uTime;         //参与时间
	UINT32 m_dwParam1;      //参数1
	UINT32 m_dwParam2;      //参数2

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO counter (id, roleid, cindex, time, value) VALUES(?,?,?,?,?);");
		csp.set_uint32(0, m_uCounterID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_dwIndex);
		csp.set_uint64(3, m_uTime);
		csp.set_int64(4, m_uValue);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO counter (id, roleid, cindex, time, value) VALUES(?,?,?,?,?);");
		csp.set_uint32(0, m_uCounterID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_dwIndex);
		csp.set_uint64(3, m_uTime);
		csp.set_int64(4, m_uValue);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from counter where id = ? and roleid = ? and index = ?");
		csp.set_uint32(0, m_uCounterID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_dwIndex);
		return pDB->Execute(&csp);
	}
};

#endif //__COUNTER_DATA_OBJECT_H__