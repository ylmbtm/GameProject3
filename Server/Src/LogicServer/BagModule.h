#ifndef __BAG_MODULE_H__
#define __BAG_MODULE_H__

#include "ModuleBase.h"
struct BagDataObject;
class CBagModule : public CModuleBase
{
public:
	CBagModule(CPlayerObject* pOwner);

	~CBagModule();

public:
	BOOL	OnCreate(UINT64 u64RoleID);

	BOOL	OnDestroy();

	BOOL	OnLogin();

	BOOL	OnLogout();

	BOOL	OnNewDay();

	BOOL	ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL	SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL	CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

	BOOL	ReadFromShareMemory(BagDataObject* pObject);

	VOID	RegisterMessageHanler();
public:
	BOOL	AddItem(UINT32 dwItemID, INT64 nCount);
	BOOL	RemoveItem(UINT32 dwItemID, INT64 nCount);
	INT64	GetItemCount(UINT32 dwItemID);

	BOOL	AddItem(UINT64 uItemGuid, UINT32 dwItemID, INT64 nCount);
	BOOL	RemoveItem(UINT64 uGuid);

	BOOL    SetBagItem(UINT64 uGuid, UINT64 uItemGuid, UINT32 dwItemID, INT64 nCount);

	BagDataObject* GetItemByGuid(UINT64 uGuid);

	BOOL	NotifyChange();

public:
	std::map<UINT64, BagDataObject*>m_mapBagData;
};

#endif //__BAG_MODULE_H__
