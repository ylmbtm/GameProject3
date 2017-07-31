#ifndef __EQUIP_DATA_OBJECT_H__
#define __EQUIP_DATA_OBJECT_H__
#include "serverStruct.h"
#include "..\ServerEngine\SharedMemory.h"
struct EquipDataObject : public ShareObject
{
	EquipDataObject()
	{
		
	}
	
	UINT64 m_u64ID;			//角色ID
	UINT64 m_uGuid;			//guid
	UINT32 m_EquipID;		//装备ID
	INT32  m_StrengthLvl;   //强化等级
	INT32  m_RefineLevel;   //精炼等级
	INT32  m_StarLevel;		//星级
	INT32  m_RefineExp;		//精验的经验
	INT32  m_StarExp;		//星级经验

	BOOL Save(IDataBase *pDB)
	{ 
		

		return TRUE;
	}

	BOOL Delete(IDataBase *pDB)
	{
		

		return TRUE;
	}
};



#endif //__EQUIP_DATA_OBJECT_H__