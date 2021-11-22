#include "stdafx.h"
#include "ActivityManager.h"
#include "GameService.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "TimerManager.h"
#include "../StaticData/StaticStruct.h"
#include "StaticData.h"

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


    return TRUE;
}

BOOL CActivityManager::LoadData(CppMySQL3DB& tDBConnection)
{


    return TRUE;
}

BOOL CActivityManager::OnSecondTimer()
{
	for (std::map <UINT32, StActivityInfo>::iterator itor = CStaticData::GetInstancePtr()->m_mapActivityInfo.begin();
	        itor != CStaticData::GetInstancePtr()->m_mapActivityInfo.end();
	        itor++)
	{
		StActivityInfo& ActInfo = itor->second;

		if (ActInfo.Statue == 0)
		{
			continue;
		}


	}

	return TRUE;
}

