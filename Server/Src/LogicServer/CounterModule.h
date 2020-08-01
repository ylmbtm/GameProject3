#ifndef __COUNTER_MODULE_H__
#define __COUNTER_MODULE_H__
#include "ModuleBase.h"
#include "CounterData.h"
class CCounterModule  : public CModuleBase
{
public:
	CCounterModule(CPlayerObject* pOwner);

	~CCounterModule();

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

	UINT64 GetCounterValue(UINT32 uID, UINT32 dwIndex = 0);

	BOOL   SetCounterValue(UINT32 uID, INT64 uValue, UINT32 dwIndex = 0);

	BOOL   AddCounterValue(UINT32 uID, INT64 uValue, UINT32 dwIndex = 0);

	BOOL   GetCounterBitValue(UINT32 uID);

	BOOL   SetCounterBitValue(UINT32 uID, BOOL bValue);

private:
	CounterDataObject* GetCounterData(UINT32 uID, UINT32 dwIndex, BOOL bCreate = FALSE);

public:
	//*********************消息处理定义开始******************************

	//*********************消息处理定义结束******************************

public:
	std::map<UINT64, CounterDataObject*> m_mapCounterData;

};

#endif //__COUNTER_MODULE_H__
