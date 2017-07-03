#ifndef __ROLE_MODULE_H__
#define __ROLE_MODULE_H__
#include "ModuleBase.h"
#include "DataPool.h"
#include "..\ServerData\serverStruct.h"

class CRoleModule : public CModuleBase
{
public:
	CRoleModule(CPlayerObject *pOwner);

	~CRoleModule();

public:
	virtual BOOL OnCreate(UINT64 u64RoleID);
	
	virtual BOOL OnDestroy(UINT64 u64RoleID);

	virtual BOOL OnLogin(UINT64 u64RoleID);

	virtual BOOL OnLogout(UINT64 u64RoleID);

	virtual BOOL OnNewDay();

	virtual BOOL OnLoadData(UINT64 u64RoleID);

	BOOL SetBaseData(UINT64 u64RoleID, std::string Name, UINT32 dwRoleType, UINT64 u64AccountID, UINT32 dwChannel);
public:

	RoleDataObject  *m_pRoleDataObject;
};

#endif //__ROLE_MODULE_H__
