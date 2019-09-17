#include "stdafx.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "SkillModule.h"
#include "../StaticData/StaticStruct.h"
#include "PlayerObject.h"
#include "../StaticData/StaticData.h"

CSkillModule::CSkillModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	RegisterMessageHanler();
}

CSkillModule::~CSkillModule()
{

}

BOOL CSkillModule::OnCreate(UINT64 u64RoleID)
{
	UINT32 dwActorID = m_pOwnPlayer->GetActorID();
	ERROR_RETURN_FALSE(dwActorID != 0);

	StActorSkillInfo* pActorSkillInfo = CStaticData::GetInstancePtr()->GetActorSkillInfo(dwActorID);
	ERROR_RETURN_FALSE(pActorSkillInfo != NULL);

	SkillDataObject* pObject = DataPool::CreateObject<SkillDataObject>(ESD_SKILL, TRUE);
	pObject->Lock();
	pObject->m_nKeyPos = 1;
	pObject->m_nLevel = 1;
	pObject->m_uRoleID = u64RoleID;
	pObject->m_dwSkillID = pActorSkillInfo->NormalID;
	pObject->Unlock();
	m_mapSkillData.insert(std::make_pair(pObject->m_dwSkillID, pObject));

	for (int i = 0; i < 5; i++)
	{
		if (pActorSkillInfo->Specials[i] == 0)
		{
			break;
		}

		pObject = DataPool::CreateObject<SkillDataObject>(ESD_SKILL, TRUE);
		pObject->Lock();
		pObject->m_nKeyPos = i + 2;
		pObject->m_nLevel = 1;
		pObject->m_uRoleID = u64RoleID;
		pObject->m_dwSkillID = pActorSkillInfo->Specials[i];
		pObject->Unlock();
		m_mapSkillData.insert(std::make_pair(pObject->m_dwSkillID, pObject));
	}

	return TRUE;
}


BOOL CSkillModule::OnDestroy()
{
	for(auto itor = m_mapSkillData.begin(); itor != m_mapSkillData.end(); itor++)
	{
		itor->second->Release();
	}

	m_mapSkillData.clear();

	return TRUE;
}

BOOL CSkillModule::OnLogin()
{
	return TRUE;
}

BOOL CSkillModule::OnLogout()
{
	return TRUE;
}

BOOL CSkillModule::OnNewDay()
{
	return TRUE;
}

VOID CSkillModule::RegisterMessageHanler()
{
}

BOOL CSkillModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBSkillData& SkillData = Ack.skilldata();
	for(int i = 0; i < SkillData.skilllist_size(); i++)
	{
		const DBSkillItem& SkillItem = SkillData.skilllist(i);
		SkillDataObject* pObject = DataPool::CreateObject<SkillDataObject>(ESD_SKILL, FALSE);
		pObject->m_nKeyPos = SkillItem.keypos();
		pObject->m_nLevel = SkillItem.level();
		pObject->m_uRoleID = SkillItem.roleid();
		pObject->m_dwSkillID = SkillItem.skillid();
		m_mapSkillData.insert(std::make_pair(pObject->m_dwSkillID, pObject));
	}

	if (m_mapSkillData.size() <= 0)
	{
		OnCreate(Ack.roleid());
	}

	return TRUE;
}

BOOL CSkillModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	for (auto itor = m_mapSkillData.begin(); itor != m_mapSkillData.end(); itor++)
	{
		SkillDataObject* pObject = itor->second;
		SkillItem* pItem = Ack.add_skilllist();
		pItem->set_skillid(pObject->m_dwSkillID);
		pItem->set_level(pObject->m_nLevel);
		pItem->set_keypos(pObject->m_nKeyPos);
	}

	return TRUE;
}

BOOL CSkillModule::NotifyChange()
{
	return TRUE;
}

SkillDataObject* CSkillModule::GetSkillData(UINT32 dwSkillID)
{
	auto itor = m_mapSkillData.find(dwSkillID);
	if (itor != m_mapSkillData.end())
	{
		return itor->second;
	}

	return NULL;
}
