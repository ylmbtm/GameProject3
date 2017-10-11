#ifndef __EQUIP_DATA_OBJECT_H__
#define __EQUIP_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
#include "../ServerEngine/SharedMemory.h"
struct EquipDataObject : public ShareObject
{
	EquipDataObject()
	{

	}

	UINT64 m_uRoleID;		//角色ID
	UINT64 m_uGuid;			//guid
	UINT32 m_EquipID;		//装备ID
	INT32  m_StrengthLvl;   //强化等级
	INT32  m_RefineLevel;   //精炼等级
	INT32  m_StarLevel;		//星级
	INT32  m_RefineExp;		//精验的经验
	INT32  m_StarExp;		//星级经验
	BOOL   m_IsUsing;       //当前使用中

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



#endif //__EQUIP_DATA_OBJECT_H__