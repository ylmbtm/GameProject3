#include "stdafx.h"
#include "CopyModule.h"
#include "DataPool.h"
#include "../Message/Msg_Copy.pb.h"
#include "BagModule.h"
#include "RoleModule.h"
#include "ModuleBase.h"
#include "PlayerObject.h"
#include "StaticData.h"
#include "StaticStruct.h"
#include "../Message/Msg_ID.pb.h"

CCopyModule::CCopyModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
	RegisterMessageHanler();
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
		itor->second->Release();
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

		CopyDataObject* pObject = DataPool::CreateObject<CopyDataObject>(ESD_COPY, FALSE);
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
		ChapterDataObject* pObject = DataPool::CreateObject<ChapterDataObject>(ESD_CHAPTER, FALSE);
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
// 	for (auto itor = m_mapCopyData.begin(); itor != m_mapCopyData.end(); itor++)
// 	{
// 		CopyDataObject* pObject = itor->second;
//
// 		CopyItem* pItem = Ack.add_copylist();
// 		pItem->set_copyid(pObject->m_dwCopyID);
// 		pItem->set_battlecnt(pObject->m_dwBattleCnt);
// 		pItem->set_resetcnt(pObject->m_dwResetCnt);
// 		pItem->set_starnum(pObject->m_dwStarNum);
// 	}
//
// 	for (auto itor = m_mapChapterData.begin(); itor != m_mapChapterData.end(); itor++)
// 	{
// 		ChapterDataObject* pObject = itor->second;
//
// 		ChapterItem* pItem = Ack.add_chapterlist();
// 		pItem->set_chapterid(pObject->m_dwChapter);
// 		pItem->set_copytype(pObject->m_dwCopyType);
// 		pItem->set_sceneaward(pObject->m_dwSceneAward);
// 		pItem->set_staraward(pObject->m_dwStarAward);
// 	}

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

VOID CCopyModule::RegisterMessageHanler()
{
}

BOOL CCopyModule::OnMainCopyResult(BattleResultNty* pNty, INT32 nIndex)
{
	const ResultPlayer& pResultPlayer = pNty->playerlist(nIndex);

	MainCopyResultNty ResultNty;
	ResultNty.set_roleid(m_pOwnPlayer->GetRoleID());
	ResultNty.set_copyresult(pResultPlayer.result());
	ResultNty.set_lasttime(pNty->lasttime());
	ResultNty.set_starnum(2);
	ResultNty.set_copyid(pNty->copyid());

	CBagModule* pBagModule = (CBagModule*)m_pOwnPlayer->GetModuleByType(MT_BAG);
	ERROR_RETURN_TRUE(pBagModule != NULL);

	CRoleModule* pRoleModule = (CRoleModule*)m_pOwnPlayer->GetModuleByType(MT_ROLE);
	ERROR_RETURN_TRUE(pRoleModule != NULL);

	StCopyInfo* pCopyInfo = CStaticData::GetInstancePtr()->GetCopyInfo(pNty->copyid());
	ERROR_RETURN_TRUE(pCopyInfo != NULL);

	std::vector<StItemData> vtItemList;
	CStaticData::GetInstancePtr()->GetItemsFromAwardID(pCopyInfo->dwAwardID, pRoleModule->m_pRoleDataObject->m_CarrerID, vtItemList);

	for (std::vector<StItemData>::size_type i = 0; i < vtItemList.size(); i++)
	{
		pBagModule->AddItem(vtItemList[i].dwItemID, vtItemList[i].dwItemNum);
		ItemData* pItemData = ResultNty.add_itemlist();
		pItemData->set_itemid(vtItemList[i].dwItemID);
		pItemData->set_itemnum(vtItemList[i].dwItemNum);
	}

	pRoleModule->AddExp(pCopyInfo->dwGetExpRation * pRoleModule->m_pRoleDataObject->m_Level);

	pRoleModule->CostAction(pCopyInfo->dwCostActID, pCopyInfo->dwCostActNum);

	m_pOwnPlayer->SendMsgProtoBuf(MSG_MAINCOPY_RESULT_NTY, ResultNty);

	m_pOwnPlayer->ClearCopyStatus();

	return TRUE;
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
