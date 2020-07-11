#ifndef __EQUIPMENT_MODULE_H__
#define __EQUIPMENT_MODULE_H__
#include "ModuleBase.h"
#include "EquipData.h"
#include "ServerDefine.h"
#include "../Message/Game_Define.pb.h"
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

	BOOL CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

	VOID RegisterMessageHanler();

public:
	UINT64 AddEquip(UINT32 dwEquipID);

	BOOL NotifyChange();

	EquipDataObject* GetEquipByGuid(UINT64 uGuid);

	//uguid：装备guid, ubagguid， 装备在背包中的格子guid
	UINT32 DressEquip(UINT64 uGuid, UINT64 uBagGuid);

	UINT32 UnDressEquip(UINT64 uGuid);

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgSetupEquipReq(NetPacket* pNetPacket);  //穿装备请求
	BOOL OnMsgUnsetEquipReq(NetPacket* pNetPacket);  //穿装备请求
	//*********************消息处理定义结束******************************

public:
	std::map<UINT64, EquipDataObject*>m_mapEquipData;
	EquipDataObject* m_vtDressEquip[EQUIP_MAX_NUM];
};

#endif //__EQUIPMENT_MODULE_H__
