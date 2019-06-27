#include "stdafx.h"
#include "SceneLogic_Normal.h"
#include "../GameObject/SceneObject.h"
#include "../Scene.h"
#include "../Message/Game_Define.pb.h"
#include "../ServerData/ServerDefine.h"

SceneLogic_Normal::SceneLogic_Normal(CScene* pScene): SceneLogicBase(pScene)
{

}

SceneLogic_Normal::~SceneLogic_Normal()
{

}

BOOL SceneLogic_Normal::OnObjectCreate(CSceneObject* pObject)
{
	if(pObject->GetObjType() == OT_PLAYER)
	{
		SceneLogicBase::OnObjectCreate(pObject);
	}

	return TRUE;
}

BOOL SceneLogic_Normal::OnObjectDie(CSceneObject* pObject)
{
	//对象死亡登陆进入副本了

	return TRUE;
}


BOOL SceneLogic_Normal::OnPlayerEnter(CSceneObject* pPlayer)
{
	//玩家登陆进入副本了

	return TRUE;
}

BOOL SceneLogic_Normal::OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect)
{
	//玩家离开副本了
	m_pScene->SetBattleResult(0, ECR_LOST);

	m_pScene->SendBattleResult();

	m_pScene->SetFinished();

	return FALSE;
}



BOOL SceneLogic_Normal::Update(UINT64 uTick)
{
	if (!m_pScene->IsAllDataReady())
	{
		return TRUE;
	}

	SceneLogicBase::Update(uTick);

	//如果玩家死绝了就算输了．
	if (m_pScene->IsCampAllDie(EBC_PLAYER))
	{
		m_pScene->SetBattleResult(0, ECR_LOST);

		m_pScene->SendBattleResult();

		m_pScene->SetFinished();

		return TRUE;
	}

	if(BattleResultCheck())
	{
		m_pScene->SetBattleResult(0, ECR_WIN);

		m_pScene->SendBattleResult();

		m_pScene->SetFinished();
	}

	return TRUE;
}

BOOL SceneLogic_Normal::OnTimeUP()
{


	return TRUE;
}

BOOL SceneLogic_Normal::BattleResultCheck()
{
	switch (m_BattleCondition.GetConditionType())
	{
		case EWC_KILL_ALL:
		{
			if (m_pScene->IsMonsterAllDie() && m_pScene->IsMonsterAllGen())
			{
				return TRUE;
			}
		}
		break;
		case EWC_DESTINATION:
		{

		}
		break;
		case EWC_PLAYER_ALIVE:
		{

		}
		break;
		case EWC_NPC_ALIVE:
		{

		}
		break;
		case EWC_KILL_NUM:
		{

		}
		break;
		default:
		{

		}
	}

	return FALSE;
}
