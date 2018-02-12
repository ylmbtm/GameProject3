#include "stdafx.h"
#include "CopyModule.h"
#include "DataPool.h"

CCopyModule::CCopyModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{

}

CCopyModule::~CCopyModule()
{

}

BOOL CCopyModule::OnCreate(UINT64 u64RoleID)
{

	return TRUE;
}


BOOL CCopyModule::OnDestroy()
{
	for(auto itor = m_mapCopyData.begin(); itor != m_mapCopyData.end(); itor++)
	{
		itor->second->release();
	}

	m_mapCopyData.clear();

	return TRUE;
}

BOOL CCopyModule::OnLogin()
{
	return TRUE;
}

BOOL CCopyModule::OnLogout()
{
	return TRUE;
}

BOOL CCopyModule::OnNewDay()
{
	return TRUE;
}

BOOL CCopyModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
	const DBCopyData& CopyData = Ack.copydata();
	for(int i = 0; i < CopyData.copylist_size(); i++)
	{
		const DBCopyItem& CopyItem = CopyData.copylist(i);

		CopyDataObject* pObject = g_pCopyDataObjectPool->NewObject(FALSE);
		pObject->m_uRoleID = CopyItem.roleid();
		pObject->m_dwCopyID = CopyItem.copyid();
		pObject->m_dwBattleCnt = CopyItem.battlecnt();
		pObject->m_uBattleTime = CopyItem.battletime();
		pObject->m_dwResetCnt = CopyItem.resetcnt();
		pObject->m_uResetTime = CopyItem.resettime();
		pObject->m_dwStarNum = CopyItem.starnum();
		m_mapCopyData.insert(std::make_pair(pObject->m_dwCopyID, pObject));
	}

	for (int i = 0; i < CopyData.chapterlist_size(); i++)
	{
		const DBChapterItem& ChapterItem = CopyData.chapterlist(i);
		ChapterDataObject* pObject = g_pChapterDataObjectPool->NewObject(FALSE);
		pObject->m_uRoleID = ChapterItem.roleid();
		pObject->m_dwCopyType = ChapterItem.copytype();
		pObject->m_dwChapter = ChapterItem.chapterid();
		pObject->m_dwSceneAward = ChapterItem.sceneaward();
		pObject->m_dwStarAward = ChapterItem.staraward();
		m_mapChapterData.insert(std::make_pair(pObject->m_dwCopyType << 16 | pObject->m_dwChapter, pObject));
	}
	return TRUE;
}

BOOL CCopyModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
	return TRUE;
}

BOOL CCopyModule::NotifyChange()
{
	return TRUE;
}

BOOL CCopyModule::CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue)
{
	return TRUE;
}

BOOL CCopyModule::DispatchPacket(NetPacket* pNetPacket)
{
	return FALSE;
}

CopyDataObject* CCopyModule::GetCopyData(UINT32 dwCopyID)
{
	auto itor = m_mapCopyData.find(dwCopyID);
	if(itor != m_mapCopyData.end())
	{
		return itor->second;
	}

	return NULL;
}

ChapterDataObject* CCopyModule::GetChapter(UINT32 dwChapter, UINT32 dwCopyType)
{
	UINT32 dwTemp = dwCopyType << 16 | dwChapter;
	auto itor = m_mapChapterData.find(dwTemp);
	if (itor != m_mapChapterData.end())
	{
		return itor->second;
	}

	return NULL;
}
