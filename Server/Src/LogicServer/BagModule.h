#ifndef __BAG_MODULE_H__
#define __BAG_MODULE_H__

#include "ModuleBase.h"

class CBagModule : public CModuleBase
{
public:
	CBagModule(CPlayerObject *pOwner);

	~CBagModule();

public:
	virtual BOOL OnCreate(UINT64 u64RoleID);

	virtual BOOL OnDestroy(UINT64 u64RoleID);

	virtual BOOL OnLogin(UINT64 u64RoleID);

	virtual BOOL OnLogout(UINT64 u64RoleID);

	virtual BOOL OnNewDay();

	virtual BOOL OnLoadData(UINT64 u64RoleID);

public:
	//std::map<UINT32, CopyDataObject*>m_mapCopyData;


};

#endif //__BAG_MODULE_H__
