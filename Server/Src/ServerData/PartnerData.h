#ifndef __PARTNER_DATA_OBJECT_H__
#define __PARTNER_DATA_OBJECT_H__
#include "serverStruct.h"
#include "SharedMemory.h"
struct PartnerDataObject : public ShareObject
{
	PartnerDataObject ()
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


#endif //__PET_DATA_OBJECT_H__