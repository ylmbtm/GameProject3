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

BOOL CModuleBase::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

INT64 CModuleBase::GetProperty(ERoleProperty ePropertyID)
{
	return 0;
}

BOOL CModuleBase::AddChangeID(UINT64 uID)
{
	m_setChange.insert(uID);

	return TRUE;
}

BOOL CModuleBase::AddRemoveID(UINT64 uID)
{
	m_setRemove.insert(uID);

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
