#include "stdafx.h"
#include "SceneLogic_Normal.h"
#include "..\GameObject\SceneObject.h"
#include "..\Scene.h"
#include "..\ServerData\ServerDefine.h"

SceneLogic_Normal::SceneLogic_Normal(CScene *pScene):SceneLogicBase(pScene)
{

}

SceneLogic_Normal::~SceneLogic_Normal()
{

}

BOOL SceneLogic_Normal::OnCreatePlayer(CSceneObject *pPlayer)
{
	//玩家数据传过来了。

	return TRUE;
}

BOOL SceneLogic_Normal::OnPlayerEnter(CSceneObject *pPlayer)
{
	//玩家进入副本了。

	return TRUE;
}

BOOL SceneLogic_Normal::OnPlayerLeave(CSceneObject *pPlayer)
{
	if(m_pScene->GetPlayerCount() != 1)
	{
		ASSERT_FAIELD;
	}

	pPlayer->m_dwResult = CR_LOST;

	//发送战斗结果
	m_pScene->SetFinished();
	return FALSE;
}

BOOL SceneLogic_Normal::OnCreateMonster(CSceneObject *pMonster)
{
	return TRUE;
}

BOOL SceneLogic_Normal::OnCreatePet(CSceneObject *pPet)
{
	return TRUE;
}

BOOL SceneLogic_Normal::OnPlayerDie()
{
	return TRUE;
}

BOOL SceneLogic_Normal::OnMonsterDie()
{
	return TRUE;
}

BOOL SceneLogic_Normal::OnPetDie()
{
	return TRUE;
}

BOOL SceneLogic_Normal::Update(UINT32 dwTick)
{
	//判断当前战斗是不是结束
	//哪边取得了胜利

	return TRUE;
}
