#include <stdafx.h>
#include "GroupMailMgr.h"
#include "..\GameServer\GameService.h"
#include "Sqlite\CppSQLite3.h"
#include "Utility\CommonFunc.h"
#include "Utility\Log\Log.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

CGroupMailMgr::CGroupMailMgr()
{

}

CGroupMailMgr::~CGroupMailMgr()
{

}

CGroupMailMgr* CGroupMailMgr::GetInstancePtr()
{
	static CGroupMailMgr _StaticMgr;

	return &_StaticMgr;
}

BOOL CGroupMailMgr::SendGroupMail(GroupMailDataObject* pGroupMail)
{
	return TRUE;
}
