#ifndef __PARTNER_DATA_OBJECT_H__
#define __PARTNER_DATA_OBJECT_H__
#include "ServerStruct.h"
#include "SharedMemory.h"
struct PartnerDataObject : public ShareObject
{
	PartnerDataObject ()
	{

	}

	UINT64 m_uRoleID;			//角色ID
	UINT64 m_uGuid;			//guid
	UINT32 m_PartnerID;		//伙伴ID
	INT32  m_StrengthLvl;   //强化等级
	INT32  m_RefineLevel;   //精炼等级
	INT32  m_StarLevel;		//星级
	INT32  m_RefineExp;		//精验的经验
	INT32  m_StarExp;		//星级经验
	BOOL   m_IsUsing;       //当前使用中

	BOOL Save(IDataBase* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		return TRUE;
	}
};


#endif //__PET_DATA_OBJECT_H__