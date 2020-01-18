#ifndef __MOUNT_MODULE_H__
#define __MOUNT_MODULE_H__
#include "ModuleBase.h"
#include "../ServerData/MountData.h"

struct MountDataObject;
class CMountModule  : public CModuleBase
{
public:
	CMountModule(CPlayerObject* pOwner);

	~CMountModule();

public:
	BOOL	OnCreate(UINT64 u64RoleID);

	BOOL	OnDestroy();

	BOOL	OnLogin();

	BOOL	OnLogout();

	BOOL	OnNewDay();

	BOOL	ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL	SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL	CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

	VOID	RegisterMessageHanler();

	UINT64	AddMount(UINT32 dwMountID);

	BOOL	NotifyChange();

	MountDataObject* GetMountByGuid(UINT64 uGuid);

	MountDataObject* GetCurrentMountData();




	//*********************消息处理定义开始******************************
	BOOL OnMsgSetupMountReq(NetPacket* pNetPacket);  //出战坐骑
	BOOL OnMsgUnsetMountReq(NetPacket* pNetPacket);  //收回坐骑
	//*********************消息处理定义结束******************************

public:
	std::map<UINT64, MountDataObject*>m_mapMountData;
};

#endif //__PET_MODULE_H__
