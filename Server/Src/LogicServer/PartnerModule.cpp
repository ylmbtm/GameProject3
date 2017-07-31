#include "stdafx.h"
#include "PartnerModule.h"

CPartnerModule::CPartnerModule(CPlayerObject *pOwner):CModuleBase(pOwner)
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

BOOL CPartnerModule::ReadFromLoginAck(DBRoleLoginAck &Ack)
{
	const DBPartnerData &PartnerData = Ack.partnerdata();
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



