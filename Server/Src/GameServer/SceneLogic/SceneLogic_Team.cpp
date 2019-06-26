#include "stdafx.h"
#include "SceneLogic_Team.h"
#include "../GameObject/SceneObject.h"
#include "../Scene.h"
#include "../Message/Game_Define.pb.h"

SceneLogic_Team::SceneLogic_Team(CScene* pScene): SceneLogicBase(pScene)
{

}

SceneLogic_Team::~SceneLogic_Team()
{

}

BOOL SceneLogic_Team::OnObjectCreate(CSceneObject* pObject)
{
	SceneLogicBase::OnObjectCreate(pObject);
	//玩家数据传过来了。
	if(pObject->GetObjType() == OT_PLAYER)
	{
		ERROR_RETURN_TRUE(pObject->m_dwCamp > 0);
		ERROR_RETURN_TRUE(pObject->m_dwCamp < m_vtBornPos.size());
		pObject->SetPos(m_vtBornPos[pObject->m_dwCamp].m_x, m_vtBornPos[pObject->m_dwCamp].m_y, m_vtBornPos[pObject->m_dwCamp].m_z);

		return TRUE;
	}

	return TRUE;
}

BOOL SceneLogic_Team::OnObjectDie(CSceneObject* pObject)
{
	//对象死亡登陆进入副本了

	return TRUE;
}


BOOL SceneLogic_Team::OnPlayerEnter(CSceneObject* pPlayer)
{
	//玩家登陆进入副本了

	return TRUE;
}

BOOL SceneLogic_Team::OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect)
{
	//玩家离开副本了
	m_pScene->BroadRemoveObject(pPlayer);

	m_pScene->DeletePlayer(pPlayer->GetObjectGUID());

	if (!bDisConnect)
	{
		m_pScene->BackToMainCity(pPlayer->GetObjectGUID());
	}

	return FALSE;
}



BOOL SceneLogic_Team::Update(UINT64 uTick)
{
	SceneLogicBase::Update(uTick);

	//如果玩家死绝了就算输了．
	if (m_pScene->IsCampAllDie(EBC_PLAYER))
	{
		m_pScene->SetFinished();




		return TRUE;
	}

	if(BattleResultCheck())
	{

	}

	return TRUE;
}

BOOL SceneLogic_Team::OnTimeUP()
{


	return TRUE;
}

BOOL SceneLogic_Team::BattleResultCheck()
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
			//if(PlayerManager->initFengCeGift())
		}
		break;
		case EWC_PLAYER_ALIVE:
		{
			//if(is->isaaa)
			//{

			//}
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
