#include "stdafx.h"
#include "EquipModule.h"

CEquipModule::CEquipModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CEquipModule::~CEquipModule()
{

}

BOOL CEquipModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CEquipModule::OnDestroy()
{
	return TRUE;
}

BOOL CEquipModule::OnLogin()
{
	return TRUE;
}

BOOL CEquipModule::OnLogout()
{
	return TRUE;
}

BOOL CEquipModule::OnNewDay()
{
	return TRUE;
}

BOOL CEquipModule::ReadFromLoginAck(DBRoleLoginAck &Ack)
{

	return TRUE;
}



