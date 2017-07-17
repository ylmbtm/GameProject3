#include "stdafx.h"
#include "SceneLogic_None.h"




SceneLogic_None::SceneLogic_None(CScene *pScene):SceneLogicBase(pScene)
{

}

SceneLogic_None::~SceneLogic_None()
{

}

BOOL SceneLogic_None::OnObjectCreate(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnObjectDie(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnPlayerEnter(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnPlayerLeave(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::Update(UINT32 dwTick)
{
	return TRUE;
}
