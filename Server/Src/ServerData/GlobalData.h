#ifndef __GLOBAL_DATA_OBJECT_H__
#define __GLOBAL_DATA_OBJECT_H__
#include "DBInterface/DBStoredProc.h"
#include "DBInterface/DBInterface.h"

#define MAX_EXTRA_INDEX 60

struct GlobalDataObject : public ShareObject
{
	GlobalDataObject()
	{
		m_dwServerID		= 0;			//服务器ID
		m_u64Guid			= 0;			//全局GUID
		m_dwMaxOnline		= 0;			//最大在线人数
		memset(m_exData, 0, sizeof(INT32) * MAX_EXTRA_INDEX);
	}

	UINT32 m_dwServerID;        //服务器ID
	UINT64 m_u64Guid;			//全局GUID
	UINT32 m_dwMaxOnline;		//最大在线人数
	INT32 m_exData[MAX_EXTRA_INDEX];      //扩展数据

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO globaldata (serverid, maxguid, maxonline, extradata) VALUES(?, ?, ?, ?);");
		csp.set_uint32(0, m_dwServerID);
		csp.set_uint64(1, m_u64Guid);
		csp.set_uint32(2, m_dwMaxOnline);
		csp.set_tinyblob(3, m_exData, sizeof(INT32) * MAX_EXTRA_INDEX);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO globaldata (serverid, maxguid, maxonline, extradata) VALUES(?, ?, ?, ?);");
		csp.set_uint32(0, m_dwServerID);
		csp.set_uint64(1, m_u64Guid);
		csp.set_uint32(2, m_dwMaxOnline);
		csp.set_tinyblob(3, m_exData, sizeof(INT32) * MAX_EXTRA_INDEX);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		return TRUE;
	}
};


#endif //__GLOBAL_DATA_OBJECT_H__