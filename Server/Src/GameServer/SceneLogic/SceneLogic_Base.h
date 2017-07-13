#ifndef _SCENELOGIC_BASE_H_
#define _SCENELOGIC_BASE_H_
class CScene;
class CSceneObject;


class SceneLogicBase
{
public:
	SceneLogicBase()
	{
		
	}
	
	~SceneLogicBase()
	{
		
	}
public:
	virtual BOOL OnCreatePlayer(CSceneObject *pPlayer)
	{
		return TRUE;
	}

	virtual BOOL OnPlayerEnter(CSceneObject *pPlayer)
	{
		return TRUE;
	}

	virtual BOOL OnCreateMonster(CSceneObject *pMonster)
	{
		return TRUE;
	}

	virtual BOOL OnCreatePet(CSceneObject *pPet)
	{
		return TRUE;
	}

	virtual BOOL OnPlayerDie(CSceneObject *pPlayer)
	{
		return TRUE;
	}

	virtual BOOL OnMonsterDie(CSceneObject *pMonster)
	{
		return TRUE;
	}

	virtual BOOL OnPetDie(CSceneObject *pPet)
	{
		return TRUE;
	}

	virtual BOOL Update()
	{
		return TRUE;
	}

public:
	CScene *m_pScene;

};

#endif //_SCENELOGIC_BASE_H_


