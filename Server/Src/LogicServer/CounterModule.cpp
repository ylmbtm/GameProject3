#include "stdafx.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "CounterModule.h"

CCounterModule::CCounterModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

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

BOOL CCounterModule::DispatchPacket(NetPacket* pNetPack)
{
	return TRUE;
}

BOOL CCounterModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBCounterData& CounterData = Ack.counterdata();
	for(int i = 0; i < CounterData.counterlist_size(); i++)
	{
		const DBCounterItem &CounterItem = CounterData.counterlist(i);
		CounterDataObject *pObject = g_pCounterDataObjectPool->NewOjbect(FALSE);
		

		//m_mapCounterData.insert(std::make_pair(pObject->m_uGuid, pObject));
	}
	return TRUE;
}

BOOL CCounterModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

CounterDataObject* CCounterModule::GetCounterData(UINT64 uID, UINT32 dwIndex, BOOL bCreate)
{
	auto itor = m_mapCounterData.find(uID);
	if(itor == m_mapCounterData.end())
	{
		if(bCreate)
		{
			CounterDataObject* pTempObject = g_pCounterDataObjectPool->NewOjbect(TRUE);
			std::vector<CounterDataObject*> tempvt;
			tempvt.push_back(pTempObject);
			m_mapCounterData.insert(std::make_pair(uID, tempvt));
			return pTempObject;
		}

		return NULL;
	}

	if(itor->second.size() > 0)
	{
		if(dwIndex == 0)
		{
			return itor->second.at(0);
		}

		std::vector<CounterDataObject*>& vtData = itor->second;

		for(std::vector<CounterDataObject*>::size_type i = 0; i < vtData.size(); i++)
		{
			if(vtData.at(i)->m_dwIndex == dwIndex)
			{
				return vtData.at(i);
			}
		}
	}

	if(bCreate)
	{
		CounterDataObject* pTempObject = g_pCounterDataObjectPool->NewOjbect(TRUE);
		std::vector<CounterDataObject*> tempvt;
		itor->second.push_back(pTempObject);
		return pTempObject;
	}

	return NULL;
}
