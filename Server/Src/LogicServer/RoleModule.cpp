#include "stdafx.h"
#include "RoleModule.h"
#include "../ServerData/ServerDefine.h"
#include "../StaticData/StaticData.h"
#include "DataPool.h"
#include "SimpleManager.h"

CRoleModule::CRoleModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
    RegisterMessageHanler();
}

CRoleModule::~CRoleModule()
{

}

BOOL CRoleModule::OnCreate(UINT64 u64RoleID)
{
    StCarrerInfo* pInfo = CStaticData::GetInstancePtr()->GetCarrerInfo(m_pRoleDataObject->m_CarrerID);
    ERROR_RETURN_FALSE(pInfo != NULL);

    ERROR_RETURN_FALSE(m_pRoleDataObject != NULL);
    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_Level = 1;

    for(int i = 0; i < ACTION_NUM; i++)
    {
        m_pRoleDataObject->m_Action[i] = CStaticData::GetInstancePtr()->GetActoinMaxValue(i + 1);
        m_pRoleDataObject->m_Actime[i] = 0;
    }

    m_pRoleDataObject->m_CityCopyID = pInfo->dwBornCity;

    m_pRoleDataObject->m_uCreateTime = CommonFunc::GetCurrTime();
    m_pRoleDataObject->m_uLogonTime = m_pRoleDataObject->m_uCreateTime;
    m_pRoleDataObject->m_uLogoffTime = m_pRoleDataObject->m_uLogonTime;

    m_pRoleDataObject->Unlock();

    m_dwActorID = pInfo->dwActorID;

    return TRUE;
}

BOOL CRoleModule::InitBaseData( UINT64 u64RoleID, std::string Name, UINT32 dwCarrerID, UINT64 u64AccountID, INT32 nChannel )
{
    m_pRoleDataObject = DataPool::CreateObject<RoleDataObject>(ESD_ROLE, TRUE);
    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_uRoleID = u64RoleID;
    m_pRoleDataObject->m_uAccountID = u64AccountID;
    CommonConvert::StrCopy(m_pRoleDataObject->m_szName, Name.c_str(), ROLE_NAME_LEN);
    m_pRoleDataObject->m_nLangID = 0;
    m_pRoleDataObject->m_CarrerID = dwCarrerID;
    m_pRoleDataObject->m_nChannel = nChannel;
    m_pRoleDataObject->Unlock();

    StCarrerInfo* pInfo = CStaticData::GetInstancePtr()->GetCarrerInfo(m_pRoleDataObject->m_CarrerID);
    ERROR_RETURN_FALSE(pInfo != NULL);
    m_dwActorID = pInfo->dwActorID;

    return TRUE;
}

BOOL CRoleModule::OnDestroy()
{
    m_pRoleDataObject->Release();

    m_pRoleDataObject = NULL;

    return TRUE;
}

BOOL CRoleModule::OnLogin()
{
    m_pRoleDataObject->Lock();
    for(int i = 0; i < ACTION_NUM; i++)
    {
        UpdateAction(i + 1);
    }

    if (m_pRoleDataObject->m_uLogoffTime < m_pRoleDataObject->m_uLogonTime)
    {
        m_pRoleDataObject->m_uLogoffTime = m_pRoleDataObject->m_uLogonTime + 3;
        CSimpleManager::GetInstancePtr()->SetLogoffTime(GetRoleID(), m_pRoleDataObject->m_uLogoffTime);
    }

    m_pRoleDataObject->m_uLogonTime = CommonFunc::GetCurrTime();
    m_pRoleDataObject->Unlock();
    CSimpleManager::GetInstancePtr()->SetLogonTime(GetRoleID(), m_pRoleDataObject->m_uLogonTime);
    return TRUE;
}

BOOL CRoleModule::OnLogout()
{
    ERROR_RETURN_FALSE(m_pRoleDataObject != NULL);
    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_uLogoffTime = CommonFunc::GetCurrTime();
    m_pRoleDataObject->m_nOnlineTime += m_pRoleDataObject->m_uLogoffTime - m_pRoleDataObject->m_uLogonTime;
    m_pRoleDataObject->Unlock();
    CSimpleManager::GetInstancePtr()->SetLogoffTime(GetRoleID(), m_pRoleDataObject->m_uLogoffTime);
    return TRUE;
}

BOOL CRoleModule::OnNewDay()
{
    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_uLogoffTime = CommonFunc::GetCurrTime() + 1;
    m_pRoleDataObject->Unlock();

    return TRUE;
}

BOOL CRoleModule::ReadFromDBLoginData( DBRoleLoginAck& Ack )
{
    m_pRoleDataObject = DataPool::CreateObject<RoleDataObject>(ESD_ROLE, FALSE);
    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_uRoleID = Ack.roledata().roleid();
    m_pRoleDataObject->m_uAccountID = Ack.roledata().accountid();
    CommonConvert::StrCopy(m_pRoleDataObject->m_szName, Ack.roledata().name().c_str(), ROLE_NAME_LEN);
    m_pRoleDataObject->m_nLangID = Ack.roledata().langid();
    m_pRoleDataObject->m_CarrerID = Ack.roledata().carrerid();
    m_pRoleDataObject->m_Level = Ack.roledata().level();
    m_pRoleDataObject->m_Exp = Ack.roledata().exp();
    m_pRoleDataObject->m_nVipLvl = Ack.roledata().viplvl();
    m_pRoleDataObject->m_nVipExp = Ack.roledata().vipexp();
    m_pRoleDataObject->m_nLangID = Ack.roledata().langid();
    m_pRoleDataObject->m_CityCopyID = Ack.roledata().citycopyid();
    m_pRoleDataObject->m_uGuildID = Ack.roledata().guildid();
    m_pRoleDataObject->m_uCreateTime = Ack.roledata().createtime();
    m_pRoleDataObject->m_uLogonTime = Ack.roledata().logontime();
    m_pRoleDataObject->m_uLogoffTime = Ack.roledata().logofftime();
    m_pRoleDataObject->m_nChannel = Ack.roledata().channel();
    m_pRoleDataObject->m_nOnlineTime = Ack.roledata().onlinetime();

    if(m_pRoleDataObject->m_CityCopyID == 0)
    {
        StCarrerInfo* pInfo = CStaticData::GetInstancePtr()->GetCarrerInfo(m_pRoleDataObject->m_CarrerID);
        if(pInfo != NULL)
        {
            m_pRoleDataObject->m_CityCopyID = pInfo->dwBornCity;
        }
        else
        {
            CLog::GetInstancePtr()->LogError("Error m_pRoleDataObject->m_CarrerID is 0");
        }
    }

    for(int i = 0; i < Ack.roledata().action_size(); i++)
    {
        m_pRoleDataObject->m_Action[i] = Ack.roledata().action(i);
        m_pRoleDataObject->m_Actime[i] = Ack.roledata().actime(i);
    }

    m_pRoleDataObject->Unlock();

    StCarrerInfo* pInfo = CStaticData::GetInstancePtr()->GetCarrerInfo(m_pRoleDataObject->m_CarrerID);
    ERROR_RETURN_FALSE(pInfo != NULL);
    m_dwActorID = pInfo->dwActorID;

    return TRUE;
}

BOOL CRoleModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
    Ack.set_accountid(m_pRoleDataObject->m_uAccountID);
    Ack.set_roleid(m_pRoleDataObject->m_uRoleID);
    Ack.set_name(m_pRoleDataObject->m_szName);
    Ack.set_level(m_pRoleDataObject->m_Level);
    Ack.set_carrer(m_pRoleDataObject->m_CarrerID);
    Ack.set_fightvalue(m_pRoleDataObject->m_u64Fight);
    Ack.set_exp(m_pRoleDataObject->m_Exp);
    Ack.set_viplvl(m_pRoleDataObject->m_nVipLvl);
    Ack.set_vipexp(m_pRoleDataObject->m_nVipExp);
    for(int i = 0; i < ACTION_NUM; i++)
    {
        ActionItem* pItem = Ack.add_actionlist();
        pItem->set_action(m_pRoleDataObject->m_Action[i]);
        pItem->set_actime(m_pRoleDataObject->m_Actime[i]);
    }

    return TRUE;
}

BOOL CRoleModule::NotifyChange()
{
    return TRUE;
}

BOOL CRoleModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
    return TRUE;
}


VOID CRoleModule::RegisterMessageHanler()
{
}

INT64 CRoleModule::GetProperty(ERoleProperty ePropertyID)
{
    switch (ePropertyID)
    {
        case ERP_LEVEL:
            return GetLevel();
        case ERP_VIPLEVEL:
            return GetVipLevel();
        case ERP_CHANNEL:
            return m_pRoleDataObject->m_nChannel;
        default:
            break;
    }

    return 0;
}

BOOL CRoleModule::CostAction(UINT32 dwActionID, INT32 nActionNum)
{
    if ((dwActionID <= 0) || (dwActionID >= ACTION_NUM))
    {
        CLog::GetInstancePtr()->LogError("CostAction Error: Inavlid dwActionID :%d", dwActionID);
        return FALSE;
    }

    if (nActionNum <= 0)
    {
        CLog::GetInstancePtr()->LogError("CostAction Error: Inavlid nActionNum :%d", nActionNum);
        return FALSE;
    }

    if (m_pRoleDataObject->m_Action[dwActionID - 1] < nActionNum)
    {
        return FALSE;
    }

    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_Action[dwActionID - 1] -= nActionNum;

    if (m_pRoleDataObject->m_Action[dwActionID - 1] < CStaticData::GetInstancePtr()->GetActoinMaxValue(dwActionID) )
    {
        if (m_pRoleDataObject->m_Actime[dwActionID - 1] <= 0)
        {
            m_pRoleDataObject->m_Actime[dwActionID - 1] = CommonFunc::GetCurrTime();
        }
    }
    else
    {
        m_pRoleDataObject->m_Actime[dwActionID - 1] = 0;
    }
    m_pRoleDataObject->Unlock();
    return TRUE;
}

BOOL CRoleModule::CheckActionEnough(UINT32 dwActionID, INT32 nActionNum)
{
    if ((dwActionID <= 0) || (dwActionID > ACTION_NUM))
    {
        CLog::GetInstancePtr()->LogError("CheckActionEnough Error: Inavlid dwActionID :%d", dwActionID);
        return FALSE;
    }

    if (nActionNum <= 0)
    {
        CLog::GetInstancePtr()->LogError("CheckActionEnough Error: Inavlid nActionNum :%d", nActionNum);
        return FALSE;
    }

    if (m_pRoleDataObject->m_Action[dwActionID - 1] >= nActionNum)
    {
        return TRUE;
    }

    UpdateAction(dwActionID);

    if (m_pRoleDataObject->m_Action[dwActionID - 1] < nActionNum)
    {
        return FALSE;
    }

    return TRUE;
}

UINT64 CRoleModule::GetAction(UINT32 dwActionID)
{
    if ((dwActionID <= 0) || (dwActionID > ACTION_NUM))
    {
        CLog::GetInstancePtr()->LogError("GetAction Error: Inavlid dwActionID :%d", dwActionID);
        return 0;
    }

    UpdateAction(dwActionID);

    return m_pRoleDataObject->m_Action[dwActionID - 1];
}

UINT64 CRoleModule::AddAction(UINT32 dwActionID, INT64 nActionNum)
{
    if ((dwActionID <= 0) || (dwActionID > ACTION_NUM))
    {
        CLog::GetInstancePtr()->LogError("AddAction Error: Inavlid dwActionID :%d", dwActionID);
        return 0;
    }

    m_pRoleDataObject->Lock();
    UpdateAction(dwActionID);

    m_pRoleDataObject->m_Action[dwActionID - 1] += nActionNum;

    if (m_pRoleDataObject->m_Action[dwActionID - 1] >= CStaticData::GetInstancePtr()->GetActoinMaxValue(dwActionID))
    {
        m_pRoleDataObject->m_Actime[dwActionID - 1] = 0;
    }
    m_pRoleDataObject->Unlock();
    return m_pRoleDataObject->m_Action[dwActionID - 1];
}

BOOL CRoleModule::UpdateAction(UINT32 dwActionID)
{
    if (m_pRoleDataObject->m_Action[dwActionID - 1] >= CStaticData::GetInstancePtr()->GetActoinMaxValue(dwActionID))
    {
        if (m_pRoleDataObject->m_Actime[dwActionID - 1] > 0)
        {
            CLog::GetInstancePtr()->LogError("UpdateAction error  StartTime is not 0");
        }
        m_pRoleDataObject->m_Actime[dwActionID - 1] = 0;
        return FALSE;
    }

    if (m_pRoleDataObject->m_Actime[dwActionID - 1] <= 0)
    {
        //CLog::GetInstancePtr()->LogError("UpdateAction error  action not max, but starttime is 0");
    }

    UINT64 dwTimeElapse = CommonFunc::GetCurrTime() - m_pRoleDataObject->m_Actime[dwActionID - 1];

    if (dwTimeElapse < CStaticData::GetInstancePtr()->GetActoinUnitTime(dwActionID))
    {
        return FALSE;
    }

    UINT32 dwActionNum = int(dwTimeElapse) / CStaticData::GetInstancePtr()->GetActoinUnitTime(dwActionID);
    m_pRoleDataObject->m_Action[dwActionID - 1] += dwActionNum;

    if (m_pRoleDataObject->m_Action[dwActionID - 1] >= CStaticData::GetInstancePtr()->GetActoinMaxValue(dwActionID))
    {
        m_pRoleDataObject->m_Action[dwActionID - 1] = CStaticData::GetInstancePtr()->GetActoinMaxValue(dwActionID);
        m_pRoleDataObject->m_Actime[dwActionID - 1] = 0;
    }
    else
    {
        m_pRoleDataObject->m_Actime[dwActionID - 1] = m_pRoleDataObject->m_Actime[dwActionID - 1] + dwActionNum * CStaticData::GetInstancePtr()->GetActoinUnitTime(dwActionID);
    }

    return TRUE;
}

BOOL CRoleModule::SetDelete( BOOL bDelete )
{
    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_bDelete = bDelete;
    m_pRoleDataObject->Unlock();
    return TRUE;
}

UINT32 CRoleModule::GetActorID()
{
    return m_dwActorID;
}

INT32 CRoleModule::GetLevel()
{
    return m_pRoleDataObject->m_Level;
}

INT32 CRoleModule::GetVipLevel()
{
    return m_pRoleDataObject->m_nVipLvl;
}

CHAR* CRoleModule::GetName()
{
    return m_pRoleDataObject->m_szName;
}

UINT32 CRoleModule::GetCarrerID()
{
    return m_pRoleDataObject->m_CarrerID;
}

UINT64 CRoleModule::GetRoleID()
{
    return m_pRoleDataObject->m_uRoleID;
}

UINT64 CRoleModule::AddExp( INT32 nExp )
{
    if(nExp <= 0)
    {
        return m_pRoleDataObject->m_Exp;
    }

    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_Exp += nExp;
    m_pRoleDataObject->Unlock();

    while (m_pRoleDataObject->m_Level < MAX_ROLE_LEVEL)
    {
        StLevelInfo* pLevelInfo = CStaticData::GetInstancePtr()->GetCarrerLevelInfo(m_pRoleDataObject->m_CarrerID, m_pRoleDataObject->m_Level + 1);
        ERROR_RETURN_FALSE(pLevelInfo != NULL);

        if (m_pRoleDataObject->m_Exp >= pLevelInfo->dwNeedExp)
        {
            m_pRoleDataObject->Lock();
            m_pRoleDataObject->m_Exp = m_pRoleDataObject->m_Exp - pLevelInfo->dwNeedExp;
            m_pRoleDataObject->m_Level += 1;
            m_pRoleDataObject->Unlock();
        }
        else
        {
            break;
        }
    }

    return m_pRoleDataObject->m_Exp;
}

UINT64 CRoleModule::GetLastLogoffTime()
{
    ERROR_RETURN_VALUE(m_pRoleDataObject != NULL, 0);

    return m_pRoleDataObject->m_uLogoffTime;
}

BOOL CRoleModule::SetLastLogoffTime(UINT64 uTime)
{
    ERROR_RETURN_FALSE(m_pRoleDataObject != NULL);

    m_pRoleDataObject->m_uLogoffTime = uTime;

    return TRUE;
}

UINT64 CRoleModule::GetLastLogonTime()
{
    ERROR_RETURN_VALUE(m_pRoleDataObject != NULL, 0);

    return m_pRoleDataObject->m_uLogonTime;
}

UINT64 CRoleModule::GetCreateTime()
{
    ERROR_RETURN_VALUE(m_pRoleDataObject != NULL, 0);

    return m_pRoleDataObject->m_uCreateTime;
}

VOID CRoleModule::SetGroupMailTime(UINT64 uTime)
{
    m_pRoleDataObject->Lock();
    m_pRoleDataObject->m_uGroupMailTime = uTime;
    m_pRoleDataObject->Unlock();
}

UINT64 CRoleModule::GetGroupMailTime()
{
    return m_pRoleDataObject->m_uGroupMailTime;
}

UINT32 CRoleModule::GetOnlineTime()
{
    return m_pRoleDataObject->m_nOnlineTime;
}
