#include "stdafx.h"
#include "PlayerObject.h"

CPlayerObject::CPlayerObject()
{
	
}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::Init()
{
	memset(m_szObjectName, 0, MAX_NAME_LEN);
	m_u64ObjectID = 0;

	m_ObjectPos.x = 0;
	m_ObjectPos.y = 0;
	m_ObjectPos.z = 0;

	m_dwFeature = 0;
	m_dwLevel   = 0;

	

	return TRUE;
}

BOOL CPlayerObject::Uninit()
{

	return TRUE;
}




UINT64 CPlayerObject::GetObjectID()
{
	return m_u64ObjectID;
}


