#include "stdafx.h"
#include "ActivityModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../Message/Msg_LoginDBData.pb.h"
#include "StaticStruct.h"
#include "StaticData.h"

CActivityModule::CActivityModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	RegisterMessageHanler();
}

CActivityModule::~CActivityModule()
{

}

BOOL CActivityModule::OnCreate(UINT64 u64RoleID)
{
	std::map <UINT32, StActivityInfo>& mapActivityList = CStaticData::GetInstancePtr()->m_mapActivityInfo;
	for(auto itor = mapActivityList.begin(); itor != mapActivityList.end(); itor ++)
	{
		StActivityInfo& tempInfo = itor->second;
		if(tempInfo.ActivityID)
		{

		}

		if(tempInfo.ActivityType == ACT_LOGINAWARD)
		{
			ActivityDataObject* pTempObject = DataPool::CreateObject<ActivityDataObject>(ESD_ACTIVITY);
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
		itor->second->Release();
	}

	m_mapActivityData.clear();

	return TRUE;
}

BOOL CActivityModule::OnLogin()
{
	std::map <UINT32, StActivityInfo>& mapActivityList = CStaticData::GetInstancePtr()->m_mapActivityInfo;
	for(auto itor = mapActivityList.begin(); itor != mapActivityList.end(); itor ++)
	{
		StActivityInfo& tempInfo = itor->second;
		if(tempInfo.ActivityID)
		{

		}

		if(tempInfo.ActivityType == ACT_LOGINAWARD)
		{
			ActivityDataObject* pTempObject = DataPool::CreateObject<ActivityDataObject>(ESD_ACTIVITY);
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

VOID CActivityModule::RegisterMessageHanler()
{
}

BOOL CActivityModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBActivityData& ActivityData = Ack.activitydata();
	for(int i = 0; i < ActivityData.activitylist_size(); i++)
	{
		const DBActivityItem& ActivityItem = ActivityData.activitylist(i);
		ActivityDataObject* pObject = DataPool::CreateObject<ActivityDataObject>(ESD_ACTIVITY, FALSE);
		pObject->m_dwActivityID = ActivityItem.activityid();
		pObject->m_uRoleID = ActivityItem.roleid();
		pObject->m_dwActivityType = ActivityItem.activitytype();
		pObject->m_dwDataLen = ActivityItem.datalen();
		pObject->m_uJoinTime = ActivityItem.jointime();
		memcpy(pObject->m_Data.m_Bytes, (void*)ActivityItem.data().c_str(), pObject->m_dwDataLen);
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

BOOL CActivityModule::GetRedPoint()
{
	for (auto itor = m_mapActivityData.begin(); itor != m_mapActivityData.end(); itor++)
	{
		ActivityDataObject* pDataObject = itor->second;

	}

	return TRUE;
}
