#include "stdafx.h"
#include "WorldObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer/BufferHelper.h"



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

UINT32 CWorldObject::ReadFromBuffer( CBufferHelper *pBufHelper )
{
	UINT32 dwSize = 0;

	dwSize += CGObject::ReadFromBuffer(pBufHelper);

	dwSize += pBufHelper->Read(m_ObjectPos);


	return dwSize;
}
