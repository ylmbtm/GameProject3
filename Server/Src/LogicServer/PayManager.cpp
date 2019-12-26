#include "stdafx.h"
#include "PayManager.h"
#include "GameService.h"

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

BOOL CPayManager::Init()
{
	return TRUE;
}

BOOL CPayManager::OnMsgRechargeMoney(INT32 nDiamond, int nProductID)
{

	return TRUE;
}
