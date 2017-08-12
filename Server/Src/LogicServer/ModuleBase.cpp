#include "stdafx.h"
#include "ModuleBase.h"
#include "PlayerObject.h"

CModuleBase::CModuleBase(CPlayerObject* pOwner)
{
	m_pOwnPlayer = pOwner;
}

CModuleBase::~CModuleBase()
{

}

BOOL CModuleBase::DispatchPacket(NetPacket* pNetPack)
{
	return TRUE;
}

BOOL CModuleBase::CalcFightValue(INT32 nValue[MAX_PROPERTY_NUM], INT32 nPercent[MAX_PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

BOOL CModuleBase::SetOwner(CPlayerObject* pOwner)
{
	m_pOwnPlayer = pOwner;
	return TRUE;
}

CPlayerObject* CModuleBase::GetOwner()
{
	return m_pOwnPlayer;
}
