#include <stdafx.h>
#include "PayManager.h"
#include "GameService.h"
#include "Sqlite/CppSQLite3.h"
#include "CommonFunc.h"
#include "Log.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

CPayManager::CPayManager()
{

}

CPayManager::~CPayManager()
{

}

CPayManager* CPayManager::GetInstancePtr()
{
	static CPayManager _StaticMgr;

	return &_StaticMgr;
}

BOOL CPayManager::InitPayManager()
{
	return TRUE;
}

BOOL CPayManager::OnMsgRechargeMoney(INT32 nDiamond, int nProductID)
{

	return TRUE;
}
