#include "stdafx.h"
#include "BagModule.h"


CBagModule::CBagModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CBagModule::~CBagModule()
{

}

BOOL CBagModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CBagModule::OnDestroy()
{
	return TRUE;
}

BOOL CBagModule::OnLogin()
{
	return TRUE;
}

BOOL CBagModule::OnLogout()
{
	return TRUE;
}

BOOL CBagModule::OnNewDay()
{
	return TRUE;
}

BOOL CBagModule::ReadFromLoginAck( DBRoleLoginAck &Ack )
{
	

	return TRUE;
}
