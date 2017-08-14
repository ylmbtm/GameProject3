#include "stdafx.h"
#include "MonsterCreator.h"
#include "Utility/Log/Log.h"
#include "Scene.h"

MonsterCreator::MonsterCreator(CScene* pScene)
{
	m_pScene = pScene;

	m_bAllFinished = FALSE;

	m_dwFinishedWave = 0;
}

MonsterCreator::~MonsterCreator()
{

}

BOOL MonsterCreator::ReadFromXml(rapidxml::xml_node<char>* pNode)
{
	for(auto pWaveNode = pNode->first_node("Wave"); pWaveNode != NULL; pWaveNode->next_sibling("Wave"))
	{
		MonsterWave Wave;
		Wave.m_dwGenType = CommonConvert::StringToInt(pWaveNode->first_attribute("gentype")->value());

		for(auto pObjectNode = pWaveNode->first_node("Object"); pObjectNode != NULL; pObjectNode->next_sibling("Object"))
		{
			MonsterData Monster;

			Monster.m_dwTypeID = CommonConvert::StringToInt(pWaveNode->first_attribute("id")->value());
			Monster.m_dwCamp = CommonConvert::StringToInt(pWaveNode->first_attribute("camp")->value());
			Monster.m_dwCount = CommonConvert::StringToInt(pWaveNode->first_attribute("num")->value());
			//Monster.m = CommonConvert::StringToInt(pWaveNode->first_attribute("strength")->value());
			Monster.m_dwDropID = CommonConvert::StringToInt(pWaveNode->first_attribute("dropid")->value());
			Wave.m_vtMonsterList.push_back(Monster);
		}
	}

	return TRUE;
}

BOOL MonsterCreator::OnUpdate(UINT32 dwTick)
{
	if(m_MonsterVaveList.size() <= 0)
	{
		m_bAllFinished = TRUE;
	}

	if(m_bAllFinished)
	{
		return TRUE;
	}

	if(m_dwFinishedWave + 1 >= m_MonsterVaveList.size())
	{
		m_bAllFinished = TRUE;
	}

	MonsterWave* pWave = &m_MonsterVaveList.at(m_dwFinishedWave + 1);
	ERROR_RETURN_FALSE(pWave != NULL);

	if(pWave)
	{
		GenMonsterWave(pWave);
		m_dwFinishedWave = m_dwFinishedWave + 1;
	}

	return TRUE;
}

BOOL MonsterCreator::GenMonsterWave( MonsterWave* pWave )
{
	std::vector<MonsterData>  m_vtMonsterList;

	for( std::vector<MonsterData>::iterator itor = pWave->m_vtMonsterList.begin(); itor != pWave->m_vtMonsterList.end(); itor++)
	{
		MonsterData* pData = &(*itor);
		m_pScene->CreateMonster(pData->m_dwTypeID, pData->m_dwCamp, pData->m_x, pData->m_y, pData->m_z, pData->m_ft);
	}

	return TRUE;
}

BOOL MonsterCreator::IsAllFinished()
{
	return m_bAllFinished;
}

BOOL MonsterCreator::GenCurrentWave()
{
	if(m_MonsterVaveList.size() <= 0)
	{
		m_bAllFinished = TRUE;
	}

	if(m_bAllFinished)
	{
		return TRUE;
	}

	if(m_dwFinishedWave + 1 >= m_MonsterVaveList.size())
	{
		m_bAllFinished = TRUE;
	}

	MonsterWave* pWave = &m_MonsterVaveList.at(m_dwFinishedWave + 1);
	ERROR_RETURN_FALSE(pWave != NULL);

	GenMonsterWave(pWave);

	m_dwFinishedWave = m_dwFinishedWave + 1;

	return TRUE;
}

