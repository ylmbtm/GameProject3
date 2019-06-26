#include "stdafx.h"
#include "SceneLogic_Arena.h"
#include "../GameObject/SceneObject.h"
#include "../Scene.h"
#include "../Message/Game_Define.pb.h"

SceneLogic_Arena::SceneLogic_Arena(CScene* pScene): SceneLogicBase(pScene)
{
}

SceneLogic_Arena::~SceneLogic_Arena()
{
}

BOOL SceneLogic_Arena::OnObjectCreate(CSceneObject* pObject)
{
	//基类里帮助设定出生点坐标
	SceneLogicBase::OnObjectCreate(pObject);

	return TRUE;
}

BOOL SceneLogic_Arena::OnObjectDie(CSceneObject* pObject)
{
	return TRUE;
}


BOOL SceneLogic_Arena::OnPlayerEnter(CSceneObject* pPlayer)
{
	//玩家进入副本了。

	return TRUE;
}

BOOL SceneLogic_Arena::OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect)
{
	return FALSE;
}

BOOL SceneLogic_Arena::Update(UINT64 uTick)
{
	SceneLogicBase::Update(uTick);


	return TRUE;
}

BOOL SceneLogic_Arena::OnTimeUP()
{


	return TRUE;
}
