#include "stdafx.h"
#include "MonsterCreator.h"
#include "Log.h"
#include "Scene.h"

MonsterCreator::MonsterCreator(CScene* pScene)
{
	m_pScene = pScene;

	m_bAllFinished = FALSE;

	m_dwCurWave = -1;
	/*
	MonsterWave Wave;
	Wave.m_dwGenType = 0;

	MonsterData Data;
	Data.m_dwActorID = 50022;
	Data.m_bCheckDie = TRUE;
	Data.m_dwCamp = 1;
	Data.m_dwType = OT_MONSTER;
	Data.m_x = 0;
	Data.m_y = 0;
	Data.m_z = 0;
	Wave.m_vtMonsterList.push_back(Data);

	Data.m_dwActorID = 50028;
	Wave.m_vtMonsterList.push_back(Data);

	Data.m_dwActorID = 50042;
	Wave.m_vtMonsterList.push_back(Data);

	Data.m_dwActorID = 50046;
	Wave.m_vtMonsterList.push_back(Data);

	Data.m_dwActorID = 50061;
	Wave.m_vtMonsterList.push_back(Data);

	Data.m_dwActorID = 50068;
	Wave.m_vtMonsterList.push_back(Data);

	Data.m_dwActorID = 60001;
	Wave.m_vtMonsterList.push_back(Data);

	Data.m_dwActorID = 80002;
	Wave.m_vtMonsterList.push_back(Data);

	m_MonsterVaveList.push_back(Wave);*/
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
			Monster.m_dwActorID = CommonConvert::StringToInt(pWaveNode->first_attribute("id")->value());
			Monster.m_dwCamp = CommonConvert::StringToInt(pWaveNode->first_attribute("camp")->value());
			Monster.m_dwCount = CommonConvert::StringToInt(pWaveNode->first_attribute("num")->value());
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

	if(m_dwCurWave >= 0)
	{
		return TRUE;
	}

	GenMonsterWave(0);
	m_dwCurWave = 0;
	return TRUE;
}

BOOL MonsterCreator::GenMonsterWave(INT32 dwWaveIndex)
{
	if(dwWaveIndex >= (INT32)m_MonsterVaveList.size())
	{
		return FALSE;
	}

	MonsterWave& Wave = m_MonsterVaveList.at(dwWaveIndex);

	std::vector<MonsterData>  m_vtMonsterList;

	for( std::vector<MonsterData>::iterator itor = Wave.m_vtMonsterList.begin(); itor != Wave.m_vtMonsterList.end(); itor++)
	{
		MonsterData* pData = &(*itor);
		m_pScene->CreateMonster(pData->m_dwActorID, pData->m_dwCamp, pData->m_x, pData->m_y, pData->m_z, pData->m_ft);
	}

	return TRUE;
}

BOOL MonsterCreator::IsAllFinished()
{
	return m_bAllFinished;
}

BOOL MonsterCreator::OnObjectDie(CSceneObject* pObject)
{
	if(m_pScene->IsMonsterAllDie())
	{
		return FALSE;
	}

	GenMonsterWave(m_dwCurWave + 1);

	return TRUE;
}

