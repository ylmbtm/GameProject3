#ifndef __ACTIVITY_DATA_OBJECT_H__
#define __ACTIVITY_DATA_OBJECT_H__
#include "ServerStruct.h"
struct ActivityDataObject : public ShareObject
{
	ActivityDataObject()
	{

	}
	UINT64 m_uRoleID;		//角色ID
	UINT32 m_dwActivityID;  //活动ID
	UINT32 m_dwActivityType;//活动类型
	UINT32 m_dwJoinTime;    //参与时间
    UINT32 m_dwDataLen;     //数据时间长度
	BYTE   m_Data[1024];    //活动数据

	BOOL Save(IDataBase* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		return TRUE;
	}
};

#endif //__ACTIVITY_DATA_OBJECT_H__