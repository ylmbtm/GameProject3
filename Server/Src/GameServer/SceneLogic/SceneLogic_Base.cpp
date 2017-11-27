#include "stdafx.h"
#include "SceneLogic_Base.h"
#include "CommonFunc.h"
#include "../Scene.h"
#include "BattleResult.h"
#include "Log.h"

SceneLogicBase::SceneLogicBase(CScene* pScene)
{
	m_pScene = pScene;

	m_bFinished = FALSE;
}

SceneLogicBase::~SceneLogicBase()
{

}

BOOL SceneLogicBase::ReadFromXml(rapidxml::xml_node<char>* pNode)
{
	UINT32 dwCamp = 0;
	CPoint3D pt;
	for(auto pBornNode = pNode->first_node("MapBorn"); pBornNode != NULL; pBornNode = pBornNode->next_sibling("MapBorn"))
	{
		for(auto pAttr = pBornNode->first_attribute(); pAttr != NULL; pAttr = pAttr->next_attribute())
		{
			if(strcmp(pAttr->name(), "ID") == 0)
			{
				dwCamp = CommonConvert::StringToInt(pAttr->value());
			}

			if(strcmp(pAttr->name(), "Pos") == 0)
			{
				pt.FromString(pAttr->value());
			}
		}

		m_vtBornPos[dwCamp] = pt;
	}

	return TRUE;

	auto pResultNode = pNode->first_node("BattleResult");
	ERROR_RETURN_TRUE(pResultNode != NULL);

	auto pAttr = pResultNode->first_attribute("type");
	ERROR_RETURN_TRUE(pAttr != NULL);

	UINT32 dwType = CommonConvert::StringToInt(pAttr->value());
	ERROR_RETURN_TRUE(dwType != 0);

	m_BattleResult.SetResultType(BRT_KILL_ALL);
	switch(dwType)
	{
		case BRT_DESTINATION:
		{
			m_BattleResult.SetDestination(0, 0, 0, 0);
		}
		break;
		case BRT_NPC_ALIVE:
		{
			m_BattleResult.SetNpcID(0);
		}
		break;
		case BRT_KILL_NUM:
		{
			m_BattleResult.SetKillMonster(0, 0);
		}
		break;
		default:
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL SceneLogicBase::OnObjectCreate(CSceneObject* pObject)
{
	//ERROR_RETURN_TRUE(pObject->m_dwCamp > CT_NONE);
	//ERROR_RETURN_TRUE(pObject->m_dwCamp < CT_CMAP_END);
	//pObject->SetPos(m_vtBornPos[pObject->m_dwCamp].m_x, 0, m_vtBornPos[pObject->m_dwCamp].m_y);
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerEnter(CSceneObject* pPlayer)
{
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerLeave(CSceneObject* pPlayer)
{
	return FALSE;
}


BOOL SceneLogicBase::OnObjectDie(CSceneObject* pObject)
{
	return TRUE;
}

BOOL SceneLogicBase::Update(UINT64 uTick)
{
	if(CommonFunc::GetCurrTime() - m_pScene->GetStartTime() > m_dwLastTime)
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
	return TRUE;

	switch(m_BattleResult.GetResultType())
	{
		case BRT_KILL_ALL:
		{
			//if(m_pScene->IsAllMonDie())
		}
		break;
		case BRT_DESTINATION:
		{
			//if(PlayerManager->initFengCeGift())
		}
		break;
		case BRT_PLAYER_ALIVE:
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
