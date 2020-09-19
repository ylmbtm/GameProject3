#ifndef __SEAL_DATA_OBJECT_H__
#define __SEAL_DATA_OBJECT_H__

#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"

struct SealDataObject : public ShareObject
{
	SealDataObject()
	{
		m_uRoleID = 0;
		m_uSealTime = 0;
		m_nSealAction = 0;
	}

	UINT64  m_uRoleID;
	INT32   m_nSealAction;
	UINT64  m_uSealTime;

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("INSERT INTO seal_role (roleid, sealendtime, sealaction) \
			VALUES(?,?,?);");
		csp.set_uint64(0, m_uRoleID);
		csp.set_uint64(1, m_uSealTime);
		csp.set_int32(2, m_nSealAction);

		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO seal_role (roleid, sealendtime, sealaction) \
			VALUES(?,?,?);");
		csp.set_uint64(0, m_uRoleID);
		csp.set_uint64(1, m_uSealTime);
		csp.set_int32(2, m_nSealAction);

		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from seal_role where roleid = ?");
		csp.set_uint64(0, m_uRoleID);
		return pDB->Execute(&csp);
	}
};

#endif //__SEAL_DATA_OBJECT_H__