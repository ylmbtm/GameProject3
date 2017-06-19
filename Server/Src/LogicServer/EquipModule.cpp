#include "stdafx.h"
#include "EquipmentSystem.h"



BOOL CEquipmentSystem::IsChanged()
{
	return TRUE;
}

BOOL CEquipmentSystem::ClearChangeFlag()
{

	return TRUE;
}


UINT32 CEquipmentSystem::WriteToBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest )
{

	return 0;
}

UINT32 CEquipmentSystem::ReadFromBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag )
{
	return 0;
}
