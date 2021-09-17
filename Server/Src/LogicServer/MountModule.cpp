#include "stdafx.h"
#include "MountModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"
#include "PacketHeader.h"
#include "../Message/Msg_RetCode.pb.h"

CMountModule::CMountModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	RegisterMessageHanler();
}

CMountModule::~CMountModule()
{

}

BOOL CMountModule::OnCreate(UINT64 u64RoleID)
{
	for (int i = 0; i < 11; i++)
	{
		AddMount(i + 1);
	}
	return TRUE;
}


BOOL CMountModule::OnDestroy()
{
	for(auto itor = m_mapMountData.begin(); itor != m_mapMountData.end(); itor++)
	{
		itor->second->Release();
	}

	m_mapMountData.clear();

	return TRUE;
}

BOOL CMountModule::OnLogin()
{
	return TRUE;
}

BOOL CMountModule::OnLogout()
{
	return TRUE;
}

BOOL CMountModule::OnNewDay()
{
	return TRUE;
}

BOOL CMountModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBMountData& MountData = Ack.mountdata();
	for(int i = 0; i < MountData.mountlist_size(); i++)
	{
		const DBMountItem& MountItem = MountData.mountlist(i);
		MountDataObject* pObject = DataPool::CreateObject<MountDataObject>(ESD_MOUNT, FALSE);
		pObject->m_uGuid = MountItem.guid();
		pObject->m_uRoleID = MountItem.roleid();
		pObject->m_MountID = MountItem.mountid();
		pObject->m_StrengthLvl = MountItem.strengthlvl();
		pObject->m_RefineLevel = MountItem.refinelevel();
		pObject->m_StarLevel = MountItem.starlevel();
		pObject->m_IsUsing = MountItem.isusing();
		m_mapMountData.insert(std::make_pair(pObject->m_uGuid, pObject));
	}
	return TRUE;
}

BOOL CMountModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	for (auto itor = m_mapMountData.begin(); itor != m_mapMountData.end(); itor++)
	{
		MountDataObject* pObject = itor->second;
		MountItem* pItem = Ack.add_mountlist();
		pItem->set_guid(pObject->m_uGuid);
		pItem->set_mountid(pObject->m_MountID);
		pItem->set_strengthlvl(pObject->m_StrengthLvl);
		pItem->set_refinelevel(pObject->m_RefineLevel);
		pItem->set_starlevel(pObject->m_StarLevel);
		pItem->set_refineexp(pObject->m_RefineExp);
		pItem->set_starexp(pObject->m_StarExp);
		pItem->set_isusing(pObject->m_IsUsing);
	}

	m_setChange.clear();
	m_setRemove.clear();

	return TRUE;
}

BOOL CMountModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

VOID CMountModule::RegisterMessageHanler()
{
    m_pOwnPlayer->RegisterMessageHandle(MSG_SETUP_MOUNT_REQ, &CMountModule::OnMsgSetupMountReq, this);
    m_pOwnPlayer->RegisterMessageHandle(MSG_UNSET_MOUNT_REQ, &CMountModule::OnMsgUnsetMountReq, this);

}

UINT64 CMountModule::AddMount(UINT32 dwMountID)
{
	MountDataObject* pObject = DataPool::CreateObject<MountDataObject>(ESD_MOUNT, TRUE);
	pObject->Lock();
	pObject->m_uRoleID = m_pOwnPlayer->GetRoleID();
	pObject->m_MountID = dwMountID;
	pObject->m_uGuid   = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	pObject->m_StrengthLvl = 0;
	pObject->m_RefineExp = 0;
	pObject->m_StarExp = 0;
	pObject->m_StarLevel = 0;
	pObject->m_IsUsing = FALSE;
	pObject->Unlock();
	m_mapMountData.insert(std::make_pair(pObject->m_uGuid, pObject));
	AddChangeID(pObject->m_uGuid);
	return pObject->m_uGuid;
}


BOOL CMountModule::NotifyChange()
{
	if (m_setChange.size() <= 0 && m_setRemove.size() <= 0)
	{
		return TRUE;
	}

	MountChangeNty Nty;
	for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
	{
		MountDataObject* pObject = GetMountByGuid(*itor);
		ERROR_CONTINUE_EX(pObject != NULL);
		MountItem* pItem = Nty.add_changelist();
		pItem->set_guid(pObject->m_uGuid);
		pItem->set_mountid(pObject->m_MountID);
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

	m_pOwnPlayer->SendMsgProtoBuf(MSG_MOUNT_CHANGE_NTY, Nty);

	m_setChange.clear();
	m_setRemove.clear();

	return TRUE;
}

MountDataObject* CMountModule::GetMountByGuid(UINT64 uGuid)
{
	auto itor = m_mapMountData.find(uGuid);
	if(itor != m_mapMountData.end())
	{
		return itor->second;
	}

	return NULL;
}

MountDataObject* CMountModule::GetCurrentMountData()
{
	for (auto itor = m_mapMountData.begin(); itor != m_mapMountData.end(); itor++)
	{
		MountDataObject* pObject = itor->second;
		if (pObject->m_IsUsing)
		{
			return pObject;
		}
	}

	return NULL;
}

BOOL CMountModule::OnMsgSetupMountReq(NetPacket* pNetPacket)
{
	SetupMountReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	MountDataObject* pCurObject = GetCurrentMountData();
	if (pCurObject != NULL)
	{
		pCurObject->Lock();
		pCurObject->m_IsUsing = false;
		pCurObject->Unlock();
		AddChangeID(pCurObject->m_uGuid);
	}

	MountDataObject* pTargetObject = GetMountByGuid(Req.mountguid());
	if(pTargetObject != NULL)
	{
		pTargetObject->Lock();
		pTargetObject->m_IsUsing = TRUE;
		pTargetObject->Unlock();
		AddChangeID(pTargetObject->m_uGuid);
		m_pOwnPlayer->SendPlayerChange(ECT_MOUNT, 0, pTargetObject->m_MountID, "");
	}



	SetupMountAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	m_pOwnPlayer->SendMsgProtoBuf(MSG_SETUP_MOUNT_ACK, Ack);

	return TRUE;
}

BOOL CMountModule::OnMsgUnsetMountReq(NetPacket* pNetPacket)
{
	UnsetMountReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	MountDataObject* pCurObject = GetCurrentMountData();
	if (pCurObject != NULL)
	{
		pCurObject->Lock();
		pCurObject->m_IsUsing = false;
		pCurObject->Unlock();
		AddChangeID(pCurObject->m_uGuid);
	}

	UnsetMountAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	m_pOwnPlayer->SendMsgProtoBuf(MSG_UNSET_MOUNT_ACK, Ack);

	return TRUE;
}
