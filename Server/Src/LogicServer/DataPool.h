#ifndef __DATA_MODULE_POOL_H__
#define __DATA_MODULE_POOL_H__


#include "RoleData.h"
#include "GlobalData.h"
#include "BagData.h"
#include "CopyData.h"
#include "EquipData.h"
#include "PetData.h"
#include "PartnerData.h"
#include "GuildData.h"
#include "TaskData.h"
#include "MountData.h"
#include "MailData.h"
#include "ActivityData.h"
#include "CounterData.h"
#include "FriendData.h"
#include "GemData.h"
#include "SkillData.h"

class CDataPool
{
	CDataPool();
	~CDataPool();

public:
	static CDataPool* GetInstancePtr();

	BOOL InitDataPool();

	BOOL ReleaseDataPool();

	BOOL RestoreFromShareMemory();

	SharedMemoryBase* GetSharePool(EShareData nIndex);

protected:
	std::vector<SharedMemoryBase*>		m_vtDataObjectPools;
};

namespace DataPool
{
template <class T>
T* CreateObject(EShareData nIndex, BOOL bNewBlock = TRUE)
{
	SharedMemoryBase* pShareBase = CDataPool::GetInstancePtr()->GetSharePool(nIndex);

	T* pTmp = static_cast<T*>(pShareBase->NewObject(bNewBlock));

	return pTmp;
}
}



#endif //__DATA_MODULE_POOL_H__
