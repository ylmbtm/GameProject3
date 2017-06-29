#include "stdafx.h"
#include "ModuleBase.h"
#include "PlayerObject.h"









CModuleBase::CModuleBase()
{

}

CModuleBase::~CModuleBase()
{

}

BOOL CModuleBase::OnCreate(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CModuleBase::OnDestroy(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CModuleBase::OnLogin(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CModuleBase::OnLogout(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CModuleBase::OnNewDay()
{
	return TRUE;
}

BOOL CModuleBase::OnLoadData(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CModuleBase::IsDataOK()
{
	return m_bIsDataOK;
}

BOOL CModuleBase::SetDataOK(BOOL bOK)
{
	m_bIsDataOK = bOK;

	m_pOwnPlayer->OnModuleFnished();

	return TRUE;
}

BOOL CModuleBase::DispatchPacket(NetPacket *pNetPack)
{
	return TRUE;
}

BOOL CModuleBase::SetOwner(CPlayerObject *pOwner)
{
	m_pOwnPlayer = pOwner;
	return TRUE;
}

CPlayerObject* CModuleBase::GetOwner()
{
	return m_pOwnPlayer;
}
