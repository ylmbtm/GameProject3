#include <stdafx.h>
#include "ActivityManager.h"
#include "../GameServer/GameService.h"
#include "Sqlite/CppSQLite3.h"
#include "CommonFunc.h"
#include "Log.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

CActivityManager::CActivityManager()
{

}

CActivityManager::~CActivityManager()
{

}

CActivityManager* CActivityManager::GetInstancePtr()
{
	static CActivityManager _StaticMgr;

	return &_StaticMgr;
}

