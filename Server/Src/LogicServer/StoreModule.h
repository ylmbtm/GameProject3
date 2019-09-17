#ifndef __STORE_MODULE_H__
#define __STORE_MODULE_H__
#include "ModuleBase.h"
struct StoreDataObject;
class CStoreModule  : public CModuleBase
{
public:
	CStoreModule(CPlayerObject* pOwner);

	~CStoreModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	VOID RegisterMessageHanler();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL	NotifyChange();

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgStoreBuyReq(NetPacket* pNetPacket);  //购买请求
	//*********************消息处理定义结束******************************

public:


};

#endif //__STORE_MODULE_H__
