#include "stdafx.h"
#include "DataPool.h"
#include "GameService.h"
#include "RoleModule.h"

CDataPool::CDataPool()
{
    m_dwSharePageSize = CConfigFile::GetInstancePtr()->GetIntValue("share_page_size");
    if (m_dwSharePageSize <= 1)
    {
        m_dwSharePageSize = 1024;
    }
}

CDataPool::~CDataPool()
{

}

CDataPool* CDataPool::GetInstancePtr()
{
    static CDataPool _StaticMgr;

    return &_StaticMgr;
}

BOOL CDataPool::InitDataPool()
{
    INT32 nAreaID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
    ERROR_RETURN_FALSE(nAreaID > 0);

    m_vtDataObjectPools.assign(ESD_END, NULL);
    m_vtDataObjectPools[ESD_ROLE]           = new SharedMemory<RoleDataObject>(ESD_ROLE, 1024);
    m_vtDataObjectPools[ESD_GLOBAL]         = new SharedMemory<GlobalDataObject>(ESD_GLOBAL, 1024);
    m_vtDataObjectPools[ESD_BAG]            = new SharedMemory<BagDataObject>(ESD_BAG, 1024);
    m_vtDataObjectPools[ESD_COPY]           = new SharedMemory<CopyDataObject>(ESD_COPY, 1024);
    m_vtDataObjectPools[ESD_CHAPTER]        = new SharedMemory<ChapterDataObject>(ESD_CHAPTER, 1024);
    m_vtDataObjectPools[ESD_EQUIP]          = new SharedMemory<EquipDataObject>(ESD_EQUIP, 1024);
    m_vtDataObjectPools[ESD_GEM]            = new SharedMemory<GemDataObject>(ESD_GEM, 1024);
    m_vtDataObjectPools[ESD_PET]            = new SharedMemory<PetDataObject>(ESD_PET, 1024);
    m_vtDataObjectPools[ESD_PARTNER]        = new SharedMemory<PartnerDataObject>(ESD_PARTNER, 1024);
    m_vtDataObjectPools[ESD_GUILD]          = new SharedMemory<GuildDataObject>(ESD_GUILD, 1024);
    m_vtDataObjectPools[ESD_GUILD_MEMBER]   = new SharedMemory<MemberDataObject>(ESD_GUILD_MEMBER, 1024);
    m_vtDataObjectPools[ESD_TASK]           = new SharedMemory<TaskDataObject>(ESD_TASK, 1024);
    m_vtDataObjectPools[ESD_MOUNT]          = new SharedMemory<MountDataObject>(ESD_MOUNT, 1024);
    m_vtDataObjectPools[ESD_MAIL]           = new SharedMemory<MailDataObject>(ESD_MAIL, 1024);
    m_vtDataObjectPools[ESD_OFFDATA]        = new SharedMemory<OffDataObject>(ESD_OFFDATA, 1024);
    m_vtDataObjectPools[ESD_GROUP_MAIL]     = new SharedMemory<GroupMailDataObject>(ESD_GROUP_MAIL, 1024);
    m_vtDataObjectPools[ESD_ACTIVITY]       = new SharedMemory<ActivityDataObject>(ESD_ACTIVITY, 1024);
    m_vtDataObjectPools[ESD_COUNTER]        = new SharedMemory<CounterDataObject>(ESD_COUNTER, 1024);
    m_vtDataObjectPools[ESD_FRIEND]         = new SharedMemory<FriendDataObject>(ESD_FRIEND, 1024);
    m_vtDataObjectPools[ESD_SKILL]          = new SharedMemory<SkillDataObject>(ESD_SKILL, 1024);
    m_vtDataObjectPools[ESD_PAYMENT]        = new SharedMemory<PayDataObject>(ESD_PAYMENT, 1024);
    m_vtDataObjectPools[ESD_SEAL_ROLE]      = new SharedMemory<SealDataObject>(ESD_SEAL_ROLE, 1024);

    for (int i = ESD_BEGIN + 1; i < ESD_END; i++)
    {
        ERROR_RETURN_FALSE(m_vtDataObjectPools[i] != NULL);

        m_vtDataObjectPools[i]->InitToMap();
    }

    return TRUE;
}

BOOL CDataPool::ReleaseDataPool()
{
    for (int i = ESD_BEGIN + 1; i < ESD_END; i++)
    {
        SharedMemoryBase* pShareBase = m_vtDataObjectPools.at(i);
        delete pShareBase;
    }

    m_vtDataObjectPools.clear();

    return TRUE;
}

BOOL CDataPool::RestoreFromShareMemory()
{
//    for (int i = ESD_ROLE; i < ESD_END; i++)
//    {
//      SharedMemoryBase *pShareBase = m_vtDataObjectPools.at(i);
//      std::map<void*, _SMBlock*>& usedMap = pShareBase->GetUsedDataList();
//      for (auto itor = usedMap.begin(); itor != usedMap.end(); itor++)
//      {
//          RoleDataObject *pObject = static_cast<RoleDataObject*>(itor->first);
//          CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->CreatePlayer(pObject->m_uRoleID);
//          pPlayer->Init(pObject->m_uRoleID);
//          CRoleModule *pRoleModule = (CRoleModule *)pPlayer->GetModuleByType(MT_ROLE);
//          pRoleModule->RestoreData(pObject);
//      }
//    }


    return TRUE;
}

SharedMemoryBase* CDataPool::GetSharePool(EShareData nIndex)
{
    ERROR_RETURN_NULL((nIndex > ESD_BEGIN && nIndex < ESD_END));

    return m_vtDataObjectPools[nIndex];
}
