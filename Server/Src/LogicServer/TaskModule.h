#ifndef __TASK_MODULE_H__
#define __TASK_MODULE_H__
#include "ModuleBase.h"
#include "../ServerData/TaskData.h"
#include "../ConfigData/ConfigDefine.h"

struct TaskDataObject;
class CTaskModule  : public CModuleBase
{
public:
	CTaskModule(CPlayerObject* pOwner);

	~CTaskModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

	BOOL	DispatchPacket(NetPacket* pNetPacket);
public:
	BOOL OnTaskEvent(ETaskEvent taskEvent, UINT32 dwParam1, UINT32 dwParam2);

	TaskDataObject* GetTaskByID(UINT32 dwTaskID);

	BOOL NotifyChange();
public:
	std::map<UINT64, TaskDataObject*>m_mapTaskData;

	std::set<UINT64> m_setChange;
	std::set<UINT64> m_setRemove;
};

#endif //__PARTNER_MODULE_H__
