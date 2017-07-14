#ifndef _SCENELOGIC_NORMAL_H_
#define _SCENELOGIC_NORMAL_H_
#include "SceneLogic_Base.h"

class SceneLogic_Normal : public SceneLogicBase
{
public:
	SceneLogic_Normal(CScene *pScene);
	
	~SceneLogic_Normal();
public:
	virtual BOOL OnCreatePlayer(CSceneObject *pPlayer);

	virtual BOOL OnPlayerEnter(CSceneObject *pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject *pPlayer);

	virtual BOOL OnCreateMonster(CSceneObject *pMonster);

	virtual BOOL OnCreatePet(CSceneObject *pPet);

	virtual BOOL OnPlayerDie();

	virtual BOOL OnMonsterDie();

	virtual BOOL OnPetDie();

	virtual BOOL Update(UINT32 dwTick);

};

#endif //_SCENELOGIC_BASE_H_


