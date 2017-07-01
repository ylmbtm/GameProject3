#include "stdafx.h"
#include "DataPool.h"

SharedMemory<RoleDataObject>* g_pRoleDataObjectPool = NULL;

BOOL CreateDataPool()
{
	g_pRoleDataObjectPool = new SharedMemory<RoleDataObject>("Role", 1024);

	return TRUE;
}
