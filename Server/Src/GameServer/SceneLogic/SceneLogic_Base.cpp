#include "stdafx.h"
#include "SceneLogic_Base.h"


SceneLogicBase::SceneLogicBase(CScene *pScene)
{
	m_pScene = pScene;
}

SceneLogicBase::~SceneLogicBase()
{

}

BOOL SceneLogicBase::OnCreatePlayer(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerEnter(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerLeave(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogicBase::OnCreateMonster(CSceneObject *pMonster)
{
	return TRUE;
}

BOOL SceneLogicBase::OnCreatePet(CSceneObject *pPet)
{
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerDie(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogicBase::OnMonsterDie(CSceneObject *pMonster)
{
	return TRUE;
}

BOOL SceneLogicBase::OnPetDie(CSceneObject *pPet)
{
	return TRUE;
}

BOOL SceneLogicBase::Update(UINT32 dwTick)
{


	return TRUE;
}

BOOL SceneLogicBase::OnTimeUP()
{
	return TRUE;
}

CScene* SceneLogicBase::GetScene()
{
	return m_pScene;
}
