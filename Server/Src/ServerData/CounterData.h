#ifndef __COUNTER_DATA_OBJECT_H__
#define __COUNTER_DATA_OBJECT_H__
#include "ServerStruct.h"

struct CounterDataObject : public ShareObject
{
	CounterDataObject()
	{

	}

	UINT64 m_uRoleID;		//角色ID
	UINT64 m_uCounterID;	//计数器ID
	UINT32 m_dwIndex;		//子索引
	UINT32 m_dwValue;		//计数器值
	UINT32 m_dwTime;		//参与时间
	UINT32 m_dwParam1;		//参数1
	UINT32 m_dwParam2;		//参数2

	BOOL Save(IDataBase* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{

		return TRUE;
	}
};

#endif //__COUNTER_DATA_OBJECT_H__