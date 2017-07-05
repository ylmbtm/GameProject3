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


BOOL CCopyModule::OnDestroy(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CCopyModule::OnLogin(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CCopyModule::OnLogout(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CCopyModule::OnNewDay()
{
	return TRUE;
}

BOOL CCopyModule::OnLoadData(UINT64 u64RoleID)
{
	SetDataOK(TRUE);
	return TRUE;
}
