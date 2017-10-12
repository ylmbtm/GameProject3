#include <stdafx.h>
#include "ChatManager.h"
#include "GameService.h"
#include "CommonFunc.h"
#include "Log.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

CChatManager::CChatManager()
{

}

CChatManager::~CChatManager()
{

}

CChatManager* CChatManager::GetInstancePtr()
{
	static CChatManager _StaticMgr;

	return &_StaticMgr;
}

