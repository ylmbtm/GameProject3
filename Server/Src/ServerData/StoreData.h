#ifndef __STORE_DATA_OBJECT_H__
#define __STORE_DATA_OBJECT_H__

#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"

struct StoreDataObject : public ShareObject
{
	StoreDataObject()
	{
		m_uRoleID = 0;
	}

	UINT64 m_uRoleID;		//角色ID

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

		return TRUE;
	}
};

#endif //__STORE_DATA_OBJECT_H__