#ifndef __COPY_MODULE_H__
#define __COPY_MODULE_H__
#include "ModuleBase.h"

struct CopyDataObject;
class CCopyModule : public CModuleBase
{
public:
	CCopyModule(CPlayerObject* pOwner);

	~CCopyModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

public:

	CopyDataObject* GetCopyData(UINT32 dwCopyID);

public:
	std::map<UINT32, CopyDataObject*>m_mapCopyData;
};

#endif //__ROLE_MODULE_H__
