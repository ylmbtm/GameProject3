#include "stdafx.h"
#include "SceneLogic_Base.h"
#include "../Scene.h"
#include "BattleResult.h"

SceneLogicBase::SceneLogicBase(CScene* pScene)
{
	m_pScene = pScene;
}

SceneLogicBase::~SceneLogicBase()
{

}

BOOL SceneLogicBase::ReadFromXml(rapidxml::xml_node<char>* pNode)
{
	UINT32 dwCamp = 0;
	CPoint3D pt;
	for(auto pBornNode = pNode->first_node("DTBorn"); pBornNode != NULL; pBornNode = pBornNode->next_sibling("DTBorn"))
	{
		for(auto pAttr = pBornNode->first_attribute(); pAttr != NULL; pAttr = pAttr->next_attribute())
		{
			if(strcmp(pAttr->name(), "ID") == 0)
			{
				dwCamp = CommonConvert::StringToInt(pAttr->value());
			}
			else if(strcmp(pAttr->name(), "Pos") == 0)
			{
				pt.FromString(pAttr->value());
			}
		}

		if (dwCamp >= m_vtBornPos.size())
		{
			m_vtBornPos.resize(dwCamp + 1, pt);
		}
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
	//pObject->SetPos(m_vtBornPos[pObject->m_dwCamp].m_x, 0, m_vtBornPos[pObject->m_dwCamp].m_z);
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
	if(CommonFunc::GetCurrTime() - m_pScene->GetStartTime() > m_pScene->GetTotalTime())
	{
		OnTimeUP();
	}

	return TRUE;
}

BOOL SceneLogicBase::OnTimeUP()
{
	//默认处理逻辑，时间到了副本至成完成
	m_pScene->SetFinished();
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

