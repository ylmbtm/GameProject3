#include "stdafx.h"
#include "RankMananger.h"
#include "GameService.h"

CRankManager* CRankManager::GetInstancePtr()
{
	static CRankManager _StaticMgr;

	return &_StaticMgr;
}

CRankManager::CRankManager()
{
	m_LevelRanker.InitRanker(50, 200);

	m_FightRanker.InitRanker(50, 200);
}

CRankManager::~CRankManager()
{

}

BOOL CRankManager::LoadData(CppMySQL3DB& tDBConnection)
{

	return TRUE;
}


