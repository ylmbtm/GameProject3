#include "stdafx.h"
#include "CopyModule.h"
#include "SharedMemory.h"
#include "DataPool.h"

CCopyModule::CCopyModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CCopyModule::~CCopyModule()
{

}

BOOL CCopyModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CCopyModule::OnDestroy()
{
	for(auto itor = m_mapCopyData.begin(); itor != m_mapCopyData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapCopyData.clear();

	return TRUE;
}

BOOL CCopyModule::OnLogin()
{
	return TRUE;
}

BOOL CCopyModule::OnLogout()
{
	return TRUE;
}

BOOL CCopyModule::OnNewDay()
{
	return TRUE;
}

BOOL CCopyModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBCopyData& CopyData = Ack.copydata();
	for(int i = 0; i < CopyData.copylist_size(); i++)
	{
		const DBCopyItem& CopyItem = CopyData.copylist(i);

		CopyDataObject* pObject = g_pCopyDataObjectPool->NewObject(FALSE);
		pObject->m_uRoleID = CopyItem.roleid();
		pObject->m_dwCopyID = CopyItem.copyid();
		m_mapCopyData.insert(std::make_pair(pObject->m_dwCopyID, pObject));
	}
	return TRUE;
}

BOOL CCopyModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CCopyModule::NotifyChange()
{
	return TRUE;
}

BOOL CCopyModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

CopyDataObject* CCopyModule::GetCopyData(UINT32 dwCopyID)
{
	auto itor = m_mapCopyData.find(dwCopyID);
	if(itor != m_mapCopyData.end())
	{
		return itor->second;
	}

	return NULL;
}
