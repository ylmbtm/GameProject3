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

	UINT64	AddMount(UINT32 dwMountID);

	BOOL	NotifyChange();

	MountDataObject* GetMountByGuid(UINT64 uGuid);

public:
	std::map<UINT64, MountDataObject*>m_mapMountData;

	std::set<UINT64> m_setChange;
	std::set<UINT64> m_setRemove;
};

#endif //__PET_MODULE_H__
