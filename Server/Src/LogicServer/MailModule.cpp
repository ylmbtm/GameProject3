#include "stdafx.h"
#include "MailModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "StaticData.h"
#include "PlayerObject.h"
#include "MailManager.h"
#include "RoleModule.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "GameLogManager.h"

CMailModule::CMailModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
    RegisterMessageHanler();
}

CMailModule::~CMailModule()
{

}

BOOL CMailModule::OnCreate(UINT64 u64RoleID)
{

    return TRUE;
}


BOOL CMailModule::OnDestroy()
{
    for(auto itor = m_mapMailData.begin(); itor != m_mapMailData.end(); itor++)
    {
        itor->second->Release();
    }

    m_mapMailData.clear();

    return TRUE;
}

BOOL CMailModule::OnLogin()
{
    return TRUE;
}

BOOL CMailModule::OnLogout()
{
    return TRUE;
}

BOOL CMailModule::OnNewDay()
{
    return TRUE;
}

BOOL CMailModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
    const DBMailData& MailData = Ack.maildata();
    for(int i = 0; i < MailData.maillist_size(); i++)
    {
        const DBMailItem& tMailItem = MailData.maillist(i);

        MailDataObject* pObject = CMailManager::GetInstancePtr()->PickUpMailData(tMailItem.guid());
        if (pObject == NULL)
        {
            pObject = DataPool::CreateObject<MailDataObject>(ESD_MAIL, TRUE);
            pObject->m_uRoleID = tMailItem.roleid();
            pObject->m_uGuid = tMailItem.guid();
            pObject->m_uGroupGuid = tMailItem.groupid();
            pObject->m_uTime = tMailItem.time();
            pObject->m_uSenderID = tMailItem.senderid();
            pObject->m_nMailType = tMailItem.mailtype();
            pObject->m_nStatus = tMailItem.status();
            CommonConvert::StrCopy(pObject->m_szSender, tMailItem.sender().c_str(), ROLE_NAME_LEN);
            CommonConvert::StrCopy(pObject->m_szTitle, tMailItem.title().c_str(), MAIL_TITLE_LEN);
            CommonConvert::StrCopy(pObject->m_szContent, tMailItem.content().c_str(), MAIL_CONTENT_LEN);
            memcpy((void*)&pObject->m_Items, (void*)tMailItem.items().c_str(), sizeof(StMailItem)*MAIL_ITEM_COUNT);
            m_mapMailData.insert(std::make_pair(pObject->m_uGuid, pObject));
        }
        else
        {
            m_mapMailData.insert(std::make_pair(pObject->m_uGuid, pObject));
        }
    }

    return TRUE;
}

BOOL CMailModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
    return TRUE;
}



BOOL CMailModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{

    return TRUE;
}

VOID CMailModule::RegisterMessageHanler()
{
}

BOOL CMailModule::AddMail(MailDataObject* pMail)
{
    m_mapMailData.insert(std::make_pair(pMail->m_uGuid, pMail));

    AddChangeID(pMail->m_uGuid);

    return TRUE;
}

BOOL CMailModule::DeleteMail(UINT64 uGuid)
{
    auto itor = m_mapMailData.find(uGuid);
    if(itor == m_mapMailData.end())
    {
        return FALSE;
    }

    MailDataObject* pObject = itor->second;

    pObject->Destroy();

    m_mapMailData.erase(itor);

    AddRemoveID(uGuid);

    return TRUE;
}

BOOL CMailModule::DeleteMailByGroupID(UINT64 uGuid)
{
    for (auto itor = m_mapMailData.begin(); itor != m_mapMailData.end(); itor++)
    {
        MailDataObject* pObject = itor->second;
        ERROR_TO_CONTINUE(pObject != NULL);

        if (pObject->m_uGroupGuid == uGuid)
        {
            pObject->Destroy();

            m_mapMailData.erase(itor);

            AddRemoveID(uGuid);

            return TRUE;
        }
    }

    return TRUE;
}

BOOL CMailModule::AddMail(EMailType eMailType, std::string& strSender, std::string& strTitle, std::string& strContent, std::vector<StMailItem>& vtItems)
{
    MailDataObject* pMailObject = DataPool::CreateObject<MailDataObject>(ESD_MAIL, TRUE);
    pMailObject->Lock();
    pMailObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
    pMailObject->m_uRoleID = m_pOwnPlayer->GetRoleID();
    CommonConvert::StrCopy(pMailObject->m_szSender, strSender.c_str(), ROLE_NAME_LEN);
    CommonConvert::StrCopy(pMailObject->m_szTitle, strTitle.c_str(), MAIL_TITLE_LEN);
    CommonConvert::StrCopy(pMailObject->m_szContent, strContent.c_str(), MAIL_CONTENT_LEN);
    pMailObject->m_nMailType = eMailType;
    pMailObject->m_uTime = CommonFunc::GetCurrTime();

    for (int i = 0; i < vtItems.size() && i < MAIL_ITEM_COUNT; i++)
    {
        if (vtItems.at(i).m_nItemID == 0)
        {
            break;
        }

        pMailObject->m_Items[i] = vtItems.at(i);
    }

    pMailObject->Unlock();

    return AddMail(pMailObject);
}

MailDataObject* CMailModule::GetMailByGuid(UINT64 uGuid)
{
    auto itor = m_mapMailData.find(uGuid);
    if(itor != m_mapMailData.end())
    {
        return itor->second;
    }

    return NULL;
}

BOOL CMailModule::ReceiveGroupMail(GroupMailDataObject* pGroupMail)
{
    MailDataObject* pMailObject = DataPool::CreateObject<MailDataObject>(ESD_MAIL, TRUE);
    pMailObject->Lock();
    pMailObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
    pMailObject->m_uRoleID = m_pOwnPlayer->GetRoleID();
    pMailObject->m_uTime = CommonFunc::GetCurrTime();
    pMailObject->m_uGroupGuid = pGroupMail->m_uGuid;
    pMailObject->m_nMailType = pGroupMail->m_nMailType;
    CommonConvert::StrCopy(pMailObject->m_szTitle, pGroupMail->m_szTitle, MAIL_TITLE_LEN);
    CommonConvert::StrCopy(pMailObject->m_szContent, pGroupMail->m_szContent, MAIL_CONTENT_LEN);
    CommonConvert::StrCopy(pMailObject->m_szSender, pGroupMail->m_szSender, ROLE_NAME_LEN);
    for (int i = 0; i < MAIL_ITEM_COUNT; i++)
    {
        if (pGroupMail->m_Items[i].m_nItemID == 0)
        {
            break;
        }

        pMailObject->m_Items[i] = pGroupMail->m_Items[i];
    }

    pMailObject->Unlock();

    AddMail(pMailObject);

    CRoleModule* pRoleModule = (CRoleModule*)m_pOwnPlayer->GetModuleByType(MT_ROLE);
    ERROR_RETURN_FALSE(pRoleModule != NULL);

    pRoleModule->SetGroupMailTime(pGroupMail->m_uTime);

    return TRUE;
}

BOOL CMailModule::NotifyChange()
{
    if (m_setChange.size() <= 0 && m_setRemove.size() <= 0)
    {
        return TRUE;
    }

    MailChangeNty Nty;
    for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
    {
        MailDataObject* pObject = GetMailByGuid(*itor);
        ERROR_CONTINUE_EX(pObject != NULL);
        MailItem* pItem = Nty.add_changelist();
        pItem->set_guid(pObject->m_uGuid);
        pItem->set_mailtype(pObject->m_nMailType);
        pItem->set_status(pObject->m_nStatus);
        pItem->set_title(pObject->m_szTitle);
        pItem->set_content(pObject->m_szContent);
        pItem->set_sender(pObject->m_szSender);

        for (INT32 i = 0; i < MAIL_ITEM_COUNT; i++)
        {
            if (pObject->m_Items[i].m_nItemID <= 0)
            {
                break;
            }

            pItem->add_itemid(pObject->m_Items[i].m_nItemID);
            pItem->add_itemnum(pObject->m_Items[i].m_nItemNum);
        }
    }

    for(auto itor = m_setRemove.begin(); itor != m_setRemove.end(); itor++)
    {
        Nty.add_removelist(*itor);
    }

    m_pOwnPlayer->SendMsgProtoBuf(MSG_MAIL_CHANGE_NTY, Nty);

    m_setChange.clear();
    m_setRemove.clear();

    return TRUE;
}
