#include "stdafx.h"
#include "PetModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "PlayerObject.h"
#include "Log.h"
#include "../Message/Msg_ID.pb.h"

CPetModule::CPetModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CPetModule::~CPetModule()
{

}

BOOL CPetModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CPetModule::OnDestroy()
{
	for(auto itor = m_mapPetData.begin(); itor != m_mapPetData.end(); itor++)
	{
		itor->second->release();
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
		PetDataObject* pObject = g_pPetDataObjectPool->NewObject(FALSE);
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
	return TRUE;
}

BOOL CPetModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

BOOL CPetModule::DispatchPacket(NetPacket* pNetPacket)
{
	return FALSE;
}

UINT64 CPetModule::AddPet(UINT32 dwPetID)
{
	PetDataObject* pObject = g_pPetDataObjectPool->NewObject(TRUE);
	pObject->lock();
	pObject->m_PetID = dwPetID;
	pObject->m_uRoleID = m_pOwnPlayer->GetObjectID();
	pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_StrengthLvl = 1;
	pObject->m_RefineExp = 0;
	pObject->m_StarExp = 0;
	pObject->m_StarLevel = 1;
	pObject->m_RefineLevel = 1;
	pObject->unlock();
	m_mapPetData.insert(std::make_pair(pObject->m_uGuid, pObject));
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
