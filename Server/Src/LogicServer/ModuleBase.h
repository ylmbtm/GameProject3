#ifndef __CMODULE_BASE_H__
#define __CMODULE_BASE_H__
#include "Utility/AVLTree.h"
#include "Utility/Position.h"
#include "../Message/Msg_LoginData.pb.h"

class CPlayerObject;

class CModuleBase
{
public:
	CModuleBase(CPlayerObject *pOwner);

	~CModuleBase();

	virtual BOOL OnCreate(UINT64 u64RoleID) = 0;

	virtual BOOL OnDestroy() = 0;

	virtual BOOL OnLogin() = 0;

	virtual BOOL OnLogout() = 0;

	virtual BOOL OnNewDay() = 0;

	virtual BOOL ReadFromLoginAck(DBRoleLoginAck &Ack) = 0;

	virtual BOOL DispatchPacket(NetPacket *pNetPack);

	BOOL SetOwner(CPlayerObject *pOwner);

	CPlayerObject* GetOwner();

public:
	CPlayerObject *m_pOwnPlayer;
};


#endif //__CMODULE_BASE_H__
