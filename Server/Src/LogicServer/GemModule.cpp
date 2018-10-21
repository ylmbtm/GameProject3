#include "stdafx.h"
#include "GemModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../ConfigData/ConfigData.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "BagModule.h"
#include "../ServerData/GemData.h"
#include "../ServerEngine/PacketHeader.h"

CGemModule::CGemModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	for(int i = 0; i < 40; i++)
	{
		m_vtDressGem[i] = NULL;
	}
}

CGemModule::~CGemModule()
{
	for(int i = 0; i < 40; i++)
	{
		m_vtDressGem[i] = NULL;
	}
}

BOOL CGemModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CGemModule::OnDestroy()
{
	for(auto itor = m_mapGemData.begin(); itor != m_mapGemData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapGemData.clear();

	return TRUE;
}

BOOL CGemModule::OnLogin()
{
	return TRUE;
}

BOOL CGemModule::OnLogout()
{
	return TRUE;
}

BOOL CGemModule::OnNewDay()
{
	return TRUE;
}

BOOL CGemModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBGemData& GemData = Ack.gemdata();
	for(int i = 0; i < GemData.gemlist_size(); i++)
	{
		const DBGemItem& ItemData = GemData.gemlist(i);
		GemDataObject* pObject = g_pGemDataObjectPool->NewObject(FALSE);
		pObject->m_uGuid = ItemData.guid();
		pObject->m_uRoleID = ItemData.roleid();
		pObject->m_GemID = ItemData.gemid();
		pObject->m_StrengthLvl = ItemData.strengthlvl();
		pObject->m_RefineLevel = ItemData.refinelevel();
		pObject->m_StarLevel = ItemData.starlevel();
		pObject->m_StarExp = ItemData.starexp();
		pObject->m_RefineExp = ItemData.refineexp();
		pObject->m_EquipPos = ItemData.pos();
		m_mapGemData.insert(std::make_pair(pObject->m_uGuid, pObject));
		if(pObject->m_EquipPos > 0)
		{
			StGemInfo* pInfo = CConfigData::GetInstancePtr()->GetGemInfo(pObject->m_GemID);
			UINT32 dwTargetPos = (pObject->m_EquipPos - 1) * 5 + pInfo->dwPos - 1 ;
			m_vtDressGem[dwTargetPos] = pObject;
		}
	}

	return TRUE;
}

BOOL CGemModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	for (auto itor = m_mapGemData.begin(); itor != m_mapGemData.end(); itor++)
	{
		GemDataObject* pObject = itor->second;
		GemItem* pItem = Ack.add_gemlist();
		pItem->set_guid(pObject->m_uGuid);
		pItem->set_gemid(pObject->m_GemID);
		pItem->set_strengthlvl(pObject->m_StrengthLvl);
		pItem->set_refinelevel(pObject->m_RefineLevel);
		pItem->set_starlevel(pObject->m_StarLevel);
		pItem->set_refineexp(pObject->m_RefineExp);
		pItem->set_starexp(pObject->m_StarExp);
		pItem->set_pos(pObject->m_EquipPos);
	}

	return TRUE;
}

UINT64 CGemModule::AddGem(UINT32 dwGemID)
{
	GemDataObject* pObject = g_pGemDataObjectPool->NewObject(TRUE);
	pObject->lock();
	pObject->m_GemID = dwGemID;
	pObject->m_uRoleID = m_pOwnPlayer->GetObjectID();
	pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_StrengthLvl = 0;
	pObject->m_RefineExp = 0;
	pObject->m_StarExp = 0;
	pObject->m_StarLevel = 0;
	pObject->m_EquipPos = 0;
	pObject->unlock();

	m_mapGemData.insert(std::make_pair(pObject->m_uGuid, pObject));

	m_setChange.insert(pObject->m_uGuid);

	return pObject->m_uGuid;
}

BOOL CGemModule::NotifyChange()
{
	if (m_setChange.size() <= 0 && m_setRemove.size() <= 0)
	{
		return TRUE;
	}

	GemChangeNty Nty;
	for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
	{
		GemDataObject* pObject = GetGemByGuid(*itor);
		ERROR_CONTINUE_EX(pObject != NULL);
		GemItem* pItem = Nty.add_changelist();
		pItem->set_guid(pObject->m_uGuid);
		pItem->set_gemid(pObject->m_GemID);
		pItem->set_strengthlvl(pObject->m_StrengthLvl);
		pItem->set_refinelevel(pObject->m_RefineLevel);
		pItem->set_starlevel(pObject->m_StarLevel);
		pItem->set_refineexp(pObject->m_RefineExp);
		pItem->set_starexp(pObject->m_StarExp);
		pItem->set_pos(pObject->m_EquipPos);
	}

	for(auto itor = m_setRemove.begin(); itor != m_setRemove.end(); itor++)
	{
		Nty.add_removelist(*itor);
	}

	m_pOwnPlayer->SendMsgProtoBuf(MSG_GEM_CHANGE_NTY, Nty);

	m_setChange.clear();
	m_setRemove.clear();

	return TRUE;
}

GemDataObject* CGemModule::GetGemByGuid(UINT64 uGuid)
{
	auto itor = m_mapGemData.find(uGuid);
	if(itor != m_mapGemData.end())
	{
		return itor->second;
	}

	return NULL;
}

UINT32 CGemModule::UnDressGem(UINT64 uGuid)
{
	GemDataObject* pObject = GetGemByGuid(uGuid);
	if (pObject == NULL)
	{
		return MRC_INVALID_EQUIP_ID;
	}

	if (pObject->m_EquipPos <= 0)
	{
		return MRC_UNKNOW_ERROR;
	}

	StGemInfo* pInfo = CConfigData::GetInstancePtr()->GetGemInfo(pObject->m_GemID);
	if (pInfo == NULL)
	{
		return MRC_UNKNOW_ERROR;
	}

	UINT32 dwTargetPos = (pObject->m_EquipPos - 1) * 5 + pInfo->dwPos - 1;

	if (m_vtDressGem[dwTargetPos] != pObject)
	{
		return MRC_UNKNOW_ERROR;
	}

	m_vtDressGem[dwTargetPos] = NULL;
	pObject->lock();
	pObject->m_EquipPos = 0;
	pObject->unlock();
	m_setChange.insert(pObject->m_uGuid);

	CBagModule* pBagModule = (CBagModule*)m_pOwnPlayer->GetModuleByType(MT_BAG);
	if (pBagModule == NULL)
	{
		return MRC_UNKNOW_ERROR;
	}

	pBagModule->AddItem(pObject->m_uGuid, pObject->m_GemID, 1);
	return MRC_SUCCESSED;
}

BOOL CGemModule::OnMsgSetupGemReq(NetPacket* pNetPacket)
{
	SetupGemReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	UINT32 nRetCode = DressGem(Req.gemguid(), Req.bagguid(), Req.targetpos());
	if (nRetCode != MRC_SUCCESSED)
	{
		SetupGemAck Ack;
		Ack.set_retcode(nRetCode);
		m_pOwnPlayer->SendMsgProtoBuf(MSG_SETUP_GEM_ACK, Ack);
	}

	return TRUE;
}

BOOL CGemModule::OnMsgUnsetGemReq(NetPacket* pNetPacket)
{
	UnsetGemReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	UINT32 nRetCode = UnDressGem(Req.gemguid());
	if (nRetCode != MRC_SUCCESSED)
	{
		UnsetEquipAck Ack;
		Ack.set_retcode(nRetCode);
		m_pOwnPlayer->SendMsgProtoBuf(MSG_UNSET_GEM_ACK, Ack);
	}

	return TRUE;
}

UINT32 CGemModule::DressGem(UINT64 uGuid, UINT64 uBagGuid, INT32 EquipPos)
{
	GemDataObject* pObject = GetGemByGuid(uGuid);
	if (pObject == NULL)
	{
		return MRC_INVALID_EQUIP_ID;
	}

	if (pObject->m_EquipPos > 0)
	{
		return MRC_UNKNOW_ERROR;
	}

	StGemInfo* pInfo = CConfigData::GetInstancePtr()->GetGemInfo(pObject->m_GemID);
	if (pInfo == NULL)
	{
		return MRC_UNKNOW_ERROR;
	}

	CBagModule* pBagModule = (CBagModule*)m_pOwnPlayer->GetModuleByType(MT_BAG);
	if (pBagModule == NULL)
	{
		return MRC_UNKNOW_ERROR;
	}

	UINT32 dwTargetPos = (EquipPos - 1) * 5 + pInfo->dwPos - 1;
	if (m_vtDressGem[dwTargetPos] != NULL)
	{
		m_vtDressGem[dwTargetPos]->lock();
		m_vtDressGem[dwTargetPos]->m_EquipPos = 0;
		m_vtDressGem[dwTargetPos]->unlock();
		m_setChange.insert(m_vtDressGem[dwTargetPos]->m_uGuid);
		pBagModule->SetBagItem(uBagGuid, m_vtDressGem[dwTargetPos]->m_uGuid, m_vtDressGem[dwTargetPos]->m_GemID, 1);
	}
	else
	{
		pBagModule->RemoveItem(uBagGuid);
	}

	pObject->lock();
	pObject->m_EquipPos = EquipPos;
	pObject->unlock();
	m_vtDressGem[dwTargetPos] = pObject;
	m_setChange.insert(pObject->m_uGuid);

	return MRC_SUCCESSED;
}

BOOL CGemModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	INT32 nMinStengthLevel = 10000;
	INT32 nMinRefineLevel = 1000;
	for (int i  = 0; i < EQUIP_MAX_NUM; i++)
	{
		if(m_vtDressGem[i] == NULL)
		{
			nMinStengthLevel = 0;
			continue;
		}

		GemDataObject* pObject = m_vtDressGem[i];
		StGemInfo* pInfo = CConfigData::GetInstancePtr()->GetGemInfo(pObject->m_GemID);
		ERROR_RETURN_FALSE(pInfo != NULL);
		if(pObject->m_StrengthLvl >= 1)
		{



		}

	}

	return TRUE;
}

BOOL CGemModule::DispatchPacket(NetPacket* pNetPacket)
{
	switch (pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_SETUP_GEM_REQ,		OnMsgSetupGemReq);
			PROCESS_MESSAGE_ITEM(MSG_UNSET_GEM_REQ,		OnMsgUnsetGemReq);
	}

	return FALSE;
}
