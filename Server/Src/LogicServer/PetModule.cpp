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
		pObject->lock();
		pObject->unlock();
		m_mapPetData.insert(std::make_pair(pObject->m_uGuid, pObject));
	}
	return TRUE;
}

BOOL CPetModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CPetModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

UINT64 CPetModule::AddPet(UINT32 dwPetID)
{
	PetDataObject* pObject = g_pPetDataObjectPool->NewObject(TRUE);
	pObject->lock();
	pObject->m_PetID = dwPetID;
	pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_StrengthLvl = 0;
	pObject->m_RefineExp = 0;
	pObject->m_StarExp = 0;
	pObject->m_StarLevel = 0;
	pObject->unlock();
	m_mapPetData.insert(std::make_pair(pObject->m_uGuid, pObject));
	return pObject->m_uGuid;
}


BOOL CPetModule::NotifyChange()
{
	PetChangeNty Nty;
	for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
	{
		PetDataObject* pObject = GetPetByGuid(*itor);
		ERROR_CONTINUE_EX(pObject != NULL);

		PetItem* pItem = Nty.add_changelist();
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
