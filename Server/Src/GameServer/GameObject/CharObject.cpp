#include "stdafx.h"
#include "CharObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"



CCharObject::CCharObject()
{
	memset(m_szObjectName, 0 , MAX_NAME_LEN);
}

CCharObject::~CCharObject()
{

}

BOOL CCharObject::IsChanged()
{

	return TRUE;
}

BOOL CCharObject::ClearChangeFlag()
{

	return TRUE;
}

UINT32 CCharObject::WriteToBuffer(  UINT32 dwChangeFlag, UINT32 dwDest )
{
	return 0;
}

UINT32 CCharObject::ReadFromBuffer( UINT32 dwChangeFlag )
{
	return 0;
}

char* CCharObject::GetObjectName()
{
	return m_szObjectName;
}

void CCharObject::SetObjectName( char *szName )
{
	if(szName == NULL)
	{
		ASSERT_FAIELD;
		return ;
	}

	strncpy(m_szObjectName, szName, MAX_NAME_LEN);
}
