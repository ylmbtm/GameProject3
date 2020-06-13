#ifndef __PET_MODULE_H__
#define __PET_MODULE_H__
#include "ModuleBase.h"
#include "PetData.h"

struct PetDataObject;
class CPetModule  : public CModuleBase
{
public:
	CPetModule(CPlayerObject* pOwner);

	~CPetModule();

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

	BOOL ToTransferData(TransferDataItem* pTransItem);

	PetDataObject* GetCurrentPetData();

	//*********************消息处理定义开始******************************
	BOOL OnMsgSetupPetReq(NetPacket* pNetPacket);  //出战宠物
	BOOL OnMsgUnsetPetReq(NetPacket* pNetPacket);  //收回宠物
	//*********************消息处理定义结束******************************

public:
	UINT64 AddPet(UINT32 dwPetID);

	BOOL NotifyChange();

	PetDataObject* GetPetByGuid(UINT64 uGuid);

public:
	std::map<UINT64, PetDataObject*>m_mapPetData;
};

#endif //__PET_MODULE_H__
