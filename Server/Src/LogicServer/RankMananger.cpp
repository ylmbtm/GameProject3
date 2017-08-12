#include <stdafx.h>
#include "RankMananger.h"
#include "..\GameServer\GameService.h"
#include "Sqlite\CppSQLite3.h"
#include "Utility\CommonFunc.h"
#include "Utility\Log\Log.h"

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
