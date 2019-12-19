#include "stdafx.h"
#include "LoginCodeMgr.h"
#include "RoleModule.h"
#include "../ServerData/RoleData.h"
#include "../Message/Msg_ID.pb.h"
#include "TimerManager.h"

CLoginCodeManager::CLoginCodeManager()
{
}

CLoginCodeManager::~CLoginCodeManager()
{
}

CLoginCodeManager* CLoginCodeManager::GetInstancePtr()
{
	static CLoginCodeManager _LoginCodeManager;

	return &_LoginCodeManager;
}

INT32 CLoginCodeManager::CreateLoginCode(UINT64 uAccountID)
{
	CLoginCodeItem* pLoginCode = InsertAlloc(uAccountID);
	pLoginCode->uAccountID = uAccountID;
	pLoginCode->nCode = rand();
	pLoginCode->uEndTime = CommonFunc::GetCurrTime() + 3;
	return pLoginCode->nCode;
}

BOOL CLoginCodeManager::CheckLoginCode(UINT64 uAccountID, INT32 nCode)
{
	CLoginCodeItem* pLoginCode = GetByKey(uAccountID);
	if (pLoginCode == NULL)
	{
		return FALSE;
	}

	if (pLoginCode->nCode != nCode)
	{
		return FALSE;
	}

	if (pLoginCode->uEndTime < CommonFunc::GetCurrTime())
	{
		return FALSE;
	}

	//任何key 只能用来比较一次就删除
	Delete(uAccountID);

	return TRUE;
}



