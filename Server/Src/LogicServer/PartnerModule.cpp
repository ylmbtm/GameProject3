#include "stdafx.h"
#include "PartnerModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

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

BOOL CPartnerModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}


UINT64 CPartnerModule::AddPartner(UINT32 dwPartnerID)
{
	PartnerDataObject* pObject = g_pPartnerDataObjectPool->newOjbect(TRUE);
	pObject->lock();
	pObject->m_PartnerID = dwPartnerID;
	pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_StrengthLvl = 0;
	pObject->m_RefineExp = 0;
	pObject->m_StarExp = 0;
	pObject->m_StarLevel = 0;
	pObject->unlock();
	m_mapPartnerData.insert(std::make_pair(pObject->m_uGuid, pObject));
	return pObject->m_uGuid;
}
