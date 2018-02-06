#include "stdafx.h"
#include "PartnerModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"

CPartnerModule::CPartnerModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CPartnerModule::~CPartnerModule()
{

}

BOOL CPartnerModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CPartnerModule::OnDestroy()
{
	for(auto itor = m_mapPartnerData.begin(); itor != m_mapPartnerData.end(); itor++)
	{
		itor->second->release();
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

		PartnerDataObject* pObject = g_pPartnerDataObjectPool->NewObject(FALSE);
		pObject->m_uGuid = PartnerItem.guid();
		pObject->m_uRoleID = PartnerItem.roleid();
		pObject->m_PartnerID = PartnerItem.partnerid();
		pObject->m_StrengthLvl = PartnerItem.strengthlvl();
		pObject->m_RefineLevel = PartnerItem.refinelevel();
		pObject->m_StarLevel = PartnerItem.starlevel();
		pObject->m_IsUsing = PartnerItem.isusing();
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
		pItem->set_isusing(pObject->m_IsUsing);
	}
	return TRUE;
}


BOOL CPartnerModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

BOOL CPartnerModule::DispatchPacket(NetPacket* pNetPacket)
{
	return FALSE;
}

UINT64 CPartnerModule::AddPartner(UINT32 dwPartnerID)
{
	PartnerDataObject* pObject = g_pPartnerDataObjectPool->NewObject(TRUE);
	pObject->lock();
	pObject->m_PartnerID = dwPartnerID;
	pObject->m_uRoleID = m_pOwnPlayer->GetObjectID();
	pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_StrengthLvl = 1;
	pObject->m_RefineExp = 0;
	pObject->m_StarExp = 0;
	pObject->m_StarLevel = 1;
	pObject->m_RefineLevel = 1;
	pObject->unlock();
	m_mapPartnerData.insert(std::make_pair(pObject->m_uGuid, pObject));
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
		pItem->set_isusing(pObject->m_IsUsing);
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
