#include "stdafx.h"
#include "ActivityModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../Message/Msg_LoginDBData.pb.h"

CActivityModule::CActivityModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CActivityModule::~CActivityModule()
{

}

BOOL CActivityModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CActivityModule::OnDestroy()
{


	return TRUE;
}

BOOL CActivityModule::OnLogin()
{
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

BOOL CActivityModule::DispatchPacket(NetPacket* pNetPack)
{
	return TRUE;
}

BOOL CActivityModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBActivityData& ActivityData = Ack.activitydata();
	for(int i = 0; i < ActivityData.activitylist_size(); i++)
	{
	    const DBActivityItem &ActivityItem = ActivityData.activitylist(i);
	    ActivityDataObject *pObject = g_pActivityDataObjectPool->NewOjbect(FALSE);
	    pObject->lock();
	    pObject->m_dwActivityID = ActivityItem.activityid();
	    pObject->m_uRoleID = ActivityItem.roleid();
	    pObject->unlock();
	    m_mapActivityData.insert(std::make_pair(pObject->m_dwActivityID, pObject));
	}
	return TRUE;
}

BOOL CActivityModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}
