#include "stdafx.h"
#include "EquipModule.h"

CEquipModule::CEquipModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CEquipModule::~CEquipModule()
{

}

BOOL CEquipModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CEquipModule::OnDestroy()
{
	return TRUE;
}

BOOL CEquipModule::OnLogin()
{
	return TRUE;
}

BOOL CEquipModule::OnLogout()
{
	return TRUE;
}

BOOL CEquipModule::OnNewDay()
{
	return TRUE;
}

BOOL CEquipModule::ReadFromLoginAck(DBRoleLoginAck &Ack)
{
	const DBEquipData &EquipData = Ack.equipdata();
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



