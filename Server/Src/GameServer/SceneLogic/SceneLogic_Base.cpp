#include "stdafx.h"
#include "SceneLogic_Base.h"
#include "Utility\CommonFunc.h"
#include "..\Scene.h"
#include "BattleResult.h"
#include "Utility\Log\Log.h"



SceneLogicBase::SceneLogicBase(CScene *pScene)
{
	m_pScene = pScene;

	m_bFinished = FALSE;
}

SceneLogicBase::~SceneLogicBase()
{

}

BOOL SceneLogicBase::ReadFromXml(rapidxml::xml_node<char> *pNode)
{
	for(auto pBirthNode = pNode->first_node("BirthPos"); pBirthNode != NULL; pBirthNode->next_sibling("BirthPos"))
	{
		for(auto pAttr = pBirthNode->first_attribute(); pAttr != NULL; pAttr->next_attribute())
		{
			UINT32 dwCamp = 0; 
			if(pAttr->name() == "camp")
			{
				dwCamp = CommonConvert::StringToInt(pAttr->value());
			}

			CPoint2d pt;
			if(pAttr->name() == "position")
			{
				pt.From(pAttr->value());
			}

			m_vtBirthPos[dwCamp] = pt;
		}
	}
	
	auto pResultNode = pNode->first_node("BattleResult");
	ERROR_RETURN_TRUE(pResultNode != NULL);

	auto pAttr = pResultNode->first_attribute("type");
	ERROR_RETURN_TRUE(pAttr != NULL);

	UINT32 dwType = CommonConvert::StringToInt(pAttr->value());
	ERROR_RETURN_TRUE(dwType != 0);

	switch(dwType)
	{
	case BRT_KILL_ALL:
		{
			m_pBattleResult = new BR_KillMonsterNum();

			pAttr = pNode->first_attribute("");
		}
		break;
	case BRT_REACH_POS:
		{
			m_pBattleResult = new BR_KillMonsterNum();

			pAttr = pNode->first_attribute("");
		}
		break;
	case BRT_ALIVE:
		{
			m_pBattleResult = new BR_KillMonsterNum();

			pAttr = pNode->first_attribute("");
		}
		break;
	case BRT_NPC_ALIVE:
		{
			m_pBattleResult = new BR_KillMonsterNum();

			pAttr = pNode->first_attribute("");
		}
		break;
	case BRT_KILL_NUM:
		{
			m_pBattleResult = new BR_KillMonsterNum();

			pAttr = pNode->first_attribute("");
		}
		break;
	default:
		{

		}
	}
	

	return TRUE;
}

BOOL SceneLogicBase::OnObjectCreate(CSceneObject *pObject)
{
	ERROR_RETURN_TRUE(pObject->m_dwCamp <= CT_NONE);
	ERROR_RETURN_TRUE(pObject->m_dwCamp >= CT_CMAP_END);
	pObject->SetPos(m_vtBirthPos[pObject->m_dwCamp].m_x, m_vtBirthPos[pObject->m_dwCamp].m_y);
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerEnter(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerLeave(CSceneObject *pPlayer)
{
	return FALSE;
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
	ERROR_RETURN_TRUE(m_pBattleResult != NULL);

	switch(m_pBattleResult->GetResultType())
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
