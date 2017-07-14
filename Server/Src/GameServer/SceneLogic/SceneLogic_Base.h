#ifndef _SCENELOGIC_BASE_H_
#define _SCENELOGIC_BASE_H_
class CScene;
class CSceneObject;


class SceneLogicBase
{
public:
	SceneLogicBase(CScene *pScene);
	
	~SceneLogicBase();
public:
	virtual BOOL OnCreatePlayer(CSceneObject *pPlayer);

	virtual BOOL OnPlayerEnter(CSceneObject *pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject *pPlayer);

	virtual BOOL OnCreateMonster(CSceneObject *pMonster);

	virtual BOOL OnCreatePet(CSceneObject *pPet);

	virtual BOOL OnPlayerDie(CSceneObject *pPlayer);

	virtual BOOL OnMonsterDie(CSceneObject *pMonster);

	virtual BOOL OnPetDie(CSceneObject *pPet);

	virtual BOOL Update(UINT32 dwTick);

	virtual BOOL OnTimeUP();

	CScene*  GetScene();

public:
	CScene *m_pScene;

};

#endif //_SCENELOGIC_BASE_H_


