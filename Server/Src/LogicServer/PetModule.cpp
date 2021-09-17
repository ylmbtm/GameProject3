#include "stdafx.h"
#include "PlayerObject.h"
#include "PetModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "StaticData.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"

CPetModule::CPetModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
    RegisterMessageHanler();
}

CPetModule::~CPetModule()
{

}

BOOL CPetModule::OnCreate(UINT64 u64RoleID)
{
    for (int i = 0; i < 13; i++)
    {
        AddPet(i + 1);
    }
    return TRUE;
}


BOOL CPetModule::OnDestroy()
{
    for(auto itor = m_mapPetData.begin(); itor != m_mapPetData.end(); itor++)
    {
        itor->second->Release();
    }

    m_mapPetData.clear();

    return TRUE;
}

BOOL CPetModule::OnLogin()
{
    return TRUE;
}

BOOL CPetModule::OnLogout()
{
    return TRUE;
}

BOOL CPetModule::OnNewDay()
{
    return TRUE;
}

BOOL CPetModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
    const DBPetData& PetData = Ack.petdata();
    for(int i = 0; i < PetData.petlist_size(); i++)
    {
        const DBPetItem& PetItem = PetData.petlist(i);
        PetDataObject* pObject = DataPool::CreateObject<PetDataObject>(ESD_PET, FALSE);
        pObject->m_uGuid = PetItem.guid();
        pObject->m_uRoleID = PetItem.roleid();
        pObject->m_PetID = PetItem.petid();
        pObject->m_StrengthLvl = PetItem.strengthlvl();
        pObject->m_RefineLevel = PetItem.refinelevel();
        pObject->m_StarLevel = PetItem.starlevel();
        pObject->m_IsUsing = PetItem.isusing();
        m_mapPetData.insert(std::make_pair(pObject->m_uGuid, pObject));
    }
    return TRUE;
}

BOOL CPetModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
    for (auto itor = m_mapPetData.begin(); itor != m_mapPetData.end(); itor++)
    {
        PetDataObject* pObject = itor->second;
        PetItem* pItem = Ack.add_petlist();
        pItem->set_guid(pObject->m_uGuid);
        pItem->set_petid(pObject->m_PetID);
        pItem->set_strengthlvl(pObject->m_StrengthLvl);
        pItem->set_refinelevel(pObject->m_RefineLevel);
        pItem->set_starlevel(pObject->m_StarLevel);
        pItem->set_refineexp(pObject->m_RefineExp);
        pItem->set_starexp(pObject->m_StarExp);
        pItem->set_isusing(pObject->m_IsUsing);
    }

    m_setChange.clear();
    m_setRemove.clear();

    return TRUE;
}

BOOL CPetModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
    return TRUE;
}

VOID CPetModule::RegisterMessageHanler()
{
    m_pOwnPlayer->RegisterMessageHandle(MSG_SETUP_PET_REQ, &CPetModule::OnMsgSetupPetReq, this);
    m_pOwnPlayer->RegisterMessageHandle(MSG_UNSET_PET_REQ, &CPetModule::OnMsgUnsetPetReq, this);
}

BOOL CPetModule::ToTransferData(TransferDataItem* pTransItem)
{
    PetDataObject* pObject = GetCurrentPetData();
    if(pObject == NULL)
    {
        return TRUE;
    }

    StPetInfo* pPetInfo = CStaticData::GetInstancePtr()->GetPetInfo(pObject->m_PetID);
    ERROR_RETURN_FALSE(pPetInfo != NULL);

    StActorInfo* pActorInfo = CStaticData::GetInstancePtr()->GetActorInfo(pPetInfo->dwActorID);
    ERROR_RETURN_FALSE(pActorInfo != NULL);

    TransPetData* pPetData = pTransItem->mutable_petdata();
    ERROR_RETURN_FALSE(pPetData != NULL);

    pPetData->set_actorid(pPetInfo->dwActorID);
    pPetData->set_level(pObject->m_StrengthLvl);
    pPetData->set_petguid(pObject->m_uGuid);
    pPetData->set_petid(pObject->m_PetID);

    for (int i = 0; i < PROPERTY_NUM; i++)
    {
        pPetData->add_propertys(pActorInfo->Propertys[i]);
    }

    StActorSkillInfo* pActorSkillInfo = CStaticData::GetInstancePtr()->GetActorSkillInfo(pPetInfo->dwActorID);
    ERROR_RETURN_FALSE(pActorSkillInfo != NULL);

    SkillItem* pSkillItem = pPetData->add_skills();
    pSkillItem->set_keypos(1);
    pSkillItem->set_level(1);
    pSkillItem->set_skillid(pActorSkillInfo->NormalID);


    return TRUE;
}

PetDataObject* CPetModule::GetCurrentPetData()
{
    for (auto itor = m_mapPetData.begin(); itor != m_mapPetData.end(); itor++)
    {
        PetDataObject* pObject = itor->second;
        if (pObject->m_IsUsing)
        {
            return pObject;
        }
    }

    return NULL;
}

BOOL CPetModule::OnMsgSetupPetReq(NetPacket* pNetPacket)
{
    SetupPetReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    PetDataObject* pCurObject = GetCurrentPetData();
    if (pCurObject != NULL)
    {
        pCurObject->Lock();
        pCurObject->m_IsUsing = false;
        pCurObject->Unlock();
        AddChangeID(pCurObject->m_uGuid);
    }

    PetDataObject* pTargetObject = GetPetByGuid(Req.petguid());
    if (pTargetObject != NULL)
    {
        pTargetObject->Lock();
        pTargetObject->m_IsUsing = TRUE;
        pTargetObject->Unlock();
        AddChangeID(pTargetObject->m_uGuid);
        m_pOwnPlayer->SendPlayerChange(ECT_PET, 0, pTargetObject->m_PetID, "");
    }

    SetupPetAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);
    m_pOwnPlayer->SendMsgProtoBuf(MSG_SETUP_PET_ACK, Ack);

    return TRUE;
}

BOOL CPetModule::OnMsgUnsetPetReq(NetPacket* pNetPacket)
{
    UnsetPetReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    PetDataObject* pCurObject = GetCurrentPetData();
    if (pCurObject != NULL)
    {
        pCurObject->Lock();
        pCurObject->m_IsUsing = false;
        pCurObject->Unlock();
        AddChangeID(pCurObject->m_uGuid);
    }

    UnsetPetAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);
    m_pOwnPlayer->SendMsgProtoBuf(MSG_UNSET_PET_ACK, Ack);

    return TRUE;
}

UINT64 CPetModule::AddPet(UINT32 dwPetID)
{
    PetDataObject* pObject = DataPool::CreateObject<PetDataObject>(ESD_PET, TRUE);
    pObject->Lock();
    pObject->m_PetID = dwPetID;
    pObject->m_uRoleID = m_pOwnPlayer->GetRoleID();
    pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
    pObject->m_StrengthLvl = 1;
    pObject->m_RefineExp = 0;
    pObject->m_StarExp = 0;
    pObject->m_StarLevel = 1;
    pObject->m_RefineLevel = 1;
    pObject->Unlock();
    m_mapPetData.insert(std::make_pair(pObject->m_uGuid, pObject));
    AddChangeID(pObject->m_uGuid);
    return pObject->m_uGuid;
}


BOOL CPetModule::NotifyChange()
{
    if (m_setChange.size() <= 0 && m_setRemove.size() <= 0)
    {
        return TRUE;
    }

    PetChangeNty Nty;
    for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
    {
        PetDataObject* pObject = GetPetByGuid(*itor);
        ERROR_CONTINUE_EX(pObject != NULL);
        PetItem* pItem = Nty.add_changelist();
        pItem->set_guid(pObject->m_uGuid);
        pItem->set_petid(pObject->m_PetID);
        pItem->set_strengthlvl(pObject->m_StrengthLvl);
        pItem->set_refinelevel(pObject->m_RefineLevel);
        pItem->set_starlevel(pObject->m_StarLevel);
        pItem->set_refineexp(pObject->m_RefineExp);
        pItem->set_starexp(pObject->m_StarExp);
        pItem->set_isusing(pObject->m_IsUsing);
    }

    for(auto itor = m_setRemove.begin(); itor != m_setRemove.end(); itor++)
    {
        Nty.add_removelist(*itor);
    }

    m_pOwnPlayer->SendMsgProtoBuf(MSG_PET_CHANGE_NTY, Nty);

    m_setChange.clear();
    m_setRemove.clear();

    return TRUE;
}

PetDataObject* CPetModule::GetPetByGuid(UINT64 uGuid)
{
    auto itor = m_mapPetData.find(uGuid);
    if(itor != m_mapPetData.end())
    {
        return itor->second;
    }

    return NULL;
}
