#ifndef _SCENELOGIC_NONE_H_
#define _SCENELOGIC_NONE_H_
#include "SceneLogic_Base.h"

class SceneLogic_None : public SceneLogicBase
{
public:
	SceneLogic_None(CScene* pScene);

	~SceneLogic_None();
public:
	virtual BOOL OnObjectCreate(CSceneObject* pPlayer);

	virtual BOOL OnObjectDie(CSceneObject* pPlayer);

	virtual BOOL OnPlayerEnter(CSceneObject* pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect);

	virtual BOOL Update(UINT64 uTick);

	virtual BOOL OnTimeUP();
};

#endif //_SCENELOGIC_NONE_H_


