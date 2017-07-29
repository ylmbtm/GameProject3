#ifndef __DATA_MODULE_POOL_H__
#define __DATA_MODULE_POOL_H__
#include "..\ServerEngine\SharedMemory.h"
#include "..\ServerData\ServerStruct.h"
#include "..\ServerData\RoleData.h"
#include "..\ServerData\GlobalData.h"
#include "..\ServerData\BagData.h"
#include "..\ServerData\CopyData.h"

extern	SharedMemory<RoleDataObject>	*g_pRoleDataObjectPool;
extern	SharedMemory<GlobalDataObject>	*g_pGlobalDataObjectPool;
extern	SharedMemory<BagDataObject>		*g_pBagDataObjectPool;
extern	SharedMemory<CopyDataObject>	*g_pCopyDataObjectPool;

BOOL CreateDataPool();

#endif //__DATA_MODULE_POOL_H__
