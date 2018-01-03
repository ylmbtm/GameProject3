#ifndef __COUNTER_MODULE_H__
#define __COUNTER_MODULE_H__
#include "ModuleBase.h"
#include "../ServerData/CounterData.h"
struct CounterDataObject;
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

	BOOL DispatchPacket(NetPacket* pNetPack);

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL	NotifyChange();

	CounterDataObject* GetCounterData(UINT64 uID, UINT32 dwIndex, BOOL bCreate = FALSE);

public:
	//*********************消息处理定义开始******************************

	//*********************消息处理定义结束******************************

public:
	std::map<UINT64, std::vector<CounterDataObject*>> m_mapCounterData;

};

#endif //__COUNTER_MODULE_H__
