#ifndef __DATA_MODULE_POOL_H__
#define __DATA_MODULE_POOL_H__
#include "..\ServerEngine\SharedMemory.h"
#include "..\ServerData\ServerStruct.h"
#include "..\ServerData\RoleData.h"
#include "..\ServerData\GlobalData.h"

extern	SharedMemory<RoleDataObject>* g_pRoleDataObjectPool;
extern	SharedMemory<GlobalDataObject>* g_pGlobalDataObjectPool;

BOOL CreateDataPool();

#endif //__DATA_MODULE_POOL_H__
