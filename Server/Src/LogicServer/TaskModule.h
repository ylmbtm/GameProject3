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

	BOOL CalcFightValue(INT32 nValue[MAX_PROPERTY_NUM], INT32 nPercent[MAX_PROPERTY_NUM], INT32& FightValue);
public:
	BOOL OnTaskEvent(ETaskEvent taskEvent, UINT32 dwParam1, UINT32 dwParam2);


public:
	std::map<UINT64, TaskDataObject*>m_mapTaskData;


};

#endif //__PARTNER_MODULE_H__
