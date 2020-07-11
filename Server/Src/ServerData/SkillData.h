#ifndef __SKILL_DATA_OBJECT_H__
#define __SKILL_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"
struct SkillDataObject : public ShareObject
{
	SkillDataObject()
	{
		m_uRoleID = 0;	//角色ID
		m_dwSkillID = 0;	//技能ID
		m_nLevel = 0;
		m_nKeyPos = 0; //
	}

	UINT64 m_uRoleID;		//角色ID
	UINT32 m_dwSkillID;		//技能ID
	UINT32 m_nLevel;		//技能等级
	UINT32 m_nKeyPos;		//按键位置

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO skill (id, roleid, level, key_pos) VALUES(?,?,?,?);");
		csp.set_uint32(0, m_dwSkillID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_nLevel);
		csp.set_uint32(3, m_nKeyPos);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO skill (id, roleid,level, key_pos) VALUES(?,?,?,?);");
		csp.set_uint32(0, m_dwSkillID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_nLevel);
		csp.set_uint32(3, m_nKeyPos);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		return TRUE;
	}
};


#endif //__SKILL_DATA_OBJECT_H__