#ifndef __PARTNER_DATA_OBJECT_H__
#define __PARTNER_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
struct PartnerDataObject : public ShareObject
{
	PartnerDataObject ()
	{
		m_uRoleID = 0;			//角色ID
		m_uGuid = 0;			//guid
		m_PartnerID = 0;		//伙伴ID
		m_StrengthLvl = 0;		//强化等级
		m_RefineLevel = 0;		//精炼等级
		m_StarLevel = 0;		//星级
		m_RefineExp = 0;		//精验的经验
		m_StarExp = 0;			//星级经验
		m_SetPos = 0;			//当前上阵位置
	}

	UINT64 m_uRoleID;			//角色ID
	UINT64 m_uGuid;			//guid
	UINT32 m_PartnerID;		//伙伴ID
	INT32  m_StrengthLvl;   //强化等级
	INT32  m_RefineLevel;   //精炼等级
	INT32  m_StarLevel;		//星级
	INT32  m_RefineExp;		//精验的经验
	INT32  m_StarExp;		//星级经验
	BOOL   m_SetPos;       //当前使用中

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO partner (guid, roleid, partnerid, setpos, strengthlvl, refinelvl, starlvl) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_PartnerID);
		csp.set_int32(3, m_SetPos);
		csp.set_int32(4, m_StrengthLvl);
		csp.set_int32(5, m_RefineLevel);
		csp.set_int32(6, m_StarLevel);
		pDB->Execute(&csp);
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO partner (guid, roleid, partnerid, setpos, strengthlvl, refinelvl, starlvl) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_PartnerID);
		csp.set_int32(3, m_SetPos);
		csp.set_int32(4, m_StrengthLvl);
		csp.set_int32(5, m_RefineLevel);
		csp.set_int32(6, m_StarLevel);
		pDB->Execute(&csp);
		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from partner where guid = ?");
		csp.set_uint64(0, m_uGuid);
		pDB->Execute(&csp);
		return TRUE;
	}
};


#endif //__PET_DATA_OBJECT_H__