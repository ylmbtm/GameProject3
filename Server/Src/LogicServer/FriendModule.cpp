#include "stdafx.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "FriendModule.h"

CFriendModule::CFriendModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CFriendModule::~CFriendModule()
{

}

BOOL CFriendModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CFriendModule::OnDestroy()
{
	for(auto itor = m_mapFriendData.begin(); itor != m_mapFriendData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapFriendData.clear();

	return TRUE;
}

BOOL CFriendModule::OnLogin()
{
	return TRUE;
}

BOOL CFriendModule::OnLogout()
{
	return TRUE;
}

BOOL CFriendModule::OnNewDay()
{
	return TRUE;
}

BOOL CFriendModule::DispatchPacket(NetPacket* pNetPack)
{
	return TRUE;
}

BOOL CFriendModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBFriendData& FriendData = Ack.frienddata();
	for(int i = 0; i < FriendData.friendlist_size(); i++)
	{
		const DBFriendItem& FriendItem = FriendData.friendlist(i);
		FriendDataObject* pObject = g_pFriendDataObjectPool->NewObject(FALSE);


		m_mapFriendData.insert(std::make_pair(pObject->m_uFriendID, pObject));
	}
	return TRUE;
}

BOOL CFriendModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

FriendDataObject* CFriendModule::GetFriendData(UINT64 uRoleID)
{
	auto itor = m_mapFriendData.find(uRoleID);
	if(itor == m_mapFriendData.end())
	{
		return NULL;
	}

	return itor->second;
}
