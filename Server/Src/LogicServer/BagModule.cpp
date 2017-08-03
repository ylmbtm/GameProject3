#include "stdafx.h"
#include "BagModule.h"
#include "DataPool.h"
#include "..\ConfigData\ConfigStruct.h"
#include "GlobalDataMgr.h"
#include "..\ConfigData\ConfigData.h"
#include "Utility\Log\Log.h"
#include "..\Message\Game_Define.pb.h"
#include "EquipModule.h"
#include "PlayerObject.h"
#include "..\ServerData\ServerDefine.h"
#include "PetModule.h"
#include "PartnerModule.h"
#include "RoleModule.h"


CBagModule::CBagModule(CPlayerObject* pOwner): CModuleBase(pOwner)
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

BOOL CBagModule::ReadFromLoginAck( DBRoleLoginAck& Ack )
{
	const DBBagData& BagData = Ack.bagdata();
	for(int i = 0; i < BagData.itemlist_size(); i++)
	{
		const DBBagItemData& ItemData = BagData.itemlist(i);

		BagDataObject* pObject = g_pBagDataObjectPool->newOjbect(FALSE);
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
	StItemInfo* pItemInfo = CConfigData::GetInstancePtr()->GetItemInfo(dwItemID);
	ERROR_RETURN_FALSE(pItemInfo == NULL);

	UINT64 uItemGuid = 0;
	INT32  nTempCount = 0;
	switch(pItemInfo->dwType)
	{
		case IMT_EQUIP:
		{
			CEquipModule* pEquipModule = (CEquipModule*)m_pOwnPlayer->GetModuleByType(MT_EQUIP);
			ERROR_RETURN_FALSE(pEquipModule != NULL);
			uItemGuid = pEquipModule->AddEquip(dwItemID);
		}
		break;
		case IMT_PET:
		{
			CPetModule* pPetModule = (CPetModule*)m_pOwnPlayer->GetModuleByType(MT_PET);
			ERROR_RETURN_FALSE(pPetModule != NULL);
			uItemGuid = pPetModule->AddPet(dwItemID);
		}
		break;
		case IMT_PARTNER:
		{
			CPartnerModule* pPartnerModule = (CPartnerModule*)m_pOwnPlayer->GetModuleByType(MT_PARTNER);
			ERROR_RETURN_FALSE(pPartnerModule != NULL);
			uItemGuid = pPartnerModule->AddPartner(dwItemID);
		}
		break;
		case IMT_MONEY:
		{
			CRoleModule* pRoleModule = (CRoleModule*)m_pOwnPlayer->GetModuleByType(MT_PARTNER);
			ERROR_RETURN_FALSE(pRoleModule != NULL);
			pRoleModule->AddMoney(dwItemID, nCount);
		}
		break;
		case IMT_ACTION:
		{
			CRoleModule* pRoleModule = (CRoleModule*)m_pOwnPlayer->GetModuleByType(MT_PARTNER);
			ERROR_RETURN_FALSE(pRoleModule != NULL);
			pRoleModule->AddAction(dwItemID, nCount);
		}
		break;
		case IMT_NORMAL:
		{
			for(auto itor = m_mapBagData.begin(); itor != m_mapBagData.end(); itor++)
			{
				BagDataObject* pTempObject = itor->second;

				if(pTempObject->m_ItemID == dwItemID)
				{

				}
			}
		}
		break;
		default:
		{

		}
	}

	BagDataObject* pObject = g_pBagDataObjectPool->newOjbect(TRUE);
	pObject->lock();
	pObject->m_uGuid = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_BagType = pItemInfo->dwBagType;
	pObject->m_ItemGuid = uItemGuid;
	pObject->m_ItemID = dwItemID;
	pObject->m_nCount = nTempCount;
	pObject->m_uRoleID = m_pOwnPlayer->GetObjectID();
	pObject->unlock();
	m_mapBagData.insert(std::make_pair(pObject->m_uGuid, pObject));
	m_BagChange.insert(pObject->m_uGuid);

	return TRUE;
}

BOOL CBagModule::RemoveItem(UINT32 dwItemID, INT32 nCount)
{
	for(auto itor = m_mapBagData.begin(); itor != m_mapBagData.end(); itor++)
	{
		BagDataObject* pTempObject = itor->second;

		if(pTempObject->m_ItemID != dwItemID)
		{
			continue;
		}


		if(pTempObject->m_nCount < nCount)
		{

		}
	}

	return TRUE;
}

INT32 CBagModule::GetItemCount(UINT32 dwItemID)
{
	INT32 nTotalCount = 0;

	for(auto itor = m_mapBagData.begin(); itor != m_mapBagData.end(); itor++)
	{
		BagDataObject* pTempObject = itor->second;

		if(pTempObject->m_ItemID != dwItemID)
		{
			continue;
		}

		nTotalCount += pTempObject->m_nCount;
	}

	return nTotalCount;
}
