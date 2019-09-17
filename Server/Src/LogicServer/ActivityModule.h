#ifndef __ACTIVITY_MODULE_H__
#define __ACTIVITY_MODULE_H__
#include "ModuleBase.h"
struct ActivityDataObject;
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

	VOID RegisterMessageHanler();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL NotifyChange();

	BOOL GetRedPoint();
public:
	//*********************消息处理定义开始******************************

	//*********************消息处理定义结束******************************
	std::map<UINT32, ActivityDataObject*> m_mapActivityData;
public:


};

#endif //__ACTIVITY_MODULE_H__
