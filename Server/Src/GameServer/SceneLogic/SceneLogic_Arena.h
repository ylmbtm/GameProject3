#ifndef _SCENELOGIC_ARENA_H_
#define _SCENELOGIC_ARENA_H_
#include "SceneLogic_Base.h"

class SceneLogic_Arena : public SceneLogicBase
{
public:
	SceneLogic_Arena(CScene* pScene);

	~SceneLogic_Arena();
public:
	virtual BOOL OnObjectCreate(CSceneObject* pPlayer);

	virtual BOOL OnObjectDie(CSceneObject* pPlayer);

	virtual BOOL OnPlayerEnter(CSceneObject* pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect);

	virtual BOOL Update(UINT64 uTick);

	virtual BOOL OnTimeUP();
};

#endif //_SCENELOGIC_BASE_H_


