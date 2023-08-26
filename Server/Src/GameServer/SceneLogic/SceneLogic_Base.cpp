#include "stdafx.h"
#include "CommonMath.h"
#include "SceneLogic_Base.h"
#include "../Scene.h"
#include "WinCondition.h"

SceneLogicBase::SceneLogicBase(CScene* pScene)
{
    m_pScene = pScene;
    m_vtBornPos.reserve(10);
}

SceneLogicBase::~SceneLogicBase()
{

}

BOOL SceneLogicBase::ReadFromXml(rapidxml::xml_node<char>* pNode)
{
	ReadBornFromXml(pNode);

	ReadConditionFromXml(pNode);

	return TRUE;
}

BOOL SceneLogicBase::OnObjectCreate(CSceneObject* pObject)
{
	if (pObject->m_dwCamp >= m_vtBornPos.size() || pObject->m_dwCamp <= 0)
	{
		pObject->SetPos(m_vtBornPos[0].m_x, m_vtBornPos[0].m_y, m_vtBornPos[0].m_z);
	}
	else
	{
		pObject->SetPos(m_vtBornPos[pObject->m_dwCamp].m_x, m_vtBornPos[pObject->m_dwCamp].m_y, m_vtBornPos[pObject->m_dwCamp].m_z);
	}
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerEnter(CSceneObject* pPlayer)
{
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect)
{
	return FALSE;
}

BOOL SceneLogicBase::OnObjectDie(CSceneObject* pObject)
{
	return TRUE;
}

BOOL SceneLogicBase::Update(UINT64 uTick)
{
	if (m_pScene->GetStartTime() <= 0)
	{
		//如果创建完成60秒副本还没有开始，就要超时结束
		if (CommonFunc::GetCurrTime() - m_pScene->GetCreateTime() > 60)
		{
			OnTimeUP();
			m_pScene->SetFinished();
		}
	}
	else
	{
		if (CommonFunc::GetCurrTime() - m_pScene->GetStartTime() > m_pScene->GetTotalTime())
		{
			OnTimeUP();
			m_pScene->SetFinished();
		}
	}
	return TRUE;
}

CScene* SceneLogicBase::GetScene()
{
	return m_pScene;
}

BOOL SceneLogicBase::ReadBornFromXml(rapidxml::xml_node<char>* pNode)
{
	auto pBornListNode = pNode->first_node("MapBorns");
	ERROR_RETURN_FALSE(pBornListNode != NULL);

	INT32 nCamp = 0;
	CPoint3D pt;
	for (auto pBornNode = pBornListNode->first_node("DTBorn"); pBornNode != NULL; pBornNode = pBornNode->next_sibling("DTBorn"))
	{
		for (auto pAttr = pBornNode->first_attribute(); pAttr != NULL; pAttr = pAttr->next_attribute())
		{
			if (strcmp(pAttr->name(), "ID") == 0)
			{
				nCamp = CommonConvert::StringToInt(pAttr->value());
			}
			else if (strcmp(pAttr->name(), "Pos") == 0)
			{
				pt.FromString(pAttr->value());
			}
		}

		if (nCamp >= m_vtBornPos.size())
		{
			m_vtBornPos.resize(nCamp + 1, pt);
		}
		else
		{
			m_vtBornPos[nCamp] = pt;
		}
	}

	return TRUE;
}

BOOL SceneLogicBase::ReadConditionFromXml(rapidxml::xml_node<char>* pNode)
{
	auto pConditionList = pNode->first_node("MapConditions");
	if (pConditionList == NULL)
	{
		return TRUE;
	}

	auto pCondtionNode = pConditionList->first_node("DTCondition");
	ERROR_RETURN_FALSE(pCondtionNode != NULL);

	auto pAttr = pCondtionNode->first_attribute("WinType");
	ERROR_RETURN_FALSE(pAttr != NULL);

	EWinCondition eType = (EWinCondition)CommonConvert::StringToInt(pAttr->value());
	ERROR_RETURN_FALSE(eType != 0);

	m_BattleCondition.SetConditionType(eType);

	switch (eType)
	{
		case EWC_DESTINATION:
		{
			Rect2D tRc;
			CommonConvert::StringToBox(pCondtionNode->first_attribute("DestBox")->value(), tRc.m_fLeft, tRc.m_fTop, tRc.m_fRight, tRc.m_fBottom);
			m_BattleCondition.SetDestination(tRc.m_fLeft, tRc.m_fTop, tRc.m_fRight, tRc.m_fBottom);
		}
		break;
		case EWC_NPC_ALIVE:
		{
			auto pAttrNpc = pCondtionNode->first_attribute("NpcID");
			ERROR_RETURN_FALSE(pAttrNpc != NULL);
			m_BattleCondition.SetNpcID(CommonConvert::StringToInt(pAttrNpc->value()));
		}
		break;
		case EWC_KILL_NUM:
		{
			auto pAttrID = pCondtionNode->first_attribute("KillMonsterID");
			ERROR_RETURN_FALSE(pAttrID != NULL);

			auto pAttrNum = pCondtionNode->first_attribute("KillMonsterNum");
			ERROR_RETURN_FALSE(pAttrNum != NULL);
			m_BattleCondition.SetKillMonster(CommonConvert::StringToInt(pAttrID->value()), CommonConvert::StringToInt(pAttrNum->value()));
		}
		break;
		case EWC_PLAYER_ALIVE:
		{
		}
		break;
		case EWC_KILL_ALL:
		{
		}
		break;
		default:
		{
			return FALSE;
		}
	}

	return TRUE;
}

