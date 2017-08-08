#include "stdafx.h"
#include "DataPool.h"

SharedMemory<RoleDataObject>	*g_pRoleDataObjectPool		= NULL;
SharedMemory<GlobalDataObject>	*g_pGlobalDataObjectPool	= NULL;
SharedMemory<BagDataObject>		*g_pBagDataObjectPool		= NULL;
SharedMemory<CopyDataObject>	*g_pCopyDataObjectPool		= NULL;
SharedMemory<EquipDataObject>	*g_pEquipDataObjectPool		= NULL;
SharedMemory<PetDataObject>		*g_pPetDataObjectPool		= NULL;
SharedMemory<PartnerDataObject>	*g_pPartnerDataObjectPool	= NULL;

BOOL CreateDataPool()
{
	g_pRoleDataObjectPool = new SharedMemory<RoleDataObject>("Role", 1024);
	g_pGlobalDataObjectPool  = new SharedMemory<GlobalDataObject>("Global", 1024);
	g_pBagDataObjectPool  = new SharedMemory<BagDataObject>("Bag", 1024);
	g_pCopyDataObjectPool  = new SharedMemory<CopyDataObject>("Copy", 1024);
	g_pEquipDataObjectPool  = new SharedMemory<EquipDataObject>("Equip", 1024);
	g_pPetDataObjectPool  = new SharedMemory<PetDataObject>("Pet", 1024);
	g_pPartnerDataObjectPool  = new SharedMemory<PartnerDataObject>("Partner", 1024);

	return TRUE;
}
