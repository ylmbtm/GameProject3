#include "stdafx.h"
#include "DataPool.h"

SharedMemory<RoleDataObject>* g_pRoleDataObjectPool = NULL;
SharedMemory<GlobalDataObject>* g_pGlobalDataObjectPool = NULL;

BOOL CreateDataPool()
{
	g_pRoleDataObjectPool = new SharedMemory<RoleDataObject>("Role", 1024);
	g_pGlobalDataObjectPool  = new SharedMemory<GlobalDataObject>("Global", 1024);

	return TRUE;
}
