#ifndef __GEM_DATA_OBJECT_H__
#define __GEM_DATA_OBJECT_H__

#include "DBInterface/DBStoredProc.h"
#include "DBInterface/DBInterface.h"
struct GemDataObject : public ShareObject
{
	GemDataObject()
	{
		m_uRoleID = 0;
		m_uGuid = 0;
		m_GemID = 0;
		m_StrengthLvl = 0;
		m_RefineLevel = 0;
		m_StarLevel = 0;
		m_RefineExp = 0;
		m_StarExp = 0;
		m_EquipPos = 0;
	}

	UINT64 m_uRoleID;		//角色ID
	UINT64 m_uGuid;			//guid
	UINT32 m_GemID;			//宝石ID
	INT32  m_StrengthLvl;   //强化等级
	INT32  m_RefineLevel;   //精炼等级
	INT32  m_StarLevel;		//星级
	INT32  m_RefineExp;		//精验的经验
	INT32  m_StarExp;		//星级经验
	BOOL   m_EquipPos;            //当前使用中

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO gem (guid, roleid, gemid, pos, strengthlvl, refinelvl, starlvl) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_GemID);
		csp.set_int32(3,  m_EquipPos);
		csp.set_int32(4,  m_StrengthLvl);
		csp.set_int32(5,  m_RefineLevel);
		csp.set_int32(6,  m_StarLevel);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO gem (guid, roleid, gemid, pos, strengthlvl, refinelvl, starlvl) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_GemID);
		csp.set_int32(3,  m_EquipPos);
		csp.set_int32(4, m_StrengthLvl);
		csp.set_int32(5, m_RefineLevel);
		csp.set_int32(6, m_StarLevel);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from gem where guid = ?");
		csp.set_uint64(0, m_uGuid);
		return pDB->Execute(&csp);
	}
};



#endif //__GEM_DATA_OBJECT_H__