#include "stdafx.h"
#include "MailManager.h"
#include "GameService.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "MailModule.h"

CMailManager::CMailManager()
{

}

CMailManager::~CMailManager()
{

}

CMailManager* CMailManager::GetInstancePtr()
{
	static CMailManager _StaticMgr;

	return &_StaticMgr;
}

BOOL CMailManager::SendGroupMail()
{
	return TRUE;
}

BOOL CMailManager::SendSingleMail(UINT64 uRoleID)
{
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(uRoleID);
	if (pPlayer != NULL)
	{
		CMailModule* pMailModule = (CMailModule*)pPlayer->GetModuleByType(MT_MAIL);
		ERROR_RETURN_FALSE(pMailModule != NULL);

		pMailModule->AddMail("SenderName", "Title", "Content");
		return TRUE;
	}

	return TRUE;
}

BOOL CMailManager::SendOffOperation(UINT64 uRoleID)
{
	return TRUE;
}

BOOL CMailManager::LoadData(CppMySQL3DB& tDBConnection)
{
	BOOL bRet = LoadGroupMailData(tDBConnection);
	ERROR_RETURN_FALSE(bRet);

	bRet = LoadGroupMailData(tDBConnection);
	ERROR_RETURN_FALSE(bRet);

	return TRUE;
}

BOOL CMailManager::LoadGroupMailData(CppMySQL3DB& tDBConnection)
{
	return TRUE;
}

BOOL CMailManager::LoadOffMailData(CppMySQL3DB& tDBConnection)
{


	return TRUE;
}

BOOL CMailManager::ProcessRoleLogin(CPlayerObject* pPlayer)
{
	/*
	for (auto itor = m_vtOffMailData.begin(); itor != m_vtOffMailData.end(); ++itor)
	{
		OffMailDataObject* pMail = *itor;

		if (pMail->m_uRoleID != pPlayer->GetObjectID())
		{
			continue;
		}

		if (pMail->m_nOpType == Mail)
		{
			CMailModule* pMailModule = (CMailModule*)pPlayer->GetModuleByType(MT_MAIL);

			pMailModule->AddMail();
		}

		if (pMail->m_nOpType == GoodFirend)
		{
			CFriendModule* pMailModule = (CFriendModule*)pPlayer->GetModuleByType(MT_FRIEND);
		}

	}
	*/
	return TRUE;
}
