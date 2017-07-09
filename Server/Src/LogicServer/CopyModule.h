#ifndef __COPY_MODULE_H__
#define __COPY_MODULE_H__
#include "ModuleBase.h"
#include "DataPool.h"
#include "..\ServerData\serverStruct.h"

class CCopyModule : public CModuleBase
{
public:
	CCopyModule(CPlayerObject *pOwner);

	~CCopyModule();

public:
	virtual BOOL OnCreate(UINT64 u64RoleID);
	
	virtual BOOL OnDestroy();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL OnNewDay();

	virtual BOOL OnLoadData(UINT64 u64RoleID);

public:
	std::map<UINT32, CopyDataObject*>m_mapCopyData;
};

#endif //__ROLE_MODULE_H__
