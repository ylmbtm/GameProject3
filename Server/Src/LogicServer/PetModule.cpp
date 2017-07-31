#include "stdafx.h"
#include "PetModule.h"

CPetModule::CPetModule(CPlayerObject *pOwner):CModuleBase(pOwner)
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

BOOL CPetModule::ReadFromLoginAck(DBRoleLoginAck &Ack)
{
	const DBPetData &PetData = Ack.petdata();
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



