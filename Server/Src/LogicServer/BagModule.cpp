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


BOOL CBagModule::OnDestroy(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CBagModule::OnLogin(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CBagModule::OnLogout(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CBagModule::OnNewDay()
{
	return TRUE;
}

BOOL CBagModule::OnLoadData(UINT64 u64RoleID)
{
	SetDataOK(TRUE);
	return TRUE;
}
