#ifndef __FRIEND_MODULE_H__
#define __FRIEND_MODULE_H__
#include "ModuleBase.h"
#include "../ServerData/FriendData.h"
struct FriendDataObject;
class CFriendModule  : public CModuleBase
{
public:
	CFriendModule(CPlayerObject* pOwner);

	~CFriendModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL DispatchPacket(NetPacket* pNetPack);

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	FriendDataObject* GetFriendData(UINT64 uRoleID);

public:
	//*********************消息处理定义开始******************************

	//*********************消息处理定义结束******************************

public:
	std::map<UINT64, FriendDataObject*> m_mapFriendData;

};

#endif //__FRIEND_MODULE_H__
