#include "stdafx.h"
#include "WorldObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"

CWorldObject::CWorldObject()
{

}

CWorldObject::~CWorldObject()
{

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


UINT64 CWorldObject::GetObjectID()
{
	return 0;
}
