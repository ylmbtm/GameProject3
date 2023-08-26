#include "stdafx.h"
#include "RapidXml.h"
#include "CommonMath.h"
#include "MonsterCreator.h"
#include "Scene.h"

MonsterCreator::MonsterCreator(CScene* pScene)
{
	m_pScene = pScene;
}

MonsterCreator::~MonsterCreator()
{

}

BOOL MonsterCreator::ReadFromXml(rapidxml::xml_node<char>* pNode)
{
	for(auto pWaveNode = pNode->first_node("DTWave"); pWaveNode != NULL; pWaveNode = pWaveNode->next_sibling("DTWave"))
	{
		MonsterWave Wave;

		char* pValue = pWaveNode->first_attribute("TriggerType")->value();
		ERROR_CONTINUE_EX(pValue  != NULL);

		Wave.m_dwTriggerType = CommonConvert::StringToInt(pValue);

		pValue = pWaveNode->first_attribute("TriggerBox")->value();
		ERROR_CONTINUE_EX(pValue != NULL);
		CommonConvert::StringToBox(pValue, Wave.m_TriggerBox.m_fLeft, Wave.m_TriggerBox.m_fTop, Wave.m_TriggerBox.m_fRight, Wave.m_TriggerBox.m_fBottom);
		for(auto pObjectNode = pWaveNode->first_node("DTMonster"); pObjectNode != NULL; pObjectNode = pObjectNode->next_sibling("DTMonster"))
		{
			MonsterData Monster;
			Monster.m_dwActorID = CommonConvert::StringToInt(pObjectNode->first_attribute("MonsterID")->value());
			CommonConvert::StringToPos(pObjectNode->first_attribute("Pos")->value(), Monster.m_x, Monster.m_y, Monster.m_z);
			Monster.m_ft = CommonConvert::StringToFloat(pObjectNode->first_attribute("Face")->value());
			Monster.m_dwCamp = CommonConvert::StringToInt(pObjectNode->first_attribute("Camp")->value());
			//Monster.m_dwDropID = CommonConvert::StringToInt(pObjectNode->first_attribute("dropid")->value());
			Wave.m_vtMonsterList.push_back(Monster);
		}

		m_MonsterVaveList.push_back(Wave);
	}

	return TRUE;
}

BOOL MonsterCreator::OnUpdate(UINT64 uTick)
{
	for (int i = 0; i < m_MonsterVaveList.size(); i++)
	{
		MonsterWave& Wave = m_MonsterVaveList.at(i);
		if (Wave.m_bTriggerDone == TRUE)
		{
			continue;
		}

		if (Wave.m_dwTriggerType == 0)
		{
			GenMonsterWave(i);

			return TRUE;
		}
	}

	return TRUE;
}

BOOL MonsterCreator::GenMonsterWave(INT32 nWaveIndex)
{
	if(nWaveIndex >= (INT32)m_MonsterVaveList.size())
	{
		return FALSE;
	}

	MonsterWave& Wave = m_MonsterVaveList.at(nWaveIndex);

	for( std::vector<MonsterData>::iterator itor = Wave.m_vtMonsterList.begin(); itor != Wave.m_vtMonsterList.end(); itor++)
	{
		MonsterData* pData = &(*itor);
		CSceneObject* pSceneObject = m_pScene->CreateMonster(pData->m_dwActorID, pData->m_dwCamp, pData->m_x, pData->m_y, pData->m_z, pData->m_ft);
		ERROR_RETURN_FALSE(pSceneObject != NULL);
		pData->m_uObjectGuid = pSceneObject->GetObjectGUID();
		pData->m_bDead = FALSE;
	}

	Wave.m_bTriggerDone = TRUE;

	return TRUE;
}

BOOL MonsterCreator::IsAllFinished()
{
	for (int i = 0; i < m_MonsterVaveList.size(); i++)
	{
		if (!m_MonsterVaveList.at(i).m_bTriggerDone)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL MonsterCreator::OnObjectDie(CSceneObject* pObject)
{
	return TRUE;
}

BOOL MonsterCreator::OnPlayerMove(FLOAT x, FLOAT z)
{
	if (IsAllFinished())
	{
		return TRUE;
	}

	for (int i = 0; i < m_MonsterVaveList.size(); i++)
	{
		MonsterWave& Wave = m_MonsterVaveList.at(i);
		if (Wave.m_bTriggerDone == TRUE)
		{
			continue;
		}

		if (Wave.m_dwTriggerType != 1)
		{
			continue;
		}

		if (Wave.m_TriggerBox.Contains(CPoint2D(x, z)))
		{
			GenMonsterWave(i);
		}
	}

	return TRUE;
}

