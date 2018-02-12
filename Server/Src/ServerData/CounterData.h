#ifndef __COUNTER_DATA_OBJECT_H__
#define __COUNTER_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"

struct CounterDataObject : public ShareObject
{
	CounterDataObject()
	{
		m_uRoleID = 0;		//角色ID
		m_uCounterID = 0;	//计数器ID
		m_dwIndex = 0;		//子索引
		m_dwValue = 0;		//计数器值
		m_dwTime = 0;		//参与时间
		m_dwParam1 = 0;		//参数1
		m_dwParam2 = 0;		//参数2
	}

	UINT64 m_uRoleID;		//角色ID
	UINT64 m_uCounterID;	//计数器ID
	UINT32 m_dwIndex;		//子索引
	UINT32 m_dwValue;		//计数器值
	UINT32 m_dwTime;		//参与时间
	UINT32 m_dwParam1;		//参数1
	UINT32 m_dwParam2;		//参数2

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

#endif //__COUNTER_DATA_OBJECT_H__