#ifndef __COPY_MODULE_H__
#define __COPY_MODULE_H__
#include "ModuleBase.h"
#include "..\ServerData\serverStruct.h"

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

	CopyDataObject* GetCopyData(UINT32 dwCopyID);

public:
	std::map<UINT32, CopyDataObject*>m_mapCopyData;
};

#endif //__ROLE_MODULE_H__
