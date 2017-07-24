#ifndef __BAG_MODULE_H__
#define __BAG_MODULE_H__

#include "ModuleBase.h"
struct BagDataObject;
class CBagModule : public CModuleBase
{
public:
	CBagModule(CPlayerObject *pOwner);

	~CBagModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromLoginAck(DBRoleLoginAck &Ack);

public:
	std::vector<BagDataObject*>m_vtBagData;


};

#endif //__BAG_MODULE_H__
