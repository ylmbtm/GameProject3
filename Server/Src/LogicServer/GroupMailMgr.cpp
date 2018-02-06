#include "stdafx.h"
#include "GroupMailMgr.h"
#include "GameService.h"
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

BOOL CGroupMailMgr::LoadGroupMailData(CppMySQL3DB& tDBConnection)
{
	return TRUE;
}
