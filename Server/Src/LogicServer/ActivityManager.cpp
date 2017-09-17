#include <stdafx.h>
#include "ActivityManager.h"
#include "../GameServer/GameService.h"
#include "Sqlite/CppSQLite3.h"
#include "CommonFunc.h"
#include "Log.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "TimerManager.h"
#include "../ConfigData/ConfigStruct.h"
#include "../ConfigData/ConfigData.h"

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

BOOL CActivityManager::Init()
{
	TimerManager::GetInstancePtr()->AddFixTimer(0, 1, &CActivityManager::ActivityTimer, this);

	return TRUE;
}

BOOL CActivityManager::ActivityTimer(UINT32 dwData)
{
	for(std::map <UINT32, StActivityInfo>::iterator itor = CConfigData::GetInstancePtr()->m_mapActivityInfo.begin();
	        itor != CConfigData::GetInstancePtr()->m_mapActivityInfo.end();
	        itor++)
	{
		StActivityInfo& ActInfo = itor->second;

		if(ActInfo.Statue == 0)
		{
			continue;
		}


	}

	return TRUE;
}
