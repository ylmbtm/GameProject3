#ifndef _SCENELOGIC_NORMAL_H_
#define _SCENELOGIC_NORMAL_H_
#include "SceneLogic_Base.h"

class SceneLogic_Normal : public SceneLogicBase
{
public:
	SceneLogic_Normal(CScene* pScene);

	~SceneLogic_Normal();
public:
	virtual BOOL OnObjectCreate(CSceneObject* pPlayer);

	virtual BOOL OnObjectDie(CSceneObject* pPlayer);

	virtual BOOL OnPlayerEnter(CSceneObject* pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect);

	virtual BOOL Update(UINT64 uTick);

	virtual BOOL OnTimeUP();

	BOOL BattleResultCheck();
};

#endif //_SCENELOGIC_NORMAL_H_


