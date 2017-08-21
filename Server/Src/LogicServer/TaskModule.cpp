#include "stdafx.h"
#include "TaskModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

CTaskModule::CTaskModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CTaskModule::~CTaskModule()
{

}

BOOL CTaskModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CTaskModule::OnDestroy()
{
	for(auto itor = m_mapTaskData.begin(); itor != m_mapTaskData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapTaskData.clear();

	return TRUE;
}

BOOL CTaskModule::OnLogin()
{
	return TRUE;
}

BOOL CTaskModule::OnLogout()
{
	return TRUE;
}

BOOL CTaskModule::OnNewDay()
{
	return TRUE;
}

BOOL CTaskModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBTaskData& TaskData = Ack.taskdata();
	/*for(int i = 0; i < CopyData.itemlist_size(); i++)
	{
	const DBBagItemData &ItemData = BagData.itemlist(i);

	BagDataObject *pObject = g_pBagDataObjectPool->NewOjbect(FALSE);
	pObject->lock();
	pObject->m_uGuid = ItemData.guid();
	pObject->m_uRoleID = ItemData.roleid();
	pObject->m_BagType = ItemData.bagtype();
	pObject->m_bBind = ItemData.bind();
	pObject->m_ItemGuid = ItemData.itemguid();
	pObject->m_ItemID = ItemData.itemid();
	pObject->unlock();
	m_mapBagData.insert(std::make_pair(pObject->m_uGuid, pObject));
	}*/
	return TRUE;
}

BOOL CTaskModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}


BOOL CTaskModule::CalcFightValue(INT32 nValue[MAX_PROPERTY_NUM], INT32 nPercent[MAX_PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}
