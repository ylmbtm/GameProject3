#include "stdafx.h"
#include "TaskModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "..\ConfigData\ConfigData.h"
#include "Log.h"

CTaskModule::CTaskModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CTaskModule::~CTaskModule()
{

}

BOOL CTaskModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CTaskModule::OnDestroy()
{
	for(auto itor = m_mapTaskData.begin(); itor != m_mapTaskData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapTaskData.clear();

	return TRUE;
}

BOOL CTaskModule::OnLogin()
{
	return TRUE;
}

BOOL CTaskModule::OnLogout()
{
	return TRUE;
}

BOOL CTaskModule::OnNewDay()
{
	return TRUE;
}

BOOL CTaskModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBTaskData& TaskData = Ack.taskdata();
	for(int i = 0; i < TaskData.tasklist_size(); i++)
	{
		const DBTaskItem &TaskItem = TaskData.tasklist(i);
		TaskDataObject *pObject = g_pTaskDataObjectPool->NewOjbect(FALSE);
		pObject->lock();

		pObject->unlock();
		m_mapTaskData.insert(std::make_pair(pObject->m_TaskID, pObject));
	}
	return TRUE;
}

BOOL CTaskModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}


BOOL CTaskModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

BOOL CTaskModule::OnTaskEvent(ETaskEvent taskEvent, UINT32 dwParam1, UINT32 dwParam2)
{
	for(std::map<UINT64, TaskDataObject*>::iterator itor = m_mapTaskData.begin(); itor != m_mapTaskData.end(); itor++)
	{
		TaskDataObject* pDataObj = itor->second;

		StTaskInfo* pInfo = CConfigData::GetInstancePtr()->GetTaskInfo(pDataObj->m_TaskID);
		ERROR_CONTINUE_EX(pInfo != NULL);

		if(pInfo->TaskEvent != taskEvent)
		{
			continue;
		}

		if(pDataObj->m_TaskState == TASK_FINISHED)
		{
			continue;
		}

		if(pDataObj->m_TaskCondition >= pInfo->Condition)
		{

		}
	}


	return TRUE;
}
