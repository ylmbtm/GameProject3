#ifndef __GLOBAL_DATA_OBJECT_H__
#define __GLOBAL_DATA_OBJECT_H__

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
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{
		char szSql[SQL_BUFF_LEN];
		sprintf_s(szSql, 1024, "REPLACE INTO globaldata (serverid, maxguid) VALUES(%d, %lld);", \
		          m_dwServerID, m_u64Guid);

		pDB->Execute(szSql);

		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{


		return TRUE;
	}
};


#endif //__GLOBAL_DATA_OBJECT_H__