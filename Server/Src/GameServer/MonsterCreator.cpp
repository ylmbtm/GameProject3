#include "stdafx.h"
#include "MonsterCreator.h"
#include "Utility/Log/Log.h"
#include "Scene.h"

MonsterCreator::MonsterCreator(CScene *pScene)
{
	m_pScene = pScene;

    m_bAllFinished = FALSE;

    m_dwFinishedWave = 0;
}

MonsterCreator::~MonsterCreator()
{

}

BOOL MonsterCreator::ReadFromXml(rapidxml::xml_node<char> *pNode)
{
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

    if(m_dwFinishedWave+1 >= m_MonsterVaveList.size())
    {
        m_bAllFinished = TRUE;
    }

    MonsterWave *pWave = &m_MonsterVaveList.at(m_dwFinishedWave+1);
    ERROR_RETURN_FALSE(pWave != NULL);

    if(pWave)
    {
        GenMonsterWave(pWave);
        m_dwFinishedWave = m_dwFinishedWave+1;
    }

	return TRUE;
}

BOOL MonsterCreator::GenMonsterWave( MonsterWave *pWave )
{
    std::vector<MonsterData>  m_vtMonsterList;

    for( std::vector<MonsterData>::iterator itor = pWave->m_vtMonsterList.begin(); itor != pWave->m_vtMonsterList.end(); itor++)
    {
		MonsterData *pData = &(*itor);
        m_pScene->GenMonster(pData->m_dwTypeID, pData->m_dwCamp,0 ,0);
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

    if(m_dwFinishedWave+1 >= m_MonsterVaveList.size())
    {
        m_bAllFinished = TRUE;
    }

    MonsterWave *pWave = &m_MonsterVaveList.at(m_dwFinishedWave+1);
    ERROR_RETURN_FALSE(pWave != NULL);

    GenMonsterWave(pWave);

    m_dwFinishedWave = m_dwFinishedWave+1;
 
    return TRUE;
}

