#include "stdafx.h"
#include "ModuleBase.h"
#include "PlayerObject.h"

CModuleBase::CModuleBase(CPlayerObject *pOwner)
{
	m_pOwnPlayer = pOwner;
}

CModuleBase::~CModuleBase()
{

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
