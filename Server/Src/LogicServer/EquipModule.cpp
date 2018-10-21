#include "stdafx.h"
#include "EquipModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../ConfigData/ConfigData.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "BagModule.h"
#include "PacketHeader.h"

CEquipModule::CEquipModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	for(int i = 0; i < EQUIP_MAX_NUM; i++)
	{
		m_vtDressEquip[i] = NULL;
	}
}

CEquipModule::~CEquipModule()
{
	for(int i = 0; i < EQUIP_MAX_NUM; i++)
	{
		m_vtDressEquip[i] = NULL;
	}
}

BOOL CEquipModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CEquipModule::OnDestroy()
{
	for(auto itor = m_mapEquipData.begin(); itor != m_mapEquipData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapEquipData.clear();

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

BOOL CEquipModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBEquipData& EquipData = Ack.equipdata();
	for(int i = 0; i < EquipData.equiplist_size(); i++)
	{
		const DBEquipItem& ItemData = EquipData.equiplist(i);
		EquipDataObject* pObject = g_pEquipDataObjectPool->NewObject(FALSE);
		pObject->m_uGuid = ItemData.guid();
		pObject->m_uRoleID = ItemData.roleid();
		pObject->m_EquipID = ItemData.equipid();
		pObject->m_StrengthLvl = ItemData.strengthlvl();
		pObject->m_RefineLevel = ItemData.refinelevel();
		pObject->m_StarLevel = ItemData.starlevel();
		pObject->m_StarExp = ItemData.starexp();
		pObject->m_RefineExp = ItemData.refineexp();
		pObject->m_IsUsing = ItemData.isusing();
		m_mapEquipData.insert(std::make_pair(pObject->m_uGuid, pObject));
		if(pObject->m_IsUsing == TRUE)
		{
			StEquipInfo* pInfo = CConfigData::GetInstancePtr()->GetEquipInfo(pObject->m_EquipID);
			ERROR_RETURN_FALSE(pInfo != NULL);
			m_vtDressEquip[pInfo->dwPos - 1] = pObject;
		}
	}

	return TRUE;
}

BOOL CEquipModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	for (auto itor = m_mapEquipData.begin(); itor != m_mapEquipData.end(); itor++)
	{
		EquipDataObject* pObject = itor->second;
		EquipItem* pItem = Ack.add_equiplist();
		pItem->set_guid(pObject->m_uGuid);
		pItem->set_equipid(pObject->m_EquipID);
		pItem->set_strengthlvl(pObject->m_StrengthLvl);
		pItem->set_refinelevel(pObject->m_RefineLevel);
		pItem->set_starlevel(pObject->m_StarLevel);
		pItem->set_refineexp(pObject->m_RefineExp);
		pItem->set_starexp(pObject->m_StarExp);
		pItem->set_isusing(pObject->m_IsUsing);
	}

	return TRUE;
}

UINT64 CEquipModule::AddEquip(UINT32 dwEquipID)
{
	EquipDataObject* pObject = g_pEquipDataObjectPool->NewObject(TRUE);
	pObject->lock();
	pObject->m_EquipID = dwEquipID;
	pObject->m_uRoleID = m_pOwnPlayer->GetObjectID();
	pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_StrengthLvl = 0;
	pObject->m_RefineExp = 0;
	pObject->m_StarExp = 0;
	pObject->m_StarLevel = 0;
	pObject->m_IsUsing = FALSE;
	pObject->unlock();

	m_mapEquipData.insert(std::make_pair(pObject->m_uGuid, pObject));

	m_setChange.insert(pObject->m_uGuid);

	return pObject->m_uGuid;
}

BOOL CEquipModule::NotifyChange()
{
	if (m_setChange.size() <= 0 && m_setRemove.size() <= 0)
	{
		return TRUE;
	}

	EquipChangeNty Nty;
	for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
	{
		EquipDataObject* pObject = GetEquipByGuid(*itor);
		ERROR_CONTINUE_EX(pObject != NULL);
		EquipItem* pItem = Nty.add_changelist();
		pItem->set_guid(pObject->m_uGuid);
		pItem->set_equipid(pObject->m_EquipID);
		pItem->set_strengthlvl(pObject->m_StrengthLvl);
		pItem->set_refinelevel(pObject->m_RefineLevel);
		pItem->set_starlevel(pObject->m_StarLevel);
		pItem->set_refineexp(pObject->m_RefineExp);
		pItem->set_starexp(pObject->m_StarExp);
		pItem->set_isusing(pObject->m_IsUsing);
	}

	for(auto itor = m_setRemove.begin(); itor != m_setRemove.end(); itor++)
	{
		Nty.add_removelist(*itor);
	}

	m_pOwnPlayer->SendMsgProtoBuf(MSG_EQUIP_CHANGE_NTY, Nty);

	m_setChange.clear();
	m_setRemove.clear();

	return TRUE;
}

EquipDataObject* CEquipModule::GetEquipByGuid(UINT64 uGuid)
{
	auto itor = m_mapEquipData.find(uGuid);
	if(itor != m_mapEquipData.end())
	{
		return itor->second;
	}

	return NULL;
}

UINT32 CEquipModule::UnDressEquip(UINT64 uGuid)
{
	EquipDataObject* pObject = GetEquipByGuid(uGuid);
	if (pObject == NULL)
	{
		return MRC_INVALID_EQUIP_ID;
	}

	if (!pObject->m_IsUsing)
	{
		return MRC_UNKNOW_ERROR;
	}

	StEquipInfo* pInfo = CConfigData::GetInstancePtr()->GetEquipInfo(pObject->m_EquipID);
	if (pInfo == NULL)
	{
		return MRC_UNKNOW_ERROR;
	}

	if (m_vtDressEquip[pInfo->dwPos - 1] != pObject)
	{
		return MRC_UNKNOW_ERROR;
	}


	pObject->lock();
	pObject->m_IsUsing = FALSE;
	pObject->unlock();
	m_setChange.insert(pObject->m_uGuid);
	m_vtDressEquip[pInfo->dwPos - 1] = NULL;

	CBagModule* pBagModule = (CBagModule*)m_pOwnPlayer->GetModuleByType(MT_BAG);
	if (pBagModule == NULL)
	{
		return MRC_UNKNOW_ERROR;
	}

	pBagModule->AddItem(pObject->m_uGuid, pObject->m_EquipID, 1);

	m_pOwnPlayer->SendObjectChangeNtf(1, pInfo->dwPos, 0, "");

	return MRC_SUCCESSED;
}

UINT32 CEquipModule::DressEquip(UINT64 uGuid, UINT64 uBagGuid)
{
	EquipDataObject* pObject = GetEquipByGuid(uGuid);
	if (pObject == NULL)
	{
		return MRC_INVALID_EQUIP_ID;
	}

	if (pObject->m_IsUsing)
	{
		return MRC_UNKNOW_ERROR;
	}

	StEquipInfo* pInfo = CConfigData::GetInstancePtr()->GetEquipInfo(pObject->m_EquipID);
	if (pInfo == NULL)
	{
		return MRC_UNKNOW_ERROR;
	}

	CBagModule* pBagModule = (CBagModule*)m_pOwnPlayer->GetModuleByType(MT_BAG);
	if (pBagModule == NULL)
	{
		return MRC_UNKNOW_ERROR;
	}

	if (m_vtDressEquip[pInfo->dwPos - 1] != NULL)
	{
		m_vtDressEquip[pInfo->dwPos - 1]->lock();
		m_vtDressEquip[pInfo->dwPos - 1]->m_IsUsing = FALSE;
		m_vtDressEquip[pInfo->dwPos - 1]->unlock();
		m_setChange.insert(m_vtDressEquip[pInfo->dwPos - 1]->m_uGuid);
		pBagModule->SetBagItem(uBagGuid, m_vtDressEquip[pInfo->dwPos - 1]->m_uGuid, m_vtDressEquip[pInfo->dwPos - 1]->m_EquipID, 1);
	}
	else
	{
		pBagModule->RemoveItem(uBagGuid);
	}

	pObject->lock();
	pObject->m_IsUsing = TRUE;
	pObject->unlock();
	m_vtDressEquip[pInfo->dwPos - 1] = pObject;
	m_setChange.insert(pObject->m_uGuid);

	m_pOwnPlayer->SendObjectChangeNtf(1, pInfo->dwPos, pObject->m_EquipID, "");

	return MRC_SUCCESSED;
}

BOOL CEquipModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	INT32 nMinStengthLevel = 10000;
	INT32 nMinRefineLevel = 1000;
	for (int i  = 0; i < EQUIP_MAX_NUM; i++)
	{
		if(m_vtDressEquip[i] == NULL)
		{
			nMinStengthLevel = 0;
			continue;
		}

		EquipDataObject* pObject = m_vtDressEquip[i];
		StEquipInfo* pInfo = CConfigData::GetInstancePtr()->GetEquipInfo(pObject->m_EquipID);
		ERROR_RETURN_FALSE(pInfo != NULL);
		if(pObject->m_StrengthLvl >= 1)
		{



		}

	}

	return TRUE;
}

BOOL CEquipModule::DispatchPacket(NetPacket* pNetPacket)
{
	switch (pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_SETUP_EQUIP_REQ, OnMsgSetupEquipReq);
			PROCESS_MESSAGE_ITEM(MSG_UNSET_EQUIP_REQ, OnMsgUnsetEquipReq);
	}

	return FALSE;
}


BOOL CEquipModule::OnMsgSetupEquipReq(NetPacket* pNetPacket)
{
	SetupEquipReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	UINT32 nRetCode = DressEquip(Req.equipguid(), Req.bagguid());
	if (nRetCode != MRC_SUCCESSED)
	{
		SetupEquipAck Ack;
		Ack.set_retcode(nRetCode);
		m_pOwnPlayer->SendMsgProtoBuf(MSG_SETUP_EQUIP_ACK, Ack);
	}

	return TRUE;
}

BOOL CEquipModule::OnMsgUnsetEquipReq(NetPacket* pNetPacket)
{
	UnsetEquipReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	UINT32 nRetCode = UnDressEquip(Req.equipguid());
	if (nRetCode != MRC_SUCCESSED)
	{
		UnsetEquipAck Ack;
		Ack.set_retcode(nRetCode);
		m_pOwnPlayer->SendMsgProtoBuf(MSG_UNSET_EQUIP_ACK, Ack);
	}

	return TRUE;
}
