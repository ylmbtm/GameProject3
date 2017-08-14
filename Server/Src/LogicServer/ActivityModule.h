#ifndef __ACTIVITY_MODULE_H__
#define __ACTIVITY_MODULE_H__
#include "ModuleBase.h"

class CActivityModule  : public CModuleBase
{
public:
	CActivityModule(CPlayerObject* pOwner);

	~CActivityModule();

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


public:


};

#endif //__ACTIVITY_MODULE_H__
