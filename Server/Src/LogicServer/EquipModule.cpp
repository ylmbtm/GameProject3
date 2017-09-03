#include "stdafx.h"
#include "EquipModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../ConfigData/ConfigData.h"
#include "Log.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"

CEquipModule::CEquipModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	for(int i = 0; i < 8; i++)
	{
		m_vtDressEquip[i] = NULL;
	}
}

CEquipModule::~CEquipModule()
{
	for(int i = 0; i < 8; i++)
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
		EquipDataObject* pObject = g_pEquipDataObjectPool->NewOjbect(FALSE);
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
	return TRUE;
}

UINT64 CEquipModule::AddEquip(UINT32 dwEquipID)
{
	EquipDataObject* pObject = g_pEquipDataObjectPool->NewOjbect(TRUE);
	pObject->lock();
	pObject->m_EquipID = dwEquipID;
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
	EquipChangeNty Nty;
	for(auto itor = m_setChange.begin(); itor != m_setChange.end(); itor++)
	{
		EquipDataObject* pObject = GetEquipByGuid(*itor);
		ERROR_CONTINUE_EX(pObject != NULL);

		EquipItem* pItem = Nty.add_changelist();
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

BOOL CEquipModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	INT32 nMinStengthLevel = 10000;
	INT32 nMinRefineLevel = 1000;
	for (int i  = 0; i < 8; i++)
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