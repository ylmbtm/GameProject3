#include <stdafx.h>
#include "ChatManager.h"
#include "..\GameServer\GameService.h"
#include "Sqlite\CppSQLite3.h"
#include "Utility\CommonFunc.h"
#include "Utility\Log\Log.h"
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

