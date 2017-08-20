#include "stdafx.h"
#include "BagModule.h"
#include "DataPool.h"
#include "..\ConfigData\ConfigStruct.h"
#include "GlobalDataMgr.h"
#include "..\ConfigData\ConfigData.h"
#include "Utility\Log\Log.h"
#include "..\Message\Game_Define.pb.h"
#include "..\Message\Msg_ID.pb.h"
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

BOOL CBagModule::ReadFromDBLoginData( DBRoleLoginAck& Ack )
{
	const DBBagData& BagData = Ack.bagdata();
	for(int i = 0; i < BagData.itemlist_size(); i++)
	{
		const DBBagItem& ItemData = BagData.itemlist(i);

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

BOOL CBagModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	for(auto itor = m_mapBagData.begin(); itor != m_mapBagData.end(); itor++)
	{
		BagDataObject* pObject = itor->second;

		BagItem* pItem = Ack.add_bagitemlist();
		/*	pItem->set_bagtype(pObject->m_BagType);
			pItem->set_count(pObject->m_nCount);
			pItem->set_bind(pObject->m_bBind);
			pItem->set_guid(pObject->m_uGuid);
			pItem->set_itemguid(pObject->m_ItemGuid);
			pItem->set_itemid(pObject->m_ItemID);*/
	}

	return TRUE;
}

BOOL CBagModule::CalcFightValue(INT32 nValue[MAX_PROPERTY_NUM], INT32 nPercent[MAX_PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

BOOL CBagModule::AddItem(UINT32 dwItemID, INT32 nCount)
{
	StItemInfo* pItemInfo = CConfigData::GetInstancePtr()->GetItemInfo(dwItemID);
	ERROR_RETURN_FALSE(pItemInfo == NULL);

	UINT64 uItemGuid = 0;
	INT32  nTempCount = nCount;
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

				if(pTempObject->m_ItemID != dwItemID)
				{
					continue;
				}
				INT32 nCanAdd = pItemInfo->StackMax - pTempObject->m_nCount;
				if( nCanAdd <= 0)
				{
					continue;
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
	INT32 nLeftCount = nCount;
	for(auto itor = m_mapBagData.begin(); itor != m_mapBagData.end(); )
	{
		if(nLeftCount <= 0)
		{
			return TRUE;
		}

		BagDataObject* pTempObject = itor->second;

		if(pTempObject->m_ItemID != dwItemID)
		{
			++itor;
			continue;
		}

		if(pTempObject->m_nCount <= nLeftCount)
		{
			pTempObject->m_nCount = 0;
			nLeftCount -= pTempObject->m_nCount;
			pTempObject->destroy();

			itor = m_mapBagData.erase(itor);
			continue;
		}
		else
		{
			pTempObject->m_nCount -= nLeftCount;
			nLeftCount = 0;
			return TRUE;
		}

		++itor;
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

BagDataObject* CBagModule::GetItemByGuid(UINT64 uGuid)
{
	auto itor = m_mapBagData.find(uGuid);
	if(itor != m_mapBagData.end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CBagModule::NotifyChange()
{
	BagChangeNty Nty;
	for(auto itor = m_BagChange.begin(); itor != m_BagChange.end(); itor++)
	{
		BagDataObject* pObject = GetItemByGuid(*itor);
		ERROR_CONTINUE_EX(pObject != NULL);


		BagItem* pItem = Nty.add_changelist();
		pItem->set_guid(*itor);
		pItem->set_itemguid(pObject->m_ItemGuid);
		pItem->set_itemid(pObject->m_ItemID);
		pItem->set_itemnum(pObject->m_nCount);
	}

	for(auto itor = m_BagRemove.begin(); itor != m_BagRemove.end(); itor++)
	{
		Nty.add_removelist(*itor);
	}

	m_pOwnPlayer->SendMsgProtoBuf(MSG_BAG_CHANGE_NTY, Nty);

	m_BagChange.clear();
	m_BagRemove.clear();

	return TRUE;
}
