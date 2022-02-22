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

BOOL CMailManager::SendGroupMail(std::string strSender, std::string strTitle, std::string strContent, std::vector<StMailItem>& vtItems, INT32 nRecvGroup)
{
    if (nRecvGroup == 2)
    {
        CPlayerManager::TNodeTypePtr pNode = CPlayerManager::GetInstancePtr()->MoveFirst();
        ERROR_RETURN_FALSE(pNode != NULL);

        CPlayerObject* pTempObj = NULL;
        for (; pNode != NULL; pNode = CPlayerManager::GetInstancePtr()->MoveNext(pNode))
        {
            pTempObj = pNode->GetValue();
            ERROR_CONTINUE_EX(pTempObj != NULL);

            if (!pTempObj->IsOnline())
            {
                continue;
            }

            CMailModule* pMailModule = (CMailModule*)pTempObj->GetModuleByType(MT_MAIL);
            ERROR_CONTINUE_EX(pMailModule != NULL);

            pMailModule->AddMail(EMT_CUSTOM, strSender, strTitle, strContent, vtItems);
        }

        return TRUE;
    }

    GroupMailDataObject* pGroupMailObject = DataPool::CreateObject<GroupMailDataObject>(ESD_GROUP_MAIL, TRUE);
    pGroupMailObject->Lock();
    pGroupMailObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
    pGroupMailObject->m_nMailType = EMT_CUSTOM;                     //邮件类型
    pGroupMailObject->m_uTime = CommonFunc::GetCurrTime();
    CommonConvert::StrCopy(pGroupMailObject->m_szTitle, strTitle.c_str(), MAIL_TITLE_LEN);
    CommonConvert::StrCopy(pGroupMailObject->m_szContent, strContent.c_str(), MAIL_CONTENT_LEN);
    CommonConvert::StrCopy(pGroupMailObject->m_szSender, strSender.c_str(), ROLE_NAME_LEN);

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

BOOL CMailManager::SendSingleMail(UINT64 uRoleID, EMailType eMailType, std::string strContent, std::vector<StMailItem>& vtItems, std::string strSender, std::string strTitle )
{
    ERROR_RETURN_FALSE(eMailType > 0);

    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(uRoleID);
    if (pPlayer != NULL) //玩家在线，直接发给玩家数据
    {
        CMailModule* pMailModule = (CMailModule*)pPlayer->GetModuleByType(MT_MAIL);
        ERROR_RETURN_FALSE(pMailModule != NULL);

        return pMailModule->AddMail(eMailType, strSender, strTitle, strContent, vtItems);
    }

    //玩家不在线，放到离线内存数据内

    MailDataObject* pMailObject = DataPool::CreateObject<MailDataObject>(ESD_MAIL, TRUE);
    pMailObject->Lock();
    pMailObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
    pMailObject->m_uRoleID = uRoleID;
    pMailObject->m_nMailType = eMailType;
    pMailObject->m_uTime = CommonFunc::GetCurrTime();
    CommonConvert::StrCopy(pMailObject->m_szTitle, strTitle.c_str(), MAIL_TITLE_LEN);
    CommonConvert::StrCopy(pMailObject->m_szContent, strContent.c_str(), MAIL_CONTENT_LEN);
    CommonConvert::StrCopy(pMailObject->m_szSender, strSender.c_str(), ROLE_NAME_LEN);

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

BOOL CMailManager::DeleteGroupMail(UINT64 uGuid)
{
    auto itor = m_mapGroupMailData.find(uGuid);
    if (itor == m_mapGroupMailData.end())
    {
        return FALSE;
    }

    GroupMailDataObject* pObject = itor->second;

    pObject->Destroy();

    m_mapGroupMailData.erase(itor);

    CPlayerManager::TNodeTypePtr pNode = CPlayerManager::GetInstancePtr()->MoveFirst();
    ERROR_RETURN_FALSE(pNode != NULL);

    CPlayerObject* pTempObj = NULL;
    for (; pNode != NULL; pNode = CPlayerManager::GetInstancePtr()->MoveNext(pNode))
    {
        pTempObj = pNode->GetValue();
        ERROR_TO_CONTINUE(pTempObj != NULL);

        CMailModule* pMailModule = (CMailModule*)pTempObj->GetModuleByType(MT_MAIL);
        ERROR_TO_CONTINUE(pMailModule != NULL);

        pMailModule->DeleteMailByGroupID(uGuid);
    }

    return TRUE;
}

BOOL CMailManager::LoadData(CppMySQL3DB& tDBConnection)
{
    ERROR_RETURN_FALSE(LoadGroupMailData(tDBConnection));

    return TRUE;
}

BOOL CMailManager::LoadGroupMailData(CppMySQL3DB& tDBConnection)
{
    CppMySQLQuery QueryResult = tDBConnection.querySQL("SELECT * FROM mail_group");
    while (!QueryResult.eof())
    {
        GroupMailDataObject* pGroupMail = DataPool::CreateObject<GroupMailDataObject>(ESD_GROUP_MAIL, FALSE);
        pGroupMail->m_nMailType = EMT_CUSTOM;                   //邮件类型
        pGroupMail->m_nChannel = QueryResult.getIntField("channel");
        pGroupMail->m_uGuid = QueryResult.getInt64Field("id");
        pGroupMail->m_uTime = QueryResult.getInt64Field("mail_time");
        CommonConvert::StrCopy(pGroupMail->m_szTitle, QueryResult.getStringField("title"),   MAIL_TITLE_LEN);
        CommonConvert::StrCopy(pGroupMail->m_szContent, QueryResult.getStringField("content"), MAIL_CONTENT_LEN);
        CommonConvert::StrCopy(pGroupMail->m_szSender, QueryResult.getStringField("sender"),  ROLE_NAME_LEN);

        INT32 nLen = 0;
        const unsigned char* pData = QueryResult.getBlobField("itemdata", nLen);
        memcpy((void*)&pGroupMail->m_Items, (void*)pData, nLen);

        m_mapGroupMailData.insert(std::make_pair(pGroupMail->m_uGuid, pGroupMail));

        QueryResult.nextRow();
    }

    return TRUE;
}

BOOL CMailManager::ProcessRoleLogin(CPlayerObject* pPlayer)
{
    CRoleModule* pRoleModule = (CRoleModule*)pPlayer->GetModuleByType(MT_ROLE);
    ERROR_RETURN_FALSE(pRoleModule != NULL);

    for (auto itor = m_mapGroupMailData.begin(); itor != m_mapGroupMailData.end(); ++itor)
    {
        GroupMailDataObject* pGroupMailObject = itor->second;
        ERROR_RETURN_FALSE(pGroupMailObject != NULL);

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
