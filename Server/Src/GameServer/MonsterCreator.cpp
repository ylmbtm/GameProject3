#include "stdafx.h"
#include "MonsterCreator.h"

MonsterCreator::MonsterCreator(CScene *pScene)
{
	m_pScene = pScene;
}

MonsterCreator::~MonsterCreator()
{

}

BOOL MonsterCreator::OnUpdate(UINT32 dwTick)
{

	return TRUE;
}
