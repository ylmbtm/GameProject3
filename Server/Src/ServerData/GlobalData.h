#ifndef __GLOBAL_DATA_OBJECT_H__
#define __GLOBAL_DATA_OBJECT_H__
#include "DBInterface/DBStoredProc.h"
#include "DBInterface/DBInterface.h"

struct GlobalDataObject : public ShareObject
{
	GlobalDataObject()
	{
		m_dwServerID		= 0;			//服务器ID
		m_u64Guid			= 0;			//全局GUID
		m_dwMaxOnline		= 0;			//最大在线人数
	}

	UINT32 m_dwServerID;        //服务器ID
	UINT64 m_u64Guid;			//全局GUID
	UINT32 m_dwMaxOnline;		//最大在线人数

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO globaldata (serverid, maxguid, maxonline) VALUES(?, ?, ?);");
		csp.set_uint32(0, m_dwServerID);
		csp.set_uint64(1, m_u64Guid);
		csp.set_uint32(2, m_dwMaxOnline);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO globaldata (serverid, maxguid, maxonline) VALUES(?, ?, ?);");
		csp.set_uint32(0, m_dwServerID);
		csp.set_uint64(1, m_u64Guid);
		csp.set_uint32(2, m_dwMaxOnline);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		//static CDBStoredProcedure csp("delete from globaldata where serverid = ?;");
		//csp.set_uint32(0, m_dwServerID);
		//return pDB->Execute(&csp);

		return TRUE;
	}
};


#endif //__GLOBAL_DATA_OBJECT_H__