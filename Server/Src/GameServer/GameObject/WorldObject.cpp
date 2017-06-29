#include "stdafx.h"
#include "WorldObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "../Scene.h"


CWorldObject::CWorldObject()
{
	m_pOwnerScene = NULL;

	m_pGridNext   = NULL;

	m_pGridPrev   = NULL;
}

CWorldObject::~CWorldObject()
{
	m_pOwnerScene = NULL;

	m_pGridNext   = NULL;

	m_pGridPrev   = NULL;
}

CPosition& CWorldObject::GetPosition()
{
	return m_ObjectPos;
}

VOID CWorldObject::SetPosition( FLOAT x, FLOAT y, FLOAT z )
{
	m_ObjectPos.x = x;

	m_ObjectPos.y = y;

	m_ObjectPos.z = z;

	return ;
}

BOOL CWorldObject::SetUpdate(UpdateStatusEnum UpdateStatus)
{
	if(m_pOwnerScene != NULL)
	{
		if(UpdateStatus > m_UpdateStatus)
		{
			m_UpdateStatus = UpdateStatus;
		}

		m_pOwnerScene->AddToUpdateList(this);
	}
	else
	{
		ASSERT_FAIELD;
		return FALSE;
	}
	
	return TRUE;
}

UINT32 CWorldObject::WriteToBuffer( UINT32 dwChangeFlag, UINT32 dwDest )
{
	UINT32 dwSize = 0;

	return dwSize;
}

UINT32 CWorldObject::ReadFromBuffer( UINT32 dwChangeFlag )
{
	UINT32 dwSize = 0;	

	return dwSize;
}

UINT64 CWorldObject::GetObjectID()
{
	return 0;
}

UINT32 CWorldObject::GetObjectType()
{
	return 0;
}

CScene* CWorldObject::GetOwnerScene()
{
	return m_pOwnerScene;
}

VOID CWorldObject::SetOwnerScene( CScene *pScene )
{
	m_pOwnerScene = pScene;

	return ;
}
