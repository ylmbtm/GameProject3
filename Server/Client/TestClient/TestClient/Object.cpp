#include "stdafx.h"
#include "Object.h"
#include "DataBuffer\BufferHelper.h"


CGObject::CGObject()
{
	m_u64ObjectID = 0;
}

CGObject::~CGObject()
{

}

UINT8 CGObject::GetObjectType()
{
	return (UINT8)(m_u64ObjectID>>56);
}

UINT64 CGObject::GetObjectID()
{
	return m_u64ObjectID;
}

VOID CGObject::SetObjectID( UINT64 u64ObjectID )
{
	m_u64ObjectID = u64ObjectID;

	return ;
}

UINT32 CGObject::ReadFromBuffer( CBufferHelper *pBufHelper )
{
	UINT32 dwSize = 0;
	dwSize += pBufHelper->Read(m_u64ObjectID);



	return dwSize;
}
