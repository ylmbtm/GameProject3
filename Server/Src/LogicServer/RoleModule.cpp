#include "stdafx.h"
#include "RoleModule.h"

CRoleModule::CRoleModule(CPlayerObject *pOwner):CModuleBase(pOwner)
{

}

CRoleModule::~CRoleModule()
{

}

BOOL CRoleModule::OnCreate(UINT64 u64RoleID)
{
	//RoleModule比较特殊，这个函数不实现


	return TRUE;
}

BOOL CRoleModule::OnCreate(UINT64 u64RoleID, std::string Name, UINT32 dwRoleType, UINT64 u64AccountID, UINT32 dwChannel)
{
	m_pRoleDataObject = g_pRoleDataObjectPool->newOjbect(TRUE);
	m_pRoleDataObject->lock();
	m_pRoleDataObject->m_u64ID = u64RoleID;
	//m_pRoleObject->m_data.m_szName = NULL;
	m_pRoleDataObject->m_dwLangID = 1;
	m_pRoleDataObject->m_RoleType = 1;
	m_pRoleDataObject->unlock();
	return TRUE;
}

BOOL CRoleModule::OnDestroy(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CRoleModule::OnLogin(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CRoleModule::OnLogout(UINT64 u64RoleID)
{
	return TRUE;
}

BOOL CRoleModule::OnNewDay()
{
	return TRUE;
}

BOOL CRoleModule::OnLoadData(UINT64 u64RoleID)
{
	//RoleModule比较特殊，这个函数不实现

	return TRUE;
}
