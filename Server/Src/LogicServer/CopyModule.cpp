#include "stdafx.h"
#include "CopyModule.h"

CCopyModule::CCopyModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CCopyModule::~CCopyModule()
{

}

BOOL CCopyModule::OnCreate(UINT64 u64RoleID)
{
	
	return TRUE;
}


BOOL CCopyModule::OnDestroy()
{
	return TRUE;
}

BOOL CCopyModule::OnLogin()
{
	return TRUE;
}

BOOL CCopyModule::OnLogout()
{
	return TRUE;
}

BOOL CCopyModule::OnNewDay()
{
	return TRUE;
}

BOOL CCopyModule::ReadFromLoginAck(DBRoleLoginAck &Ack)
{
	const DBCopyData &CopyData = Ack.copydata();
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
