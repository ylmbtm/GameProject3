#include "stdafx.h"
#include "ActivityModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../Message/Msg_LoginDBData.pb.h"
#include "../ConfigData/ConfigStruct.h"
#include "../ConfigData/ConfigData.h"

CActivityModule::CActivityModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CActivityModule::~CActivityModule()
{

}

BOOL CActivityModule::OnCreate(UINT64 u64RoleID)
{
	std::map <UINT32, StActivityInfo>& mapActivityList = CConfigData::GetInstancePtr()->m_mapActivityInfo;
	for(auto itor = mapActivityList.begin(); itor != mapActivityList.end(); itor ++)
	{
		StActivityInfo& tempInfo = itor->second;
		if(tempInfo.ActivityID)
		{

		}

		if(tempInfo.ActivityType == ACT_LOGINAWARD)
		{
			ActivityDataObject* pTempObject = g_pActivityDataObjectPool->NewObject(TRUE);
			pTempObject->m_dwActivityID = tempInfo.ActivityID;
			pTempObject->m_dwActivityType = ACT_LOGINAWARD;
		}

	}


	return TRUE;
}


BOOL CActivityModule::OnDestroy()
{
	for(auto itor = m_mapActivityData.begin(); itor != m_mapActivityData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapActivityData.clear();

	return TRUE;
}

BOOL CActivityModule::OnLogin()
{
	std::map <UINT32, StActivityInfo>& mapActivityList = CConfigData::GetInstancePtr()->m_mapActivityInfo;
	for(auto itor = mapActivityList.begin(); itor != mapActivityList.end(); itor ++)
	{
		StActivityInfo& tempInfo = itor->second;
		if(tempInfo.ActivityID)
		{

		}

		if(tempInfo.ActivityType == ACT_LOGINAWARD)
		{
			ActivityDataObject* pTempObject = g_pActivityDataObjectPool->NewObject(TRUE);
			pTempObject->m_dwActivityID = tempInfo.ActivityID;
			pTempObject->m_dwActivityType = ACT_LOGINAWARD;
		}

	}

	return TRUE;
}

BOOL CActivityModule::OnLogout()
{
	return TRUE;
}

BOOL CActivityModule::OnNewDay()
{
	return TRUE;
}

BOOL CActivityModule::DispatchPacket(NetPacket* pNetPacket)
{
	return FALSE;
}

BOOL CActivityModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBActivityData& ActivityData = Ack.activitydata();
	for(int i = 0; i < ActivityData.activitylist_size(); i++)
	{
		const DBActivityItem& ActivityItem = ActivityData.activitylist(i);
		ActivityDataObject* pObject = g_pActivityDataObjectPool->NewObject(FALSE);
		pObject->m_dwActivityID = ActivityItem.activityid();
		pObject->m_uRoleID = ActivityItem.roleid();
		m_mapActivityData.insert(std::make_pair(pObject->m_dwActivityID, pObject));
	}
	return TRUE;
}

BOOL CActivityModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CActivityModule::NotifyChange()
{
	return TRUE;
}
