#include "stdafx.h"
#include "LoginCodeMgr.h"
#include "RoleModule.h"
#include "RoleData.h"
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
    CLoginCodeItem* pLoginCode = GetByKey(uAccountID);
    if (pLoginCode == NULL)
    {
        pLoginCode = InsertAlloc(uAccountID);
    }

    ERROR_RETURN_VALUE(pLoginCode != 0, 0);

    pLoginCode->uAccountID = uAccountID;
    pLoginCode->nLoginCode = rand();
    pLoginCode->uEndTime = CommonFunc::GetCurrTime() + 10;
    return pLoginCode->nLoginCode;
}

BOOL CLoginCodeManager::CheckLoginCode(UINT64 uAccountID, INT32 nCode)
{
    //任何key 只能用来比较一次就删除
    CLoginCodeItem* pLoginCode = GetByKey(uAccountID);
    if (pLoginCode == NULL)
    {
        return FALSE;
    }

    if (pLoginCode->nLoginCode != nCode)
    {
        Delete(uAccountID);
        return FALSE;
    }

    if (pLoginCode->uEndTime < CommonFunc::GetCurrTime())
    {
        Delete(uAccountID);
        return FALSE;
    }

    Delete(uAccountID);

    return TRUE;
}



