#ifndef __TASK_DATA_OBJECT_H__
#define __TASK_DATA_OBJECT_H__
#include "ServerStruct.h"
#include "SharedMemory.h"

struct TaskDataObject : public ShareObject
{
	TaskDataObject ()
	{

	}

	UINT64 m_u64ID;			//角色ID
	UINT32 m_TaskID;		//任务ID
	UINT32 m_TaskState;
	UINT32 m_TaskCondition; //

	BOOL Save(IDataBase* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		return TRUE;
	}
};


#endif //__TASK_DATA_OBJECT_H__