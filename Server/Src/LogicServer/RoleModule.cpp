#include "stdafx.h"
#include "RoleModule.h"
#include "Utility/Log/Log.h"

CRoleModule::CRoleModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CRoleModule::~CRoleModule()
{

}

BOOL CRoleModule::OnCreate(UINT64 u64RoleID)
{
    ERROR_RETURN_FALSE(m_pRoleDataObject != NULL);
    m_pRoleDataObject->lock();
    m_pRoleDataObject->m_dwLevel = 1;
    m_pRoleDataObject->unlock();

	return TRUE;
}

BOOL CRoleModule::InitBaseData( UINT64 u64RoleID, std::string Name, UINT32 dwActorID, UINT64 u64AccountID, UINT32 dwChannel )
{
	m_pRoleDataObject = g_pRoleDataObjectPool->newOjbect(TRUE);
	m_pRoleDataObject->lock();
	m_pRoleDataObject->m_u64ID = u64RoleID;
	m_pRoleDataObject->m_u64AccountID = u64AccountID;
	strcpy_s(m_pRoleDataObject->m_szName, 255, Name.c_str());
	m_pRoleDataObject->m_dwLangID = 0;
	m_pRoleDataObject->m_dwActorID = dwActorID;
	m_pRoleDataObject->unlock();
	return TRUE;
}

BOOL CRoleModule::OnDestroy()
{
	m_pRoleDataObject->release();

	m_pRoleDataObject = NULL;

	return TRUE;
}

BOOL CRoleModule::OnLogin()
{
	return TRUE;
}

BOOL CRoleModule::OnLogout()
{
	return TRUE;
}

BOOL CRoleModule::OnNewDay()
{
	return TRUE;
}

BOOL CRoleModule::OnLoadData(UINT64 u64RoleID)
{
	SetDataOK(TRUE);
	return TRUE;
}

BOOL CRoleModule::ReadFromLoginAck( DBRoleLoginAck &Ack )
{
    m_pRoleDataObject = g_pRoleDataObjectPool->newOjbect(FALSE);
    m_pRoleDataObject->lock();
    m_pRoleDataObject->m_u64ID = Ack.roleid();
    m_pRoleDataObject->m_u64AccountID = Ack.accountid();
    strcpy_s(m_pRoleDataObject->m_szName, 255, Ack.name().c_str());
    m_pRoleDataObject->m_dwLangID = Ack.langid();
    m_pRoleDataObject->m_dwActorID = Ack.actorid();
    m_pRoleDataObject->m_dwLevel = Ack.level();
    m_pRoleDataObject->m_dwExp = Ack.exp();
    m_pRoleDataObject->m_dwVipLvl = Ack.viplvl();
    m_pRoleDataObject->m_dwVipExp = Ack.vipexp();
    m_pRoleDataObject->m_dwLangID = Ack.langid();
    m_pRoleDataObject->unlock();


    return TRUE;
}
