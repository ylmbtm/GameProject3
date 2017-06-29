#ifndef _SCENE_OBJECT_MANAGER_H_
#define _SCENE_OBJECT_MANAGER_H_
#include "SceneObject.h"


class CSceneObjectMgr : public std::map<UINT64, CSceneObject*>
{
public:
	CSceneObjectMgr()
	{

	}

	~CSceneObjectMgr()
	{

	}

public:
	BOOL			AddPlayer(CSceneObject *pObject);

	BOOL			RemovePlayer(UINT64 ObjectID);

	CSceneObject*   GetSceneObject(UINT64 ObjectID);
};

class CUpdateObjectMgr : public std::set<CWorldObject*>
{
public:
	CUpdateObjectMgr()
	{

	}

	~CUpdateObjectMgr()
	{

	}


public:
	BOOL AddUpdateObject(CWorldObject *pObject);

	BOOL RemoveUpdateObject(CWorldObject *pObject);

	CWorldObject* GetFisrtOjbect();
};

#endif //_SCENE_OBJECT_MANAGER_H_
