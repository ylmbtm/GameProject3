#ifndef __DATA_MODULE_POOL_H__
#define __DATA_MODULE_POOL_H__

#include "SharedMemory.h"

#include "../ServerData/RoleData.h"
#include "../ServerData/GlobalData.h"
#include "../ServerData/BagData.h"
#include "../ServerData/CopyData.h"
#include "../ServerData/EquipData.h"
#include "../ServerData/PetData.h"
#include "../ServerData/PartnerData.h"
#include "../ServerData/GuildData.h"
#include "../ServerData/TaskData.h"
#include "../ServerData/MountData.h"
#include "../ServerData/MailData.h"
#include "../ServerData/ActivityData.h"
#include "../ServerData/CounterData.h"
#include "../ServerData/FriendData.h"
#include "../ServerData/GemData.h"

extern	SharedMemory<RoleDataObject>*		g_pRoleDataObjectPool;
extern	SharedMemory<GlobalDataObject>*		g_pGlobalDataObjectPool;
extern	SharedMemory<BagDataObject>*		g_pBagDataObjectPool;
extern	SharedMemory<CopyDataObject>*		g_pCopyDataObjectPool;
extern	SharedMemory<EquipDataObject>*		g_pEquipDataObjectPool;
extern	SharedMemory<GemDataObject>*		g_pGemDataObjectPool;
extern	SharedMemory<PetDataObject>*		g_pPetDataObjectPool;
extern	SharedMemory<PartnerDataObject>*	g_pPartnerDataObjectPool;
extern	SharedMemory<GuildDataObject>*		g_pGuildDataObjectPool;
extern	SharedMemory<MemberDataObject>*		g_pMemberDataObjectPool;
extern	SharedMemory<TaskDataObject>*		g_pTaskDataObjectPool;
extern	SharedMemory<MountDataObject>*		g_pMountDataObjectPool;
extern	SharedMemory<MailDataObject>*		g_pMailDataObjectPool;
extern	SharedMemory<GroupMailDataObject>*	g_pGroupMailDataObjectPool;
extern	SharedMemory<ActivityDataObject>*	g_pActivityDataObjectPool;
extern	SharedMemory<CounterDataObject>*	g_pCounterDataObjectPool;
extern	SharedMemory<FriendDataObject>*		g_pFriendDataObjectPool;
std::string GenDataName(std::string strName);

BOOL CreateDataPool();
BOOL ReleaseDataPool();

#endif //__DATA_MODULE_POOL_H__
