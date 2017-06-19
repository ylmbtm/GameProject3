#include "stdafx.h"
#include "DBPlayerObject.h"
#include "DataBuffer/BufferHelper.h"

CDBPlayerObject::CDBPlayerObject()
{
	
}

CDBPlayerObject::~CDBPlayerObject()
{

}

BOOL CDBPlayerObject::Init()
{
	memset(m_szObjectName, 0, MAX_NAME_LEN);

	m_u64ObjectID = 0;

	m_fX = 0;
	m_fY = 0;
	m_fZ = 0;

	m_dwFeature = 0;

	m_dwLevel   = 0;

	return TRUE;
}

BOOL CDBPlayerObject::Uninit()
{

	return TRUE;
}


UINT32 CDBPlayerObject::WriteToDBPacket( CBufferHelper *pWriteBuffer )
{
	UINT32 dwSize = 0;
	dwSize += WritePlayerBaseInfo(pWriteBuffer);

	return dwSize;
}

UINT32 CDBPlayerObject::WritePlayerBaseInfo( CBufferHelper *pWriteBuffer )
{
	UINT32 dwSize = 0;
	dwSize += pWriteBuffer->Write(m_u64ObjectID);
	dwSize += pWriteBuffer->Write(m_szObjectName);
	dwSize += pWriteBuffer->Write(m_dwSceneID);
	dwSize += pWriteBuffer->Write(m_dwFeature);
	dwSize += pWriteBuffer->Write(m_fX);
	dwSize += pWriteBuffer->Write(m_fY);
	dwSize += pWriteBuffer->Write(m_fZ);

	return dwSize;
}


CDBPlayerObject* CDBPlayerObjectMgr::GetPlayer( UINT64 u64CharID )
{
	return GetByKey(u64CharID);
}

CDBPlayerObject* CDBPlayerObjectMgr::CreatePlayerByID( UINT64 u64CharID )
{
	return InsertAlloc(u64CharID);
}

BOOL CDBPlayerObjectMgr::ReleasePlayer( UINT32 u64CharID )
{
	return Delete(u64CharID);
}
