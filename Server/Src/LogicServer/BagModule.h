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
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL CalcFightValue(INT32 nValue[MAX_PROPERTY_NUM], INT32 nPercent[MAX_PROPERTY_NUM], INT32& FightValue);
public:

	BOOL AddItem(UINT32 dwItemID, INT32 nCount);
	BOOL RemoveItem(UINT32 dwItemID, INT32 nCount);
	INT32 GetItemCount(UINT32 dwItemID);

	BagDataObject* GetItemByGuid(UINT64 uGuid);

	BOOL NotifyChange();

public:
	std::map<UINT64, BagDataObject*>m_mapBagData;

	std::set<UINT64> m_BagChange;
	std::set<UINT64> m_BagRemove;
};

#endif //__BAG_MODULE_H__
