#include "stdafx.h"
#include "SceneLogic_Base.h"
#include "Utility\CommonFunc.h"
#include "..\Scene.h"


SceneLogicBase::SceneLogicBase(CScene *pScene)
{
	m_pScene = pScene;

	m_bFinished = FALSE;
}

SceneLogicBase::~SceneLogicBase()
{

}

BOOL SceneLogicBase::OnObjectCreate(CSceneObject *pObject)
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


BOOL SceneLogicBase::OnObjectDie(CSceneObject *pObject)
{
	return TRUE;
}

BOOL SceneLogicBase::Update(UINT32 dwTick)
{
	if(CommonFunc::GetCurrTime()- m_pScene->GetStartTime() > m_dwLastTime)
	{
		OnTimeUP();
	}

	return TRUE;
}

BOOL SceneLogicBase::OnTimeUP()
{
	return TRUE;
}

BOOL SceneLogicBase::IsFinished()
{
	return m_bFinished;
}

BOOL SceneLogicBase::SetLastTime(UINT32 dwTime)
{
	m_dwLastTime = dwTime;

	return TRUE;
}

BOOL SceneLogicBase::BattleResultCheck()
{
	switch(m_dwResultType)
	{
	case BRT_KILL_ALL:
		{
			//if(m_pScene->IsAllMonDie())
		}
		break;
	case BRT_REACH_POS:
		{
			//if(PlayerManager->initFengCeGift())
		}
		break;
	case BRT_ALIVE:
		{
			//if(is->isaaa)
			//{

			//}
		}
		break;
	case BRT_NPC_ALIVE:
		{

		}
		break;
	case BRT_KILL_NUM:
		{

		}
		break;
	default:
		{

		}
	}

	return FALSE;
}

CScene* SceneLogicBase::GetScene()
{
	return m_pScene;
}

BOOL SceneLogicBase::SetFinished()
{
    m_bFinished = TRUE;

    return TRUE;
}
