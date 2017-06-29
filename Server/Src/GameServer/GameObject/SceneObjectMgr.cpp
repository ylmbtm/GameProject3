#include "stdafx.h"
#include "SceneObjectMgr.h"


BOOL CSceneObjectMgr::AddPlayer( CSceneObject *pObject )
{
	insert(std::make_pair(pObject->GetObjectID(), pObject));

	return TRUE;
}

CSceneObject* CSceneObjectMgr::GetSceneObject( UINT64 ObjectID )
{
	iterator itor  = find(ObjectID);
	if(itor != end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CSceneObjectMgr::RemovePlayer( UINT64 ObjectID )
{
	if(GetSceneObject(ObjectID) == NULL)
	{
		ASSERT_FAIELD;
	}

	erase(ObjectID);

	return TRUE;
}

BOOL CUpdateObjectMgr::AddUpdateObject( CWorldObject *pObject )
{
	insert(pObject);

	return TRUE;
}

CWorldObject* CUpdateObjectMgr::GetFisrtOjbect()
{
	iterator itor = begin();
	if(itor == end())
	{
		return NULL;
	}

	CWorldObject *pWorldObject = *itor;

	erase(itor);

	return pWorldObject;
}

BOOL CUpdateObjectMgr::RemoveUpdateObject( CWorldObject *pObject )
{
	iterator itor = find(pObject);
	if(itor == end())
	{
		return FALSE;
	}

	erase(itor);

	return TRUE;
}


