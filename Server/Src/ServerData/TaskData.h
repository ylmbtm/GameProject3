#ifndef __TASK_DATA_OBJECT_H__
#define __TASK_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
#include "SharedMemory.h"

struct TaskDataObject : public ShareObject
{
	TaskDataObject ()
	{

	}

	UINT64 m_u64ID;			//角色ID
	UINT32 m_uTaskID;		//任务ID
	UINT32 m_TaskState;
	UINT32 m_TaskCondition; //

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


#endif //__TASK_DATA_OBJECT_H__