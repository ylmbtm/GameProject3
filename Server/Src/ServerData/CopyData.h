#ifndef __COPY_DATA_OBJECT_H__
#define __COPY_DATA_OBJECT_H__
#include "serverStruct.h"
struct CopyDataObject : public ShareObject
{
	CopyDataObject()
	{
		UINT64 m_u64ID = 0;
		UINT32 m_dwChaper = 0;
		UINT32 m_dwCopyType = 0;
		UINT32 m_dwBattleTimes = 0;
		UINT32 m_dwResetTimes = 0;
		UINT32 m_dwStar = 0;
	}

	UINT64 m_u64ID;			//角色ID
	UINT32 m_dwChaper;
	UINT32 m_dwCopyType;
	UINT32 m_dwBattleTimes;
	UINT32 m_dwResetTimes;
	UINT32 m_dwStar;

	BOOL Save(IDataBase *pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase *pDB)
	{
		return TRUE;
	}
};


#endif //__COPY_DATA_OBJECT_H__