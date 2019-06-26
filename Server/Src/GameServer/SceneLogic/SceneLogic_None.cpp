#include "stdafx.h"
#include "SceneLogic_None.h"
#include "../Scene.h"




SceneLogic_None::SceneLogic_None(CScene* pScene): SceneLogicBase(pScene)
{

}

SceneLogic_None::~SceneLogic_None()
{

}

BOOL SceneLogic_None::OnObjectCreate(CSceneObject* pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnObjectDie(CSceneObject* pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnPlayerEnter(CSceneObject* pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect)
{
	return FALSE;
}

BOOL SceneLogic_None::Update(UINT64 uTick)
{
	SceneLogicBase::Update(uTick);

	return TRUE;
}

BOOL SceneLogic_None::OnTimeUP()
{
	return TRUE;
}
