#ifndef __TASK_DATA_OBJECT_H__
#define __TASK_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"
struct TaskDataObject : public ShareObject
{
	TaskDataObject ()
	{
		m_uRoleID = 0;	//角色ID
		m_uTaskID = 0;	//任务ID
		m_TaskStatus = 0;
		m_nProgress = 0; //
	}

	UINT64 m_uRoleID;		//角色ID
	UINT32 m_uTaskID;		//任务ID
	UINT32 m_TaskStatus;	//任务状态
	UINT32 m_nProgress;	//任务进度

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO task (id, roleid, task_status, progress) VALUES(?,?,?,?);");
		csp.set_uint32(0, m_uTaskID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_TaskStatus);
		csp.set_uint32(3, m_nProgress);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO task (id, roleid, task_status, progress) VALUES(?,?,?,?);");
		csp.set_uint32(0, m_uTaskID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_TaskStatus);
		csp.set_uint32(3, m_nProgress);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		return TRUE;
	}
};


#endif //__TASK_DATA_OBJECT_H__