#ifndef __PET_DATA_OBJECT_H__
#define __PET_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"
struct PetDataObject : public ShareObject
{
	PetDataObject ()
	{
		m_uRoleID = 0;			//角色ID
		m_uGuid = 0;			//guid
		m_PetID = 0;			//宠物ID
		m_StrengthLvl = 0;		//强化等级
		m_RefineLevel = 0;		//精炼等级
		m_StarLevel = 0;		//星级
		m_RefineExp = 0;		//精验的经验
		m_StarExp = 0;			//星级经验
		m_IsUsing = 0;			//当前使用中
	}

	UINT64 m_uRoleID;			//角色ID
	UINT64 m_uGuid;			//guid
	UINT32 m_PetID;			//宠物ID
	INT32  m_StrengthLvl;   //强化等级
	INT32  m_RefineLevel;   //精炼等级
	INT32  m_StarLevel;		//星级
	INT32  m_RefineExp;		//精验的经验
	INT32  m_StarExp;		//星级经验
	BOOL   m_IsUsing;       //当前使用中

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO pet (guid, roleid, petid, isuse, strengthlvl, refinelvl, starlvl) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_PetID);
		csp.set_bool(3, m_IsUsing);
		csp.set_int32(4, m_StrengthLvl);
		csp.set_int32(5, m_RefineLevel);
		csp.set_int32(6, m_StarLevel);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO pet (guid, roleid, petid, isuse, strengthlvl, refinelvl, starlvl) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_PetID);
		csp.set_bool(3, m_IsUsing);
		csp.set_int32(4, m_StrengthLvl);
		csp.set_int32(5, m_RefineLevel);
		csp.set_int32(6, m_StarLevel);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from pet where guid = ?");
		csp.set_uint64(0, m_uGuid);
		return pDB->Execute(&csp);
	}
};


#endif //__PET_DATA_OBJECT_H__