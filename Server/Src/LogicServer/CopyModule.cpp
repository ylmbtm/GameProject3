#include "stdafx.h"
#include "CopyModule.h"

CCopyModule::CCopyModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CCopyModule::~CCopyModule()
{

}

BOOL CCopyModule::OnCreate(UINT64 u64RoleID)
{
	
	return TRUE;
}


BOOL CCopyModule::OnDestroy()
{
	return TRUE;
}

BOOL CCopyModule::OnLogin()
{
	return TRUE;
}

BOOL CCopyModule::OnLogout()
{
	return TRUE;
}

BOOL CCopyModule::OnNewDay()
{
	return TRUE;
}

BOOL CCopyModule::ReadFromLoginAck(DBRoleLoginAck &Ack)
{

	return TRUE;
}
