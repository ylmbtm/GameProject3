#ifndef __DATA_MODULE_POOL_H__
#define __DATA_MODULE_POOL_H__
#include "..\ServerEngine\SharedMemory.h"
#include "..\ServerData\ServerStruct.h"
#include "..\ServerData\RoleData.h"

extern	SharedMemory<RoleDataObject>* g_pRoleDataObjectPool;

BOOL CreateDataPool();

#endif //__DATA_MODULE_POOL_H__
