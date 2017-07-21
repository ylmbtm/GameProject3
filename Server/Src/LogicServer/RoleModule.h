#ifndef __ROLE_MODULE_H__
#define __ROLE_MODULE_H__
#include "ModuleBase.h"
#include "DataPool.h"
#include "..\ServerData\serverStruct.h"
#include "..\Message\Msg_Login.pb.h"

class CRoleModule : public CModuleBase
{
public:
	CRoleModule(CPlayerObject *pOwner);

	~CRoleModule();

public:
	virtual BOOL OnCreate(UINT64 u64RoleID);
	
	virtual BOOL OnDestroy();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL OnNewDay();

	virtual BOOL OnLoadData(UINT64 u64RoleID);

    BOOL InitBaseData(UINT64 u64RoleID, std::string Name, UINT32 dwActorID, UINT64 u64AccountID, UINT32 dwChannel);

    BOOL ReadFromLoginAck(DBRoleLoginAck &Ack);
public:

	RoleDataObject  *m_pRoleDataObject;
};

#endif //__ROLE_MODULE_H__
