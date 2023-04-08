#include "stdafx.h"
#include "PlayerObject.h"
#include "PacketHeader.h"
#include "PlayerManager.h"
#include "RoleModule.h"
#include "GameSvrMgr.h"
#include "CopyModule.h"
#include "BagModule.h"
#include "EquipModule.h"
#include "PetModule.h"
#include "PartnerModule.h"
#include "TaskModule.h"
#include "ActivityModule.h"
#include "MountModule.h"
#include "CounterModule.h"
#include "StoreModule.h"
#include "GemModule.h"
#include "SkillModule.h"
#include "MailModule.h"
#include "FriendModule.h"
#include "ServerDefine.h"
#include "RoleData.h"
#include "StaticData.h"
#include "CopyData.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "MailManager.h"
#include "GameLogManager.h"



CPlayerObject::CPlayerObject()
{

}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::Init(UINT64 u64ID)
{
    m_uRoleID             = u64ID;
    m_nProxyConnID     = 0;
    m_nClientConnID    = 0;
    m_dwCopyGuid        = 0;      //当前的副本ID
    m_dwCopyID          = 0;        //当前的副本类型
    m_dwCopySvrID       = 0;        //副本服务器的ID
    m_IsOnline          = FALSE;
    m_uRoomID           = 0;
    ERROR_RETURN_FALSE(CreateAllModule());

    return TRUE;
}

BOOL CPlayerObject::Uninit()
{
    DestroyAllModule();
    m_uRoleID             = 0;      //角色ID
    m_nProxyConnID     = 0;        //网关服的连接ID
    m_nClientConnID    = 0;        //客户端的连接ID
    m_dwCopyGuid        = 0;        //当前的副本ID
    m_dwCopyID          = 0;        //当前的副本类型
    m_dwCopySvrID       = 0;        //副本服务器的ID
    m_IsOnline          = FALSE;    //是否在线
    m_uRoomID           = 0;
    return TRUE;
}


BOOL CPlayerObject::OnCreate(UINT64 u64RoleID)
{
    for(int i = MT_ROLE; i < MT_END; i++)
    {
        CModuleBase* pBase = m_MoudleList.at(i);
        ERROR_RETURN_FALSE(pBase != NULL);
        pBase->OnCreate(u64RoleID);
    }

    return TRUE;
}

BOOL CPlayerObject::OnDestroy()
{
    DestroyAllModule();

    return TRUE;
}

BOOL CPlayerObject::OnLogin()
{
    for(int i = MT_ROLE; i < MT_END; i++)
    {
        CModuleBase* pBase = m_MoudleList.at(i);
        ERROR_RETURN_FALSE(pBase != NULL);
        pBase->OnLogin();
    }

    m_IsOnline = TRUE;

    CRoleModule* pRoleModule = (CRoleModule*)GetModuleByType(MT_ROLE);
    ERROR_RETURN_VALUE(pRoleModule != NULL, MRC_UNKNOW_ERROR);

    if (!CommonFunc::IsSameDay(pRoleModule->GetLastLogoffTime()))
    {
        for (int i = MT_ROLE; i < MT_END; i++)
        {
            CModuleBase* pBase = m_MoudleList.at(i);
            ERROR_RETURN_FALSE(pBase != NULL);
            pBase->OnNewDay();
        }
    }

    m_uRoomID = 0;

    ERROR_RETURN_FALSE(m_uRoleID != 0);

    CMailManager::GetInstancePtr()->ProcessRoleLogin(this);

    CalcFightDataInfo();

    SendRoleLoginAck();

    CGameSvrMgr::GetInstancePtr()->SendPlayerToMainCity(m_uRoleID, GetCityCopyID());

    CGameLogManager::GetInstancePtr()->LogRoleLogin(this);

    return TRUE;
}

BOOL CPlayerObject::OnLogout()
{
    for(int i = MT_ROLE; i < MT_END; i++)
    {
        CModuleBase* pBase = m_MoudleList.at(i);
        ERROR_RETURN_FALSE(pBase != NULL);
        pBase->OnLogout();
    }

    m_IsOnline = FALSE;

    m_uRoomID = 0;

    return TRUE;
}

BOOL CPlayerObject::OnNewDay()
{
    for(int i = MT_ROLE; i < MT_END; i++)
    {
        CModuleBase* pBase = m_MoudleList.at(i);
        ERROR_RETURN_FALSE(pBase != NULL);
        pBase->OnNewDay();
    }
    return TRUE;
}

BOOL CPlayerObject::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
    for(int i = MT_ROLE; i < MT_END; i++)
    {
        CModuleBase* pBase = m_MoudleList.at(i);
        ERROR_RETURN_FALSE(pBase != NULL);
        pBase->ReadFromDBLoginData(Ack);
    }

    return TRUE;
}

BOOL CPlayerObject::CreateAllModule()
{
    m_MoudleList.assign(MT_END, NULL);
    m_MoudleList[MT_ROLE]           = new CRoleModule(this);
    m_MoudleList[MT_COPY]           = new CCopyModule(this);
    m_MoudleList[MT_BAG]            = new CBagModule(this);
    m_MoudleList[MT_EQUIP]          = new CEquipModule(this);
    m_MoudleList[MT_GEM]            = new CGemModule(this);
    m_MoudleList[MT_PET]            = new CPetModule(this);
    m_MoudleList[MT_PARTNER]        = new CPartnerModule(this);
    m_MoudleList[MT_TASK]           = new CTaskModule(this);
    m_MoudleList[MT_MOUNT]          = new CMountModule(this);
    m_MoudleList[MT_ACTIVITY]       = new CActivityModule(this);
    m_MoudleList[MT_COUNTER]        = new CCounterModule(this);
    m_MoudleList[MT_STORE]          = new CStoreModule(this);
    m_MoudleList[MT_SKILL]          = new CSkillModule(this);
    m_MoudleList[MT_MAIL]           = new CMailModule(this);
    m_MoudleList[MT_FRIEND]         = new CFriendModule(this);
    return TRUE;
}

BOOL CPlayerObject::DestroyAllModule()
{
    for(int i = MT_ROLE; i < MT_END; i++)
    {
        CModuleBase* pBase = m_MoudleList.at(i);
        ERROR_RETURN_FALSE(pBase != NULL);

        pBase->OnDestroy();
        delete pBase;
    }

    m_MoudleList.clear();

    return TRUE;
}

BOOL CPlayerObject::SendMsgProtoBuf(INT32 nMsgID, const google::protobuf::Message& pdata)
{
    if (m_nProxyConnID == 0)
    {
        CLog::GetInstancePtr()->LogWarn("Error SendMsgProtoBuf Failed m_dwProxyConnID==0 MessageID:%d, RoleID:%ld", nMsgID, m_uRoleID);
        return FALSE;
    }

    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_nProxyConnID, nMsgID, GetRoleID(), m_nClientConnID, pdata);
}

BOOL CPlayerObject::SendMsgRawData(INT32 nMsgID, const char* pdata, UINT32 dwLen)
{
    if (m_nProxyConnID == 0)
    {
        CLog::GetInstancePtr()->LogError("Error SendMsgRawData MessageID:%d, RoleID:%ld", nMsgID, m_uRoleID);
        return FALSE;
    }

    return ServiceBase::GetInstancePtr()->SendMsgRawData(m_nProxyConnID, nMsgID, GetRoleID(), m_nClientConnID, pdata, dwLen);
}

BOOL CPlayerObject::SendMsgToScene(INT32 nMsgID, const google::protobuf::Message& pdata)
{
    INT32 nConnID = CGameSvrMgr::GetInstancePtr()->GetConnIDBySvrID(m_dwCopySvrID);

    if (nConnID == 0)
    {
        CLog::GetInstancePtr()->LogError("Error SendMsgToScene MessageID:%d, CopySvrID:%ld", nMsgID, m_dwCopySvrID);
        return FALSE;
    }

    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, nMsgID, m_uRoleID, m_dwCopyGuid, pdata);
}

UINT32 CPlayerObject::CheckCopyConditoin(UINT32 dwCopyID)
{
    return TRUE;
    StCopyInfo* pCopyInfo = CStaticData::GetInstancePtr()->GetCopyInfo(dwCopyID);
    ERROR_RETURN_VALUE(m_uRoleID != 0, MRC_INVALID_COPYID);

    CRoleModule* pRoleModule = (CRoleModule*)GetModuleByType(MT_ROLE);
    ERROR_RETURN_VALUE(pRoleModule != NULL, MRC_UNKNOW_ERROR);

    if(!pRoleModule->CheckActionEnough(pCopyInfo->dwCostActID, pCopyInfo->dwCostActNum))
    {
        return MRC_NOT_ENOUGH_ACTOIN;
    }

    CCopyModule* pCopyModule = (CCopyModule*)GetModuleByType(MT_COPY);
    ERROR_RETURN_VALUE(pCopyModule != NULL, MRC_UNKNOW_ERROR);

    CopyDataObject* pObject = pCopyModule->GetCopyData(dwCopyID);
    ERROR_RETURN_VALUE(pObject != NULL, MRC_UNKNOW_ERROR);

    if(pObject->m_dwBattleCnt >= pCopyInfo->dwBattleTimes)
    {
        return MRC_NOT_ENOUGH_TIMES;
    }

    return MRC_SUCCESSED;
}

BOOL CPlayerObject::SendIntoSceneNotify(UINT32 dwCopyGuid, UINT32 dwCopyID, UINT32 dwSvrID)
{
    ERROR_RETURN_FALSE(dwCopyID != 0);
    ERROR_RETURN_FALSE(dwCopyGuid != 0);
    ERROR_RETURN_FALSE(dwSvrID != 0);

    NotifyIntoScene Nty;
    Nty.set_copyid(dwCopyID);
    Nty.set_copyguid(dwCopyGuid);
    Nty.set_serverid(dwSvrID);
    Nty.set_roleid(m_uRoleID);
    ERROR_RETURN_FALSE(m_uRoleID != 0);
    SendMsgProtoBuf(MSG_NOTIFY_INTO_SCENE, Nty);
    return TRUE;
}

BOOL CPlayerObject::SendLeaveScene(UINT32 dwCopyGuid, UINT32 dwSvrID)
{
    LeaveSceneReq LeaveReq;
    LeaveReq.set_roleid(m_uRoleID);
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameSvrMgr::GetInstancePtr()->GetConnIDBySvrID(dwSvrID), MSG_LEAVE_SCENE_REQ, m_uRoleID, dwCopyGuid, LeaveReq);
    return TRUE;
}

BOOL CPlayerObject::SetConnectID(UINT32 dwProxyID, UINT32 dwClientID)
{
    m_nProxyConnID = dwProxyID;
    m_nClientConnID = dwClientID;

    return TRUE;
}

CModuleBase* CPlayerObject::GetModuleByType(UINT32 dwModuleType)
{
    ERROR_RETURN_NULL(dwModuleType < (UINT32)m_MoudleList.size());

    return m_MoudleList.at(dwModuleType);
}

UINT64 CPlayerObject::GetRoleID()
{
    return m_uRoleID;
}

UINT32 CPlayerObject::GetCityCopyID()
{
    CRoleModule* pModule = (CRoleModule*)GetModuleByType(MT_ROLE);
    ERROR_RETURN_FALSE(pModule != NULL);
    ERROR_RETURN_FALSE(pModule->m_pRoleDataObject != NULL);
    return pModule->m_pRoleDataObject->m_CityCopyID;
}

UINT64 CPlayerObject::GetAccountID()
{
    CRoleModule* pModule = (CRoleModule*)GetModuleByType(MT_ROLE);
    ERROR_RETURN_FALSE(pModule != NULL);
    ERROR_RETURN_FALSE(pModule->m_pRoleDataObject != NULL);
    return pModule->m_pRoleDataObject->m_uAccountID;
}

UINT32 CPlayerObject::GetActorID()
{
    CRoleModule* pModule = (CRoleModule*)GetModuleByType(MT_ROLE);

    ERROR_RETURN_VALUE(pModule != NULL, 0);

    return pModule->GetActorID();
}

CHAR* CPlayerObject::GetName()
{
    CRoleModule* pModule = (CRoleModule*)GetModuleByType(MT_ROLE);

    ERROR_RETURN_VALUE(pModule != NULL, 0);

    return pModule->GetName();
}

UINT32 CPlayerObject::GetCarrerID()
{
    CRoleModule* pModule = (CRoleModule*)GetModuleByType(MT_ROLE);

    ERROR_RETURN_VALUE(pModule != NULL, 0);

    return pModule->GetCarrerID();
}

INT64 CPlayerObject::GetProperty(ERoleProperty ePropertyID)
{
    INT32 nModuleID = ePropertyID / 100;

    if (nModuleID < MT_ROLE || nModuleID >= MT_END)
    {
        CLog::GetInstancePtr()->LogError("CPlayerObject::GetProperty Error Inavlie PropertyID:%d", ePropertyID);
        return 0;
    }

    CModuleBase* pModule = GetModuleByType(nModuleID);
    if (pModule == NULL)
    {
        return 0;
    }

    return pModule->GetProperty(ePropertyID);
}

UINT64 CPlayerObject::GetRoomID()
{
    return m_uRoomID;
}

VOID CPlayerObject::SetRoomID(UINT64 uRoomID)
{
    m_uRoomID = uRoomID;
}

BOOL CPlayerObject::SendRoleLoginAck()
{
    CRoleModule* pModule = (CRoleModule*)GetModuleByType(MT_ROLE);
    ERROR_RETURN_FALSE(pModule != NULL);
    RoleLoginAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);
    for(int i = MT_ROLE; i < MT_END; i++)
    {
        CModuleBase* pBase = m_MoudleList.at(i);
        ERROR_RETURN_FALSE(pBase != NULL);
        pBase->SaveToClientLoginData(Ack);
    }

    SendMsgProtoBuf(MSG_ROLE_LOGIN_ACK, Ack);

    return TRUE;
}

BOOL CPlayerObject::SendPlayerChange(EChangeType eChangeType, UINT64 uIntValue1, UINT64 uIntValue2, std::string strValue)
{
    ObjectChangeNotify Ntf;
    Ntf.set_roleid(GetRoleID());
    Ntf.set_changetype(eChangeType);
    Ntf.set_intvalue1(uIntValue1);
    Ntf.set_intvalue2(uIntValue2);
    if (!strValue.empty() && strValue.size() > 0)
    {
        Ntf.set_strvalue(strValue);
    }

    SendMsgToScene(MSG_PLAYER_CHAGE_NTF, Ntf);

    return TRUE;
}

BOOL CPlayerObject::ToTransferData(TransferDataItem* pTransItem)
{
    CRoleModule* pModule = (CRoleModule*)GetModuleByType(MT_ROLE);

    TransRoleData* pRoleData = pTransItem->mutable_roledata();
    ERROR_RETURN_FALSE(pRoleData != NULL);

    pRoleData->set_roleid(m_uRoleID);
    pRoleData->set_carrerid(pModule->m_pRoleDataObject->m_CarrerID);
    pRoleData->set_actorid(pModule->GetActorID());
    pRoleData->set_level(pModule->m_pRoleDataObject->m_Level);
    pRoleData->set_name(pModule->m_pRoleDataObject->m_szName);

    CMountModule* pMountModule = (CMountModule*)GetModuleByType(MT_MOUNT);
    ERROR_RETURN_FALSE(pMountModule != NULL);

    MountDataObject* pMountData = pMountModule->GetCurrentMountData();
    if (pMountData != NULL)
    {
        pRoleData->set_mountid(pMountData->m_MountID);
    }

    for(int i = 0; i < PROPERTY_NUM; i++)
    {
        pRoleData->add_propertys(m_Propertys[i]);
    }

    CEquipModule* pEquipModule = (CEquipModule*)GetModuleByType(MT_EQUIP);
    ERROR_RETURN_FALSE(pEquipModule != NULL);

    for (int i = 0; i < EQUIP_MAX_NUM; i++)
    {
        EquipDataObject* pEquip = pEquipModule->m_vtDressEquip[i];
        if (pEquip == NULL)
        {
            pRoleData->add_equips(0);
        }
        else
        {
            pRoleData->add_equips(pEquip->m_EquipID);
        }
    }

    CSkillModule* pSkillModule = (CSkillModule*)GetModuleByType(MT_SKILL);
    ERROR_RETURN_FALSE(pSkillModule != NULL);

    for (auto itor = pSkillModule->m_mapSkillData.begin(); itor != pSkillModule->m_mapSkillData.end(); ++itor)
    {
        SkillItem* pSkillItem = pRoleData->add_skills();
        pSkillItem->set_keypos(itor->second->m_nKeyPos);
        pSkillItem->set_level(itor->second->m_nLevel);
        pSkillItem->set_skillid(itor->second->m_dwSkillID);
    }

    CPetModule* pPetModule = (CPetModule*)GetModuleByType(MT_PET);
    ERROR_RETURN_FALSE(pPetModule != NULL);
    pPetModule->ToTransferData(pTransItem);

    CPartnerModule* pPartnerModule = (CPartnerModule*)GetModuleByType(MT_PARTNER);
    ERROR_RETURN_FALSE(pPartnerModule != NULL);
    pPartnerModule->ToTransferData(pTransItem);




    return TRUE;
}

BOOL CPlayerObject::NotifyTaskEvent(UINT32 dwEventID, UINT32 dwParam1, UINT32 dwParam2)
{
    CTaskModule* pTaskModule = (CTaskModule*)GetModuleByType(MT_TASK);
    pTaskModule->OnTaskEvent((ETaskEvent)dwEventID, dwParam1, dwParam2);
    return TRUE;
}

BOOL CPlayerObject::IsOnline()
{
    if (m_IsOnline && m_nProxyConnID == 0)
    {
        return FALSE;
    }

    return m_IsOnline;
}

BOOL CPlayerObject::SetOnline(BOOL bOnline)
{
    m_IsOnline = TRUE;
    return TRUE;
}

BOOL CPlayerObject::NotifyChange()
{
    for (int i = MT_ROLE; i < MT_END; i++)
    {
        CModuleBase* pBase = m_MoudleList.at(i);
        ERROR_RETURN_FALSE(pBase != NULL);

        pBase->NotifyChange();
    }
    return TRUE;
}

BOOL CPlayerObject::CalcFightDataInfo()
{
    INT32 PropertyValue[PROPERTY_NUM] = {0};
    INT32 PropertyPercent[PROPERTY_NUM] = {0};
    INT32 FightValue = 0;

    for(int nIndex = MT_ROLE; nIndex < MT_END; nIndex++)
    {
        CRoleModule* pTempModule = (CRoleModule*)GetModuleByType(nIndex);
        pTempModule->CalcFightValue(PropertyValue, PropertyPercent, FightValue);
    }

    CRoleModule* pModule = (CRoleModule*)GetModuleByType(MT_ROLE);
    ERROR_RETURN_FALSE(pModule != NULL);
    StLevelInfo* pLevelInfo = CStaticData::GetInstancePtr()->GetCarrerLevelInfo(pModule->m_pRoleDataObject->m_CarrerID, pModule->m_pRoleDataObject->m_Level);
    ERROR_RETURN_FALSE(pLevelInfo != NULL);
    memcpy(PropertyValue, pLevelInfo->Propertys, sizeof(INT32)*PROPERTY_NUM);

    for(int i = 0; i < PROPERTY_NUM; i++)
    {
        m_Propertys[i] = PropertyValue[i] + PropertyValue[i] * PropertyPercent[i] / 10000;
    }

    return TRUE;
}

BOOL CPlayerObject::ClearCopyStatus()
{
    m_dwCopyGuid = 0;           //当前的副本ID
    m_dwCopyID = 0;             //当前的副本类型
    m_dwCopySvrID = 0;          //副本服务器的ID
    m_bMainCity = FALSE;
    return TRUE;
}

BOOL CPlayerObject::SetCopyStatus(UINT32 dwCopyGuid, UINT32 dwCopyID, UINT32 dwCopySvrID, BOOL bMainCity)
{
    m_dwCopyID = dwCopyID;
    m_dwCopyGuid = dwCopyGuid;
    m_dwCopySvrID = dwCopySvrID;
    m_bMainCity = bMainCity;

    return TRUE;
}





