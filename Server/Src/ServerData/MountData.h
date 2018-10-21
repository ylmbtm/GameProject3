#ifndef __MOUNT_DATA_OBJECT_H__
#define __MOUNT_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"

struct MountDataObject : public ShareObject
{
	MountDataObject ()
	{

	}

	UINT64 m_uRoleID;			//角色ID
	UINT64 m_uGuid;			//guid
	UINT32 m_MountID;			//坐骑ID
	INT32  m_StrengthLvl;   //强化等级
	INT32  m_RefineLevel;   //精炼等级
	INT32  m_StarLevel;		//星级
	INT32  m_RefineExp;		//精验的经验
	INT32  m_StarExp;		//星级经验
	BOOL   m_IsUsing;       //当前使用中

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO mount (guid, roleid, mountid, isuse, strengthlvl, refinelvl, starlvl) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_MountID);
		csp.set_bool(3, m_IsUsing);
		csp.set_int32(4, m_StrengthLvl);
		csp.set_int32(5, m_RefineLevel);
		csp.set_int32(6, m_StarLevel);
		pDB->Execute(&csp);
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO mount (guid, roleid, mountid, isuse, strengthlvl, refinelvl, starlvl) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_MountID);
		csp.set_bool(3, m_IsUsing);
		csp.set_int32(4, m_StrengthLvl);
		csp.set_int32(5, m_RefineLevel);
		csp.set_int32(6, m_StarLevel);
		pDB->Execute(&csp);
		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from mount where guid = ?");
		csp.set_uint64(0, m_uGuid);
		pDB->Execute(&csp);
		return TRUE;
	}
};


#endif //__MOUNT_DATA_OBJECT_H__