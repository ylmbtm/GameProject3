#include "stdafx.h"
#include "PetModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

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
	/*for(int i = 0; i < CopyData.itemlist_size(); i++)
	{
	const DBBagItemData &ItemData = BagData.itemlist(i);

	BagDataObject *pObject = g_pBagDataObjectPool->newOjbect(FALSE);
	pObject->lock();
	pObject->m_uGuid = ItemData.guid();
	pObject->m_uRoleID = ItemData.roleid();
	pObject->m_BagType = ItemData.bagtype();
	pObject->m_bBind = ItemData.bind();
	pObject->m_ItemGuid = ItemData.itemguid();
	pObject->m_ItemID = ItemData.itemid();
	pObject->unlock();
	m_mapBagData.insert(std::make_pair(pObject->m_uGuid, pObject));
	}*/
	return TRUE;
}

BOOL CPetModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

UINT64 CPetModule::AddPet(UINT32 dwPetID)
{
	PetDataObject* pObject = g_pPetDataObjectPool->newOjbect(TRUE);
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