#include "stdafx.h"
#include "BagModule.h"
#include "DataPool.h"
#include "..\ConfigData\ConfigStruct.h"
#include "GlobalDataMgr.h"


CBagModule::CBagModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CBagModule::~CBagModule()
{

}

BOOL CBagModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CBagModule::OnDestroy()
{
	return TRUE;
}

BOOL CBagModule::OnLogin()
{
	return TRUE;
}

BOOL CBagModule::OnLogout()
{
	return TRUE;
}

BOOL CBagModule::OnNewDay()
{
	return TRUE;
}

BOOL CBagModule::ReadFromLoginAck( DBRoleLoginAck &Ack )
{
	const DBBagData &BagData = Ack.bagdata();
	for(int i = 0; i < BagData.itemlist_size(); i++)
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
	}

	return TRUE;
}

BOOL CBagModule::AddItem(UINT32 dwItemID, INT32 nCount)
{
	StItemInfo *pItem = NULL;
	
	BagDataObject *pObject = NULL;
	
	for(auto itor = m_mapBagData.begin(); itor != m_mapBagData.end(); itor++)
	{
		BagDataObject *pTempObject = itor->second;

		if(pTempObject->m_ItemID == dwItemID)
		{

		}
	}

	pObject = g_pBagDataObjectPool->newOjbect(TRUE);
	pObject->lock();
	pObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_ItemID = dwItemID;
	pObject->m_nCount = nCount;
	pObject->unlock();
	m_BagChange.insert(pObject->m_uGuid);

	return TRUE;
}

BOOL CBagModule::RemoveItem(UINT32 dwItemID, INT32 nCount)
{
	for(auto itor = m_mapBagData.begin(); itor != m_mapBagData.end(); itor++)
	{
		BagDataObject *pTempObject = itor->second;

		if(pTempObject->m_ItemID == dwItemID)
		{

		}
	}

	return TRUE;
}
