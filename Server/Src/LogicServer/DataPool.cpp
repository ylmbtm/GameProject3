#include "stdafx.h"
#include "DataPool.h"
#include "GameService.h"

SharedMemory<RoleDataObject>*		g_pRoleDataObjectPool		= NULL;
SharedMemory<GlobalDataObject>*		g_pGlobalDataObjectPool		= NULL;
SharedMemory<BagDataObject>*		g_pBagDataObjectPool		= NULL;
SharedMemory<CopyDataObject>*		g_pCopyDataObjectPool		= NULL;
SharedMemory<ChapterDataObject>*	g_pChapterDataObjectPool	= NULL;
SharedMemory<EquipDataObject>*		g_pEquipDataObjectPool		= NULL;
SharedMemory<GemDataObject>*		g_pGemDataObjectPool		= NULL;
SharedMemory<PetDataObject>*		g_pPetDataObjectPool		= NULL;
SharedMemory<PartnerDataObject>*	g_pPartnerDataObjectPool	= NULL;
SharedMemory<GuildDataObject>*		g_pGuildDataObjectPool		= NULL;
SharedMemory<MemberDataObject>*		g_pMemberDataObjectPool		= NULL;
SharedMemory<TaskDataObject>*		g_pTaskDataObjectPool		= NULL;
SharedMemory<MountDataObject>*		g_pMountDataObjectPool		= NULL;
SharedMemory<MailDataObject>*		g_pMailDataObjectPool		= NULL;
SharedMemory<GroupMailDataObject>*	g_pGroupMailDataObjectPool	= NULL;
SharedMemory<ActivityDataObject>*	g_pActivityDataObjectPool	= NULL;
SharedMemory<CounterDataObject>*	g_pCounterDataObjectPool	= NULL;
SharedMemory<FriendDataObject>*		g_pFriendDataObjectPool		= NULL;
SharedMemory<SkillDataObject>*		g_pSkillDataObjectPool		= NULL;

BOOL CreateDataPool()
{
	g_pRoleDataObjectPool		= new SharedMemory<RoleDataObject>(ESD_ROLE, 1024);
	g_pGlobalDataObjectPool		= new SharedMemory<GlobalDataObject>(ESD_GLOBAL, 1024);
	g_pBagDataObjectPool		= new SharedMemory<BagDataObject>(ESD_BAG, 1024);
	g_pCopyDataObjectPool		= new SharedMemory<CopyDataObject>(ESD_COPY, 1024);
	g_pChapterDataObjectPool	= new SharedMemory<ChapterDataObject>(ESD_CHAPTER, 1024);
	g_pEquipDataObjectPool		= new SharedMemory<EquipDataObject>(ESD_EQUIP, 1024);
	g_pGemDataObjectPool		= new SharedMemory<GemDataObject>(ESD_GEM, 1024);
	g_pPetDataObjectPool		= new SharedMemory<PetDataObject>(ESD_PET, 1024);
	g_pPartnerDataObjectPool	= new SharedMemory<PartnerDataObject>(ESD_PARTNER, 1024);
	g_pGuildDataObjectPool		= new SharedMemory<GuildDataObject>(ESD_GUILD, 1024);
	g_pMemberDataObjectPool		= new SharedMemory<MemberDataObject>(ESD_GUILD_MEMBER, 1024);
	g_pTaskDataObjectPool		= new SharedMemory<TaskDataObject>(ESD_TASK, 1024);
	g_pMountDataObjectPool		= new SharedMemory<MountDataObject>(ESD_MOUNT, 1024);
	g_pMailDataObjectPool		= new SharedMemory<MailDataObject>(ESD_MAIL, 1024);
	g_pGroupMailDataObjectPool  = new SharedMemory<GroupMailDataObject>(ESD_GROUP_MAIL, 1024);
	g_pActivityDataObjectPool	= new SharedMemory<ActivityDataObject>(ESD_ACTIVITY, 1024);
	g_pCounterDataObjectPool    = new SharedMemory<CounterDataObject>(ESD_COUNTER, 1024);
	g_pFriendDataObjectPool		= new SharedMemory<FriendDataObject>(ESD_FRIEND, 1024);
	g_pSkillDataObjectPool		= new SharedMemory<SkillDataObject>(ESD_SKILL, 1024);

	g_pRoleDataObjectPool->InitToMap();
	g_pGlobalDataObjectPool->InitToMap();
	g_pBagDataObjectPool->InitToMap();
	g_pCopyDataObjectPool->InitToMap();
	g_pChapterDataObjectPool->InitToMap();
	g_pEquipDataObjectPool->InitToMap();
	g_pPetDataObjectPool->InitToMap();
	g_pPartnerDataObjectPool->InitToMap();
	g_pGuildDataObjectPool->InitToMap();
	g_pMemberDataObjectPool->InitToMap();
	g_pTaskDataObjectPool->InitToMap();
	g_pMountDataObjectPool->InitToMap();
	g_pMailDataObjectPool->InitToMap();
	g_pGroupMailDataObjectPool->InitToMap();
	g_pActivityDataObjectPool->InitToMap();
	g_pCounterDataObjectPool->InitToMap();
	g_pFriendDataObjectPool->InitToMap();
	g_pSkillDataObjectPool->InitToMap();

	





	return TRUE;
}

BOOL ReleaseDataPool()
{
	delete g_pRoleDataObjectPool;
	delete g_pGlobalDataObjectPool;
	delete g_pBagDataObjectPool;
	delete g_pCopyDataObjectPool;
	delete g_pChapterDataObjectPool;
	delete g_pEquipDataObjectPool;
	delete g_pPetDataObjectPool;
	delete g_pPartnerDataObjectPool;
	delete g_pGuildDataObjectPool;
	delete g_pMemberDataObjectPool;
	delete g_pTaskDataObjectPool;
	delete g_pMountDataObjectPool;
	delete g_pMailDataObjectPool;
	delete g_pGroupMailDataObjectPool;
	delete g_pActivityDataObjectPool;
	delete g_pCounterDataObjectPool;
	delete g_pFriendDataObjectPool;
	return TRUE;
}

BOOL RestoreFromShareMemory()
{
// 	mapUsedSMBlock& usedMap =  g_pRoleDataObjectPool->GetUsedDataList();
// 
// 	for (auto itor = usedMap.begin(); itor != usedMap.end(); itor++)
// 	{
// 		RoleDataObject *pObject = itor->first;
// 		CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->CreatePlayer(pObject->m_uRoleID);
// 		pPlayer->Init(pObject->m_uRoleID);
// 		CRoleModule *pRoleModule = (CRoleModule *)pPlayer->GetModuleByType(MT_ROLE);
// 		pRoleModule->RestoreData(pObject);
// 	}
// 
// 	mapUsedSMBlock& usedMap2 = g_pBagDataObjectPool->GetUsedDataList();
// 	for (auto itor = usedMap2.begin(); itor != usedMap2.end(); itor++)
// 	{
// 		BagDataObject *pObject = itor->first;
// 		CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pObject->m_uRoleID);
// 		CBagModule *pBagModule = (CBagModule*)pPlayer->GetModuleByType(MT_BAG);
// 		pBagModule->RestoreData(pObject);
// 	}

	return TRUE;
}

std::string GenDataName(std::string strName)
{
	return strName + "_" + CommonConvert::IntToString(CGameService::GetInstancePtr()->GetServerID());
}
