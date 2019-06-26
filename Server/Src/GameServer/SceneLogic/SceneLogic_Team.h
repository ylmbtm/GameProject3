#ifndef _SCENELOGIC_TEAM_H_
#define _SCENELOGIC_TEAM_H_
#include "SceneLogic_Base.h"

class SceneLogic_Team : public SceneLogicBase
{
public:
	SceneLogic_Team(CScene* pScene);

	~SceneLogic_Team();
public:
	virtual BOOL OnObjectCreate(CSceneObject* pPlayer);

	virtual BOOL OnObjectDie(CSceneObject* pPlayer);

	virtual BOOL OnPlayerEnter(CSceneObject* pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect);

	virtual BOOL Update(UINT64 uTick);

	virtual BOOL OnTimeUP();

	BOOL BattleResultCheck();
};

#endif //_SCENELOGIC_TEAM_H_


