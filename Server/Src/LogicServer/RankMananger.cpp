#include "stdafx.h"
#include "RankMananger.h"
#include "GameService.h"

CRankManager::CRankManager()
{

}

CRankManager::~CRankManager()
{

}

CRankManager* CRankManager::GetInstancePtr()
{
	static CRankManager _StaticMgr;

	return &_StaticMgr;
}
