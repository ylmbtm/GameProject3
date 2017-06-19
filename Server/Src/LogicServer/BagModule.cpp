#include "stdafx.h"
#include "BagSystem.h"


BOOL CBagSystem::IsChanged()
{
	return TRUE;
}

BOOL CBagSystem::ClearChangeFlag()
{

	return TRUE;
}


UINT32 CBagSystem::WriteToBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest )
{

	return 0;
}

UINT32 CBagSystem::ReadFromBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag )
{
	return 0;
}
