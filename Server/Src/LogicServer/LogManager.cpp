#include "stdafx.h"
#include "LogManager.h"


CLogManager::CLogManager()
{

}

CLogManager::~CLogManager()
{

}

CLogManager* CLogManager::GetInstancePtr()
{
	static CLogManager _StaticMgr;

	return &_StaticMgr;
}

