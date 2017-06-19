#include "stdafx.h"
#include "PlayerObject.h"

CPlayerObject::CPlayerObject()
{

}

CPlayerObject::~CPlayerObject()
{

}

UINT32 CPlayerObject::ReadFromBuffer( CBufferHelper *pBufHelper )
{
	UINT32 dwSize = 0;
	dwSize += CCharObject::ReadFromBuffer(pBufHelper);

	return dwSize;
}


BOOL CPlayerObjectMgr::AddPlayer( CPlayerObject *pObject )
{
	insert(std::make_pair(pObject->GetObjectID(), pObject));

	return TRUE;
}

CPlayerObject* CPlayerObjectMgr::GetPlayer( UINT64 ObjectID )
{
	iterator itor  = find(ObjectID);
	if(itor != end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CPlayerObjectMgr::RemovePlayer( UINT64 ObjectID )
{
	erase(ObjectID);

	

	return TRUE;
}


