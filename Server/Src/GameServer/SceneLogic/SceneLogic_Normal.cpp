#include "stdafx.h"
#include "SceneLogic_Normal.h"
#include "../GameObject/SceneObject.h"
#include "../Scene.h"
#include "../ServerData/ServerDefine.h"
#include "../Message/Game_Define.pb.h"
#include "CommonFunc.h"

SceneLogic_Normal::SceneLogic_Normal(CScene* pScene): SceneLogicBase(pScene)
{

}

SceneLogic_Normal::~SceneLogic_Normal()
{

}

BOOL SceneLogic_Normal::OnObjectCreate(CSceneObject* pObject)
{
	SceneLogicBase::OnObjectCreate(pObject);

	//玩家数据传过来了。
	if(pObject->GetObjType() == OT_PLAYER)
	{
		return TRUE;
	}

	return TRUE;
}

BOOL SceneLogic_Normal::OnObjectDie(CSceneObject* pObject)
{
	if(pObject->GetObjType() == OT_PLAYER)
	{
		m_bFinished = TRUE;
		return TRUE;
	}

	return TRUE;
}


BOOL SceneLogic_Normal::OnPlayerEnter(CSceneObject* pPlayer)
{
	//玩家进入副本了。

	return TRUE;
}

BOOL SceneLogic_Normal::OnPlayerLeave(CSceneObject* pPlayer)
{
	if(m_pScene->GetPlayerCount() != 1)
	{
		ASSERT_FAIELD;
	}

	pPlayer->m_dwResult = CR_LOST;

	//发送战斗结果
	m_bFinished = TRUE;
	return FALSE;
}



BOOL SceneLogic_Normal::Update(UINT32 dwTick)
{
	SceneLogicBase::Update(dwTick);

	if(m_pScene->GetStartTime() == 0)
	{
		if(CommonFunc::GetCurrTime() - m_pScene->GetCreateTime() > 60)
		{
			SetFinished();
		}
	}

	//判断当前战斗是不是结束
	//哪边取得了胜利

	if(BattleResultCheck())
	{

	}

	return TRUE;
}

BOOL SceneLogic_Normal::OnTimeUP()
{


	return TRUE;
}
