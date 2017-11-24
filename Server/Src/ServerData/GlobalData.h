#ifndef __GLOBAL_DATA_OBJECT_H__
#define __GLOBAL_DATA_OBJECT_H__
#include "DBInterface/DBStoredProc.h"
#include "DBInterface/DBInterface.h"
#include "SharedMemory.h"
#include "ServerDefine.h"

struct GlobalDataObject : public ShareObject
{
	GlobalDataObject()
	{
		m_dwServerID		= 0;			//服务器ID
		m_u64Guid			= 0;			//全局GUID
	}

	UINT32 m_dwServerID;        //服务器ID
	UINT64 m_u64Guid;			//全局GUID

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO globaldata (serverid, maxguid) VALUES(?, ?);");
		csp.set_uint64(0, m_dwServerID);
		csp.set_uint64(1, m_u64Guid);

		pDB->Execute(&csp);

		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("update globaldata set maxguid = ? where serverid = ?;");
		csp.set_uint64(0, m_u64Guid);
		csp.set_uint64(1, m_dwServerID);
		pDB->Execute(&csp);

		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{


		return TRUE;
	}
};


#endif //__GLOBAL_DATA_OBJECT_H__