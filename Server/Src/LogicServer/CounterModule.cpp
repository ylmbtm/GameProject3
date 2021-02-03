#include "stdafx.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "CounterID.h"
#include "CounterModule.h"
#include "PlayerObject.h"

CCounterModule::CCounterModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	RegisterMessageHanler();
}

CCounterModule::~CCounterModule()
{

}

BOOL CCounterModule::OnCreate(UINT64 u64RoleID)
{
	return TRUE;
}


BOOL CCounterModule::OnDestroy()
{
	for(auto itor = m_mapCounterData.begin(); itor != m_mapCounterData.end(); itor++)
	{
		CounterDataObject* pObject = itor->second;

		pObject->Release();
	}

	m_mapCounterData.clear();

	return TRUE;
}

BOOL CCounterModule::OnLogin()
{
	return TRUE;
}

BOOL CCounterModule::OnLogout()
{
	return TRUE;
}

BOOL CCounterModule::OnNewDay()
{
	return TRUE;
}

VOID CCounterModule::RegisterMessageHanler()
{
}

BOOL CCounterModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBCounterData& CounterData = Ack.counterdata();
	for(int i = 0; i < CounterData.counterlist_size(); i++)
	{
		const DBCounterItem& CounterItem = CounterData.counterlist(i);
		CounterDataObject* pObject = DataPool::CreateObject<CounterDataObject>(ESD_COUNTER, FALSE);
		pObject->m_uRoleID = CounterItem.roleid();
		pObject->m_dwIndex = CounterItem.index();
		pObject->m_uTime = CounterItem.time();
		pObject->m_uCounterID = CounterItem.counterid();
		pObject->m_uValue = CounterItem.value();
		UINT64 uKey = pObject->m_dwIndex;
		uKey = uKey << 32 | pObject->m_uCounterID;
		m_mapCounterData.insert(std::make_pair(uKey, pObject));
	}
	return TRUE;
}

BOOL CCounterModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CCounterModule::NotifyChange()
{
	return TRUE;
}

CounterDataObject* CCounterModule::GetCounterData(UINT32 uID, UINT32 dwIndex, BOOL bCreate)
{
	ERROR_RETURN_FALSE(uID > 0);
	UINT64 uKey = dwIndex;
	uKey = uKey << 32 | uID;
	auto itor = m_mapCounterData.find(uKey);
	if (itor != m_mapCounterData.end())
	{
		return itor->second;
	}

	if (bCreate)
	{
		CounterDataObject* pTempObject = DataPool::CreateObject<CounterDataObject>(ESD_COUNTER, TRUE);
		pTempObject->m_uCounterID = uID;
		pTempObject->m_uRoleID = m_pOwnPlayer->GetRoleID();
		pTempObject->m_uValue = 0;
		pTempObject->m_dwIndex = dwIndex;
		pTempObject->m_uTime = CommonFunc::GetCurrTime();
		m_mapCounterData.insert(std::make_pair(uKey, pTempObject));
		return pTempObject;
	}

	return NULL;
}

UINT64 CCounterModule::GetCounterValue(UINT32 uID, UINT32 dwIndex /*= 0*/)
{
	ERROR_RETURN_FALSE(uID > 0);
	CounterDataObject* pTempObject = GetCounterData(uID, dwIndex);
	if (pTempObject == NULL)
	{
		return 0;
	}

	return pTempObject->m_uValue;
}

BOOL CCounterModule::SetCounterValue(UINT32 uID, INT64 uValue, UINT32 dwIndex /*= 0*/)
{
	ERROR_RETURN_FALSE(uID > 0);
	CounterDataObject* pTempObject = GetCounterData(uID, dwIndex, TRUE);
	ERROR_RETURN_FALSE(pTempObject != NULL);

	pTempObject->Lock();
	pTempObject->m_uValue = uValue;
	pTempObject->m_uTime = CommonFunc::GetCurrTime();
	pTempObject->Unlock();

	return TRUE;
}

BOOL CCounterModule::AddCounterValue(UINT32 uID, INT64 uValue, UINT32 dwIndex /*= 0*/)
{
	ERROR_RETURN_FALSE(uID > 0);
	CounterDataObject* pTempObject = GetCounterData(uID, dwIndex, TRUE);
	ERROR_RETURN_FALSE(pTempObject != NULL);

	pTempObject->Lock();
	pTempObject->m_uValue += uValue;
	pTempObject->m_uTime = CommonFunc::GetCurrTime();
	pTempObject->Unlock();

	return TRUE;
}

BOOL CCounterModule::GetCounterBitValue(UINT32 uID)
{
	ERROR_RETURN_FALSE(uID > 0); // uID必须大于0

	CounterDataObject* pCounterObject = GetCounterData(uID / 64 + 1, 0);
	if (pCounterObject == NULL)
	{
		return FALSE;
	}

	return CommonFunc::GetBitValue((UINT64)pCounterObject->m_uValue, uID % 64);
}

BOOL CCounterModule::SetCounterBitValue(UINT32 uID, BOOL bValue)
{
	ERROR_RETURN_FALSE(uID > 0); // uID必须大于0

	CounterDataObject* pCounterObject = GetCounterData(uID / 64 + 1, 0, TRUE);
	ERROR_RETURN_FALSE(pCounterObject != NULL);
	pCounterObject->Lock();
	UINT64 uValue = pCounterObject->m_uValue;
	CommonFunc::SetBitValue(uValue, uID % 64, bValue);
	pCounterObject->m_uValue = uValue;
	pCounterObject->Unlock();

	return TRUE;
}
