#ifndef __DATA_MODULE_POOL_H__
#define __DATA_MODULE_POOL_H__


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
#include "../ServerData/SkillData.h"

namespace DataPool
{
	template <class T>
	T* CreateObject(EShareData nIndex, BOOL bNewBlock = TRUE)
	{
		SharedMemoryBase *pShareBase = CDataPool::GetInstancePtr()->GetSharePool(nIndex);

		T* pTmp = static_cast<T*>(pShareBase->NewObject(bNewBlock));

		return pTmp;
	}

}

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











#endif //__DATA_MODULE_POOL_H__
