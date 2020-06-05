#include "stdafx.h"
#include "GameLogManager.h"

CGameLogManager::CGameLogManager()
{

}

CGameLogManager::~CGameLogManager()
{

}

CGameLogManager* CGameLogManager::GetInstancePtr()
{
	static CGameLogManager _LogManager;

	return &_LogManager;
}
