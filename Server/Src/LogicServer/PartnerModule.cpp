#include "stdafx.h"
#include "PlayerObject.h"
#include "PartnerModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "StaticData.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "PacketHeader.h"

CPartnerModule::CPartnerModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	for (int i = 0; i < PARTNER_MAX_NUM; i++)
	{
		m_vtSetupPartner[i] = NULL;
	}
	RegisterMessageHanler();
}

CPartnerModule::~CPartnerModule()
{
	for (int i = 0; i < PARTNER_MAX_NUM; i++)
	{
		m_vtSetupPartner[i] = NULL;
	}
}

BOOL CPartnerModule::OnCreate(UINT64 u64RoleID)
{
	for (int i = 0; i < 20; i++)
	{
		AddPartner(i + 1);
	}
	return TRUE;
}


BOOL CPartnerModule::OnDestroy()
{
	for(auto itor = m_mapPartnerData.begin(); itor != m_mapPartnerData.end(); itor++)
	{
		itor->second->Release();
	}

	m_mapPartnerData.clear();

	return TRUE;
}

BOOL CPartnerModule::OnLogin()
{
	return TRUE;
}

BOOL CPartnerModule::OnLogout()
{
	return TRUE;
}

BOOL CPartnerModule::OnNewDay()
{
	return TRUE;
}

BOOL CPartnerModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBPartnerData& PartnerData = Ack.partnerdata();
	for(int i = 0; i < PartnerData.partnerlist_size(); i++)
	{
		const DBPartnerItem& PartnerItem = PartnerData.partnerlist(i);

		PartnerDataObject* pObject = DataPool::CreateObject<PartnerDataObject>(ESD_PARTNER, FALSE);
		pObject->m_uGuid = PartnerItem.guid();
		pObject->m_uRoleID = PartnerItem.roleid();
		pObject->m_PartnerID = PartnerItem.partnerid();
		pObject->m_StrengthLvl = PartnerItem.strengthlvl();
		pObject->m_RefineLevel = PartnerItem.refinelevel();
		pObject->m_StarLevel = PartnerItem.starlevel();
		pObject->m_SetPos = PartnerItem.setpos();

		if (pObject->m_SetPos == 1)
		{
			m_vtSetupPartner[0] = pObject;
		}
		if (pObject->m_SetPos == 2)
		{
			m_vtSetupPartner[1] = pObject;
		}

		m_mapPartnerData.insert(std::make_pair(pObject->m_uGuid, pObject));
	}
	return TRUE;
}

BOOL CPartnerModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	for (auto itor = m_mapPartnerData.begin(); itor != m_mapPartnerData.end(); itor++)
	{
		PartnerDataObject* pObject = itor->second;
		PartnerItem* pItem = Ack.add_partnerlist();
		pItem->set_guid(pObject->m_uGuid);
		pItem->set_partnerid(pObject->m_PartnerID);
		pItem->set_strengthlvl(pObject->m_StrengthLvl);
		pItem->set_refinelevel(pObject->m_RefineLevel);
		pItem->set_starlevel(pObject->m_StarLevel);
		pItem->set_refineexp(pObject->m_RefineExp);
		pItem->set_starexp(pObject->m_StarExp);
		pItem->set_setpos(pObject->m_SetPos);
	}

	m_setChange.clear();
	m_setRemove.clear();

	return TRUE;
}


BOOL CPartnerModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

VOID CPartnerModule::RegisterMessageHanler()
{
    m_pOwnPlayer->RegisterMessageHandle(MSG_SETUP_PARTNER_REQ, &CPartnerModule::OnMsgSetupPartnerReq, this);
    m_pOwnPlayer->RegisterMessageHandle(MSG_UNSET_PARTNER_REQ, &CPartnerModule::OnMsgUnsetPartnerReq, this);
}

BOOL CPartnerModule::ToTransferData(TransferDataItem* pTransItem)
{
	for (int i = 0; i < PARTNER_MAX_NUM; i++)
	{
		if (m_vtSetupPartner[i] == NULL)
		{
			continue;
		}

		PartnerDataObject* pObject = m_vtSetupPartner[i];
		if (pObject == NULL)
		{
			return TRUE;
		}

		StPartnerInfo* pPartnerInfo = CStaticData::GetInstancePtr()->GetPartnerInfo(pObject->m_PartnerID);
		ERROR_RETURN_FALSE(pPartnerInfo != NULL);

		StActorInfo* pActorInfo = CStaticData::GetInstancePtr()->GetActorInfo(pPartnerInfo->dwActorID);
		ERROR_RETURN_FALSE(pActorInfo != NULL);

		TransPartnerData* pPartnerData = pTransItem->mutable_partnerdata();
		ERROR_RETURN_FALSE(pPartnerData != NULL);

		pPartnerData->set_actorid(pPartnerInfo->dwActorID);
		pPartnerData->set_level(pObject->m_StrengthLvl);
		pPartnerData->set_partnerguid(pObject->m_uGuid);
		pPartnerData->set_partnerid(pObject->m_PartnerID);

		for (int i = 0; i < PROPERTY_NUM; i++)
		{
			pPartnerData->add_propertys(pActorInfo->Propertys[i]);
		}

		StActorSkillInfo* pActorSkillInfo = CStaticData::GetInstancePtr()->GetActorSkillInfo(pPartnerInfo->dwActorID);
		ERROR_RETURN_FALSE(pActorSkillInfo != NULL);

		SkillItem* pSkillItem = pPartnerData->add_skills();
		pSkillItem->set_keypos(1);
		pSkillItem->set_level(1);
		pSkillItem->set_skillid(pActorSkillInfo->NormalID);

		return TRUE;
	}

	return TRUE;
}

BOOL CPartnerModule::OnMsgSetupPartnerReq(NetPacket* pNetPacket)
{
	SetupPartnerReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	if (Req.targetpos() <= 0 || Req.targetpos() > PARTNER_MAX_NUM)
	{
		SetupPartnerAck Ack;
		Ack.set_retcode(MRC_UNKNOW_ERROR);
		m_pOwnPlayer->SendMsgProtoBuf(MSG_SETUP_PARTNER_ACK, Ack);
		return TRUE;
	}

	PartnerDataObject* pPartnerObj = GetPartnerByGuid(Req.partnerguid());
	if(pPartnerObj == NULL)
	{
		SetupPartnerAck Ack;
		Ack.set_retcode(MRC_UNKNOW_ERROR);
		m_pOwnPlayer->SendMsgProtoBuf(MSG_SETUP_PARTNER_ACK, Ack);
		return TRUE;
	}

	if (m_vtSetupPartner[Req.targetpos() - 1] != NULL)
	{
		m_vtSetupPartner[Req.targetpos() - 1]->Lock();
		m_vtSetupPartner[Req.targetpos() - 1]->m_SetPos = 0;
		m_vtSetupPartner[Req.targetpos() - 1]->Unlock();
		AddChangeID(m_vtSetupPartner[Req.targetpos() - 1]->m_uGuid);
	}

	m_vtSetupPartner[Req.targetpos() - 1] = pPartnerObj;
	pPartnerObj->Lock();
	pPartnerObj->m_SetPos = Req.targetpos();
	pPartnerObj->Unlock();
	AddChangeID(pPartnerObj->m_uGuid);

	SetupPartnerAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	m_pOwnPlayer->SendMsgProtoBuf(MSG_SETUP_PARTNER_ACK, Ack);

	return TRUE;
}

BOOL CPartnerModule::OnMsgUnsetPartnerReq(NetPacket* pNetPacket)
{
	UnsetPartnerReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	if (Req.targetpos() <= 0 || Req.targetpos() > PARTNER_MAX_NUM)
	{
		SetupPartnerAck Ack;
		Ack.set_retcode(MRC_UNKNOW_ERROR);
		m_pOwnPlayer->SendMsgProtoBuf(MSG_UNSET_PARTNER_ACK, Ack);
		return TRUE;
	}

	if (m_vtSetupPartner[Req.targetpos() - 1] != NULL)
	{
		m_vtSetupPartner[Req.targetpos() - 1]->Lock();
		m_vtSetupPartner[Req.targetpos() - 1]->m_SetPos = 0;
		m_vtSetupPartner[Req.targetpos() - 1]->Unlock();
		AddChangeID(m_vtSetupPartner[Req.targetpos() - 1]->m_uGuid);
	}

	UnsetPartnerAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	m_pOwnPlayer->SendMsgProtoBuf(MSG_UNSET_PARTNER_ACK, Ack);

	return TRUE;
}

UINT64 CPartnerModule::AddPartner(UINT32 dwPartnerID)
{
	PartnerDataObject* pObject = DataPool::CreateObject<PartnerDataObject>(ESD_PARTNER, TRUE);
	pObject->Lock();
	pObject->m_PartnerID = dwPartnerID;
	pObject->m_uRoleID = m_pOwnPlayer->GetRoleID();
	pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_StrengthLvl = 1;
	pObject->m_RefineExp = 0;
	pObject->m_StarExp = 0;
	pObject->m_StarLevel = 1;
	pObject->m_RefineLevel = 1;
	pObject->m_SetPos = 0;
	pObject->Unlock();
	m_mapPartnerData.insert(std::make_pair(pObject->m_uGuid, pObject));
	AddChangeID(pObject->m_uGuid);
	return pObject->m_uGuid;
}


BOOL CPartnerModule::NotifyChange()
{
	if (m_setChange.size() <= 0 && m_setRemove.size() <= 0)
	{
		return TRUE;
	}

	PartnerChangeNty Nty;
	for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
	{
		PartnerDataObject* pObject = GetPartnerByGuid(*itor);
		ERROR_CONTINUE_EX(pObject != NULL);
		PartnerItem* pItem = Nty.add_changelist();
		pItem->set_guid(pObject->m_uGuid);
		pItem->set_partnerid(pObject->m_PartnerID);
		pItem->set_strengthlvl(pObject->m_StrengthLvl);
		pItem->set_refinelevel(pObject->m_RefineLevel);
		pItem->set_starlevel(pObject->m_StarLevel);
		pItem->set_refineexp(pObject->m_RefineExp);
		pItem->set_starexp(pObject->m_StarExp);
		pItem->set_setpos(pObject->m_SetPos);
	}

	for(auto itor = m_setRemove.begin(); itor != m_setRemove.end(); itor++)
	{
		Nty.add_removelist(*itor);
	}

	m_pOwnPlayer->SendMsgProtoBuf(MSG_PARTNER_CHANGE_NTY, Nty);

	m_setChange.clear();
	m_setRemove.clear();

	return TRUE;
}

PartnerDataObject* CPartnerModule::GetPartnerByGuid(UINT64 uGuid)
{
	auto itor = m_mapPartnerData.find(uGuid);
	if(itor != m_mapPartnerData.end())
	{
		return itor->second;
	}

	return NULL;
}
