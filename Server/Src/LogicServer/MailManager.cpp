#include "stdafx.h"
#include "MailManager.h"
#include "GameService.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "MailModule.h"
#include "RoleModule.h"

CMailManager::CMailManager()
{

}

CMailManager::~CMailManager()
{
	for (auto itor = m_mapOffMailData.begin(); itor != m_mapOffMailData.end(); itor++)
	{
		itor->second->Release();
	}

	m_mapOffMailData.clear();

	for (auto itor = m_mapGroupMailData.begin(); itor != m_mapGroupMailData.end(); itor++)
	{
		itor->second->Release();
	}

	m_mapGroupMailData.clear();
}

CMailManager* CMailManager::GetInstancePtr()
{
	static CMailManager _StaticMgr;

	return &_StaticMgr;
}

BOOL CMailManager::SendGroupMail(UINT32 nGroupID, std::string strSender, std::string strTitle, std::string strContent, std::vector<StMailItem>& vtItems)
{
	GroupMailDataObject* pGroupMailObject = DataPool::CreateObject<GroupMailDataObject>(ESD_GROUP_MAIL, TRUE);
	pGroupMailObject->Lock();
	pGroupMailObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pGroupMailObject->m_nGroupID = nGroupID;
	//pGroupMailObject->m_dwMailType;					 //邮件类型
	//pGroupMailObject->m_dwChannel;						 //目标渠道
	pGroupMailObject->m_uTime = CommonFunc::GetCurrTime();

	for (int i = 0; i < vtItems.size() && i < MAIL_ITEM_COUNT; i++)
	{
		if (vtItems.at(i).m_nItemID == 0)
		{
			break;
		}

		pGroupMailObject->m_Items[i] = vtItems.at(i);
	}

	pGroupMailObject->Unlock();

	m_mapGroupMailData.insert(std::make_pair(pGroupMailObject->m_uGuid, pGroupMailObject));

	CPlayerManager::TNodeTypePtr pNode = CPlayerManager::GetInstancePtr()->MoveFirst();
	ERROR_RETURN_FALSE(pNode != NULL);

	CPlayerObject* pTempObj = NULL;
	for (; pNode != NULL; pNode = CPlayerManager::GetInstancePtr()->MoveNext(pNode))
	{
		pTempObj = pNode->GetValue();
		ERROR_RETURN_FALSE(pTempObj != NULL);

		if (!pTempObj->IsOnline())
		{
			continue;
		}

		CMailModule* pMailModule = (CMailModule*)pTempObj->GetModuleByType(MT_MAIL);
		ERROR_RETURN_FALSE(pMailModule != NULL);

		pMailModule->ReceiveGroupMail(pGroupMailObject);
	}

	return TRUE;
}

BOOL CMailManager::SendSingleMail(UINT64 uRoleID, std::string strSender, std::string strTitle, std::string strContent, std::vector<StMailItem>& vtItems)
{
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(uRoleID);
	if (pPlayer != NULL) //玩家在线，直接发给玩家数据
	{
		CMailModule* pMailModule = (CMailModule*)pPlayer->GetModuleByType(MT_MAIL);
		ERROR_RETURN_FALSE(pMailModule != NULL);

		pMailModule->AddMail(strSender, strTitle, strContent, vtItems);
		return TRUE;
	}

	//玩家不在线，放到离线内存数据内

	MailDataObject* pMailObject = DataPool::CreateObject<MailDataObject>(ESD_MAIL, TRUE);
	pMailObject->Lock();
	pMailObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pMailObject->m_uRoleID = uRoleID;
	pMailObject->m_uTime = CommonFunc::GetCurrTime();
	strncpy(pMailObject->m_szSender, strSender.c_str(), CommonFunc::Min(ROLE_NAME_LEN, (INT32)strSender.size()));

	for (int i = 0; i < vtItems.size() && i < MAIL_ITEM_COUNT; i++)
	{
		if (vtItems.at(i).m_nItemID == 0)
		{
			break;
		}

		pMailObject->m_Items[i] = vtItems.at(i);
	}

	pMailObject->Unlock();

	m_mapOffMailData.insert(std::make_pair(pMailObject->m_uGuid, pMailObject));

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

	return TRUE;
}

BOOL CMailManager::LoadGroupMailData(CppMySQL3DB& tDBConnection)
{
	return TRUE;
}

BOOL CMailManager::ProcessRoleLogin(CPlayerObject* pPlayer)
{
	CRoleModule* pRoleModule = (CRoleModule*)pPlayer->GetModuleByType(MT_ROLE);
	ERROR_RETURN_FALSE(pRoleModule != NULL);

	for (auto itor = m_mapGroupMailData.begin(); itor != m_mapGroupMailData.end(); ++itor)
	{
		GroupMailDataObject* pGroupMailObject = itor->second;
		ERROR_RETURN_FALSE(pGroupMailObject == NULL);

		if (pGroupMailObject->m_uTime > pRoleModule->GetGroupMailTime())
		{
			CMailModule* pMailModule = (CMailModule*)pPlayer->GetModuleByType(MT_MAIL);
			ERROR_RETURN_FALSE(pMailModule != NULL);

			pMailModule->ReceiveGroupMail(pGroupMailObject);
		}
	}

	return TRUE;
}

MailDataObject* CMailManager::PickUpMailData(UINT64 uGuid)
{
	MailDataObject* pObject = NULL;
	auto itor = m_mapOffMailData.find(uGuid);
	if (itor != m_mapOffMailData.end())
	{
		pObject = itor->second;
		m_mapOffMailData.erase(itor);
	}

	return pObject;
}
