#include "stdafx.h"
#include "TaskModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "StaticData.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Game_Define.pb.h"
#include "GameLogManager.h"

CTaskModule::CTaskModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
    RegisterMessageHanler();
}

CTaskModule::~CTaskModule()
{
}

BOOL CTaskModule::OnCreate(UINT64 uRoleID)
{
    return TRUE;
}


BOOL CTaskModule::OnDestroy()
{
    for(auto itor = m_mapTaskData.begin(); itor != m_mapTaskData.end(); itor++)
    {
        itor->second->Release();
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
        const DBTaskItem& TaskItem = TaskData.tasklist(i);
        TaskDataObject* pObject = DataPool::CreateObject<TaskDataObject>(ESD_TASK, FALSE);
        pObject->m_nProgress = TaskItem.progress();
        pObject->m_uRoleID = TaskItem.roleid();
        pObject->m_nTaskID = TaskItem.taskid();
        pObject->m_nTaskStatus = TaskItem.status();

        if(pObject->m_nTaskStatus == ETS_COMMIT)
        {
            m_mapCommitTask.insert(std::make_pair(pObject->m_nTaskID, pObject));
        }
        else
        {
            m_mapTaskData.insert(std::make_pair(pObject->m_nTaskID, pObject));
        }
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

VOID CTaskModule::RegisterMessageHanler()
{
}

BOOL CTaskModule::OnTaskEvent(ETaskEvent taskEvent, INT32 nParam1, INT32 nParam2)
{
    for(std::map<UINT32, TaskDataObject*>::iterator itor = m_mapTaskData.begin(); itor != m_mapTaskData.end(); itor++)
    {
        TaskDataObject* pDataObj = itor->second;
        if ((pDataObj->m_nTaskStatus == ETS_FINISH) || (pDataObj->m_nTaskStatus == ETS_COMMIT))
        {
            continue;
        }

        StTaskInfo* pInfo = CStaticData::GetInstancePtr()->GetTaskInfo(pDataObj->m_nTaskID);
        if (pInfo == NULL)
        {
            continue;
        }

        if(pInfo->TaskEvent != taskEvent)
        {
            continue;
        }

        if(pDataObj->m_nProgress >= pInfo->NeedCount)
        {

        }
    }

    return TRUE;
}

TaskDataObject* CTaskModule::GetTaskByID(UINT32 dwTaskID)
{
    auto itor = m_mapTaskData.find(dwTaskID);
    if(itor != m_mapTaskData.end())
    {
        return itor->second;
    }

    return NULL;
}

BOOL CTaskModule::NotifyChange()
{
    if (m_setChange.size() <= 0 && m_setRemove.size() <= 0)
    {
        return TRUE;
    }

    TaskChangeNty Nty;
    for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
    {
        TaskDataObject* pObject = GetTaskByID((UINT32) * itor);
        ERROR_CONTINUE_EX(pObject != NULL);

        TaskItem* pItem = Nty.add_changelist();
    }

    for(auto itor = m_setRemove.begin(); itor != m_setRemove.end(); itor++)
    {
        Nty.add_removelist(*itor);
    }

    m_pOwnPlayer->SendMsgProtoBuf(MSG_TASK_CHANGE_NTY, Nty);

    m_setChange.clear();
    m_setRemove.clear();

    return TRUE;
}
