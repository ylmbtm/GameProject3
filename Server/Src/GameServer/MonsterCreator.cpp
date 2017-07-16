#include "stdafx.h"
#include "MonsterCreator.h"
#include "Utility/Log/Log.h"
#include "Scene.h"

MonsterCreator::MonsterCreator(CScene *pScene)
{
	m_pScene = pScene;

    m_bAllFinished = FALSE;
}

MonsterCreator::~MonsterCreator()
{

}

BOOL MonsterCreator::OnUpdate(UINT32 dwTick)
{
    if(m_bAllFinished)
    {
        return TRUE;
    }

    m_bAllFinished = TRUE;

    std::vector<MonsterWave>::iterator itor = m_MonsterVaveList.begin();
    for(; itor != m_MonsterVaveList.end(); itor++)
    {
        MonsterWave *pData = &(*itor);
        FALSE_CONTINUE(!pData->IsFinished());

        if(!pData->IsReady())
        {
            m_bAllFinished = FALSE;
            continue;
        }

        GenMonsterWave(pData);

        pData->SetFinished();
    }

    

	return TRUE;
}

BOOL MonsterCreator::GenMonsterWave( MonsterWave *pWave )
{
    std::vector<MonsterData>  m_vtMonsterList;

    for( std::vector<MonsterData>::iterator itor = pWave->m_vtMonsterList.begin(); itor != pWave->m_vtMonsterList.end(); itor++)
    {
        m_pScene->GenMonster((*itor).m_dwTypeID);
    }

    return TRUE;
}

BOOL MonsterCreator::IsAllFinished()
{
    return m_bAllFinished;
}

BOOL MonsterWave::IsFinished()
{
    return m_bFinished;
}

BOOL MonsterWave::SetFinished()
{
    m_bFinished = TRUE;

    return TRUE;
}

BOOL MonsterWave::IsReady()
{
    return m_bReady;
}
