#include "stdafx.h"
#include "ActivityModule.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"

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

BOOL CActivityModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBPetData& PetData = Ack.petdata();
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

BOOL CActivityModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CActivityModule::CalcFightValue(INT32 nValue[MAX_PROPERTY_NUM], INT32 nPercent[MAX_PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}
