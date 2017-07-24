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