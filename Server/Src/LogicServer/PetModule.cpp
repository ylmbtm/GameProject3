#include "stdafx.h"
#include "PetModule.h"

CPetModule::CPetModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CPetModule::~CPetModule()
{

}

BOOL CPetModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CPetModule::OnDestroy()
{
	return TRUE;
}

BOOL CPetModule::OnLogin()
{
	return TRUE;
}

BOOL CPetModule::OnLogout()
{
	return TRUE;
}

BOOL CPetModule::OnNewDay()
{
	return TRUE;
}

BOOL CPetModule::ReadFromLoginAck(DBRoleLoginAck &Ack)
{

	return TRUE;
}



