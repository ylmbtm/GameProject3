#ifndef __EQUIPMENT_MODULE_H__
#define __EQUIPMENT_MODULE_H__
#include "ModuleBase.h"
#include "..\ServerData\EquipData.h"

struct EquipDataObject;
class CEquipModule  : public CModuleBase
{
public:
	CEquipModule(CPlayerObject* pOwner);

	~CEquipModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

public:
	UINT64 AddEquip(UINT32 dwEquipID);

public:
	std::map<UINT32, EquipDataObject*>m_mapEquipData;

	std::set<UINT64> m_setChange;
	std::set<UINT64> m_setRemove;


};

#endif //__EQUIPMENT_MODULE_H__
