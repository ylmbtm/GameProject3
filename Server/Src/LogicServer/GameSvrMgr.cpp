#include "stdafx.h"
#include "GameSvrMgr.h"
#include "CommandDef.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "Log.h"
#include "../Message/Game_Define.pb.h"
#include "BagModule.h"
#include "RoleModule.h"
#include "../ServerData/ServerDefine.h"
#include "../ConfigData/ConfigStruct.h"
#include "../ConfigData/ConfigData.h"
#include "../ServerData/RoleData.h"

CGameSvrMgr::CGameSvrMgr(void)
{
}

CGameSvrMgr::~CGameSvrMgr(void)
{
}

CGameSvrMgr* CGameSvrMgr::GetInstancePtr()
{
	static CGameSvrMgr _PlayerManager;

	return &_PlayerManager;
}


BOOL CGameSvrMgr::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_GAME_REGTO_LOGIC_REQ,		OnMsgGameSvrRegister);
			PROCESS_MESSAGE_ITEM(MSG_CREATE_SCENE_ACK,			OnMsgCreateSceneAck);
			PROCESS_MESSAGE_ITEM(MSG_TRANSFER_DATA_ACK,	        OnMsgTransRoleDataAck);
			PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,		    OnMsgEnterSceneReq);
			PROCESS_MESSAGE_ITEM(MSG_COPYINFO_REPORT_REQ,		OnMsgCopyReportReq);
			PROCESS_MESSAGE_ITEM(MSG_BATTLE_RESULT_NTY,		    OnMsgBattleResultNty);
	}

	return FALSE;
}

UINT32 CGameSvrMgr::GetServerIDByCopyID(UINT32 dwCopyGuid)
{
	return 1;
}

BOOL CGameSvrMgr::CreateScene(UINT32 dwCopyID, UINT64 CreateParam, UINT32 dwPlayerNum, UINT32 dwCopyType )
{
	ERROR_RETURN_TRUE(dwCopyID != 0);
	ERROR_RETURN_TRUE(CreateParam != 0);

	//选择一个可用的副本服务器
	UINT32 dwServerID = GetFreeGameServerID();
	if(dwServerID == 0)
	{
		CLog::GetInstancePtr()->LogError("没有找到可用的场景服务器，或者说没有找到可用的副本服务器");
		return FALSE;
	}

	//向副本服务器发送创建副本的消息
	if(!SendCreateSceneCmd(dwServerID, dwCopyID, dwCopyType, CreateParam, dwPlayerNum))
	{
		//发送创建副本的消息失败
		CLog::GetInstancePtr()->LogError("发送创建副本的消息失败");
		return FALSE;
	}

	return TRUE;
}


BOOL CGameSvrMgr::SendCreateSceneCmd( UINT32 dwServerID, UINT32 dwCopyID, UINT32 dwCopyType, UINT64 CreateParam, UINT32 dwPlayerNum )
{
	CreateNewSceneReq Req;
	Req.set_copyid(dwCopyID);
	Req.set_createparam(CreateParam);
	Req.set_copytype(dwCopyType);
	Req.set_playernum(dwPlayerNum);
	ERROR_RETURN_FALSE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(GetConnIDBySvrID(dwServerID), MSG_CREATE_SCENE_REQ, 0, 0, Req));
	return TRUE;
}


UINT32 CGameSvrMgr::GetConnIDBySvrID(UINT32 dwServerID)
{
	std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.find(dwServerID);
	if(itor == m_mapGameSvr.end())
	{
		return 0;
	}

	return itor->second.dwConnID;
}

BOOL CGameSvrMgr::SendPlayerToMainCity(UINT64 u64ID, UINT32 dwCopyID)
{
	UINT32 dwSvrID, dwConnID, dwCopyGuid;
	CGameSvrMgr::GetInstancePtr()->GetMainCityInfo(dwCopyID, dwSvrID, dwConnID, dwCopyGuid);
	ERROR_RETURN_TRUE(dwConnID != 0);
	ERROR_RETURN_FALSE(u64ID != 0);
	ERROR_RETURN_FALSE(dwCopyGuid != 0);
	ERROR_RETURN_FALSE(dwSvrID != 0);
	ERROR_RETURN_FALSE(dwCopyID != 0);
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(u64ID);
	ERROR_RETURN_FALSE(pPlayer != NULL);
	ERROR_RETURN_FALSE(pPlayer->m_dwCopyID != dwCopyID);
	ERROR_RETURN_FALSE(pPlayer->m_dwCopyGuid != dwCopyGuid);

	TransferDataReq Req;
	Req.set_camp(CT_PVE_PLAYER);
	ERROR_RETURN_FALSE(pPlayer->ToTransferData(Req));
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_TRANSFER_DATA_REQ, u64ID, dwCopyGuid, Req);
	pPlayer->m_dwToCopyID = dwCopyID;
	pPlayer->m_dwToCopyGuid = dwCopyGuid;
	pPlayer->m_dwToCopySvrID = dwSvrID;

	return TRUE;
}

BOOL CGameSvrMgr::SendPlayerToCopy(UINT64 u64ID, UINT32 dwServerID, UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCamp)
{
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(u64ID);
	ERROR_RETURN_FALSE(pPlayer != NULL);
	ERROR_RETURN_FALSE(pPlayer->m_dwCopyID != dwCopyID);
	ERROR_RETURN_FALSE(pPlayer->m_dwCopyGuid != dwCopyGuid);

	TransferDataReq Req;
	ERROR_RETURN_FALSE(pPlayer->ToTransferData(Req));
	Req.set_camp(CT_PVE_PLAYER);
	UINT32 dwConnID = CGameSvrMgr::GetInstancePtr()->GetConnIDBySvrID(dwServerID);
	ERROR_RETURN_FALSE(dwConnID != 0);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_TRANSFER_DATA_REQ, u64ID, dwCopyGuid, Req);
	pPlayer->m_dwToCopyID = dwCopyID;
	pPlayer->m_dwToCopyGuid = dwCopyGuid;
	pPlayer->m_dwToCopySvrID = dwServerID;
	return TRUE;
}

BOOL CGameSvrMgr::GetMainCityInfo(UINT32 dwCopyID, UINT32& dwServerID, UINT32& dwConnID, UINT32& dwCopyGuid)
{
	auto itor = m_mapCity.find(dwCopyID);
	if(itor != m_mapCity.end())
	{
		dwServerID = itor->second.m_dwSvrID;
		dwConnID = itor->second.m_dwConnID;
		dwCopyGuid = itor->second.m_dwCopyGuid;
		return TRUE;
	}

	dwServerID = 1;
	dwCopyGuid = dwServerID << 24 | 1;
	dwConnID = GetConnIDBySvrID(dwServerID);

	return TRUE;
}


BOOL CGameSvrMgr::OnMsgGameSvrRegister(NetPacket* pNetPacket)
{
	SvrRegToSvrReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.find(Req.serverid());
	if(itor != m_mapGameSvr.end())
	{
		itor->second.dwConnID = pNetPacket->m_dwConnID;
		itor->second.dwSvrID = Req.serverid();
	}

	m_mapGameSvr.insert(std::make_pair(Req.serverid(), GameSvrInfo(Req.serverid(), pNetPacket->m_dwConnID)));

	return TRUE;
}

BOOL CGameSvrMgr::OnMsgCopyReportReq(NetPacket* pNetPacket)
{
	CopyReportReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	//
	return TRUE;
	for(int i = 0; i < Req.copylist_size(); i++)
	{
		const CopyItem& item = Req.copylist(i);
		m_mapCity.insert(std::make_pair(item.copyid(), CityInfo(item.copyid(), item.serverid(), pNetPacket->m_dwConnID, item.copyguid())));
	}

	return TRUE;
}

BOOL CGameSvrMgr::OnMsgCreateSceneAck(NetPacket* pNetPacket)
{
	CreateNewSceneAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	ERROR_RETURN_TRUE(Ack.copyid() != 0);
	ERROR_RETURN_TRUE(Ack.copyguid() != 0);
	ERROR_RETURN_TRUE(Ack.serverid() != 0);
	ERROR_RETURN_TRUE(Ack.createparam() != 0);
	ERROR_RETURN_TRUE(Ack.playernum() != 0);
	ERROR_RETURN_TRUE(Ack.copytype() != 0);
	//StCopyBase *pCopyBase = CConfigData::GetInstancePtr()->GetCopyBaseInfo(Ack.copytype());
	//switch(pCopyBase->dwLogicType)
	//{
	//case 1:
	{
		OnCreateMainCopy(Ack);
	}

	//default:
	//	{
	//		break;
	//	}
	//}

	return TRUE;
}

BOOL CGameSvrMgr::OnCreateMainCopy(CreateNewSceneAck& Ack)
{
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Ack.createparam());
	ERROR_RETURN_FALSE(pPlayer != NULL);
	ERROR_RETURN_FALSE(pPlayer->m_dwCopyID != Ack.copyid());
	ERROR_RETURN_FALSE(pPlayer->m_dwCopyGuid != Ack.copyguid());
	//ERROR_RETURN_TRUE(CGameSvrMgr::GetInstancePtr()->SendPlayerToCity(Ack.createparam(), Ack.copyid(), Ack.copyguid(), Ack.serverid()));

	TransferDataReq Req;
	ERROR_RETURN_FALSE(pPlayer->ToTransferData(Req));
	Req.set_camp(CT_PVE_PLAYER);
	UINT32 dwConnID = CGameSvrMgr::GetInstancePtr()->GetConnIDBySvrID(Ack.serverid());
	ERROR_RETURN_FALSE(dwConnID != 0);

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_TRANSFER_DATA_REQ, Ack.createparam(), Ack.copyguid(), Req);
	pPlayer->m_dwToCopyID = Ack.copyid();
	pPlayer->m_dwToCopyGuid = Ack.copyguid();
	pPlayer->m_dwToCopySvrID = Ack.serverid();
	return TRUE;
}

BOOL CGameSvrMgr::OnMsgTransRoleDataAck(NetPacket* pNetPacket)
{
	TransferDataAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);
	ERROR_RETURN_TRUE(pHeader->u64TargetID == Ack.roleid());
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Ack.roleid());
	ERROR_RETURN_TRUE(pPlayer != NULL);
	ERROR_RETURN_TRUE(Ack.copyid() != 0);
	ERROR_RETURN_TRUE(Ack.copyguid() != 0);
	ERROR_RETURN_TRUE(Ack.serverid() != 0);
	pPlayer->SendIntoSceneNotify(Ack.copyguid(), Ack.copyid(), Ack.serverid());
	pPlayer->m_dwToCopyID = Ack.copyid();
	pPlayer->m_dwToCopyGuid = Ack.copyguid();
	pPlayer->m_dwToCopySvrID = Ack.serverid();
	return TRUE;
}

BOOL CGameSvrMgr::OnMsgEnterSceneReq(NetPacket* pNetPacket)
{
	EnterSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);
	ERROR_RETURN_TRUE(Req.copyguid() != 0);
	ERROR_RETURN_TRUE(Req.copyid() != 0);
	ERROR_RETURN_TRUE(Req.serverid() != 0);

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pPlayer->m_dwToCopyGuid == Req.copyguid());
	ERROR_RETURN_TRUE(pPlayer->m_dwToCopyID == Req.copyid());

	//如果原来在主城副本，需要通知离开
	if(pPlayer->m_dwCopyID == 6)
	{
		pPlayer->SendLeaveScene(pPlayer->m_dwCopyGuid, pPlayer->m_dwCopySvrID);
	}

	pPlayer->m_dwCopyGuid = Req.copyguid();
	pPlayer->m_dwCopyID = Req.copyid();
	pPlayer->m_dwCopySvrID = Req.serverid();
	pPlayer->m_dwToCopyID = 0;
	pPlayer->m_dwToCopyGuid = 0;
	pPlayer->m_dwToCopySvrID = 0;
	return TRUE;
}

BOOL CGameSvrMgr::OnCloseConnect(UINT32 dwConnID)
{


	return TRUE;
}

UINT32 CGameSvrMgr::GetFreeGameServerID()
{
	UINT32 dwMinLoad = 1000000;
	UINT32 dwSvrID = 0;
	for(std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.begin(); itor != m_mapGameSvr.end(); itor++)
	{
		if(itor->second.dwLoad < dwMinLoad)
		{
			dwSvrID = itor->second.dwSvrID;
			dwMinLoad = itor->second.dwLoad;
		}
	}

	return dwSvrID;
}

BOOL CGameSvrMgr::OnMsgBattleResultNty( NetPacket* pNetPacket )
{
	BattleResultNty Nty;
	Nty.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	ERROR_RETURN_TRUE(Nty.copytype() != 0);
	ERROR_RETURN_TRUE(Nty.copyid() != 0);
	ERROR_RETURN_TRUE(Nty.copyguid() != 0);
	ERROR_RETURN_TRUE(Nty.serverid() != 0);

	switch(Nty.copytype())
	{
		case CPT_MAIN:
			OnMainCopyResult(Nty);
			break;
	}

	return TRUE;
}


BOOL CGameSvrMgr::OnMainCopyResult(BattleResultNty& Nty)
{
	ERROR_RETURN_TRUE(Nty.playerlist_size() == 1);

	MainCopyResultNty Req;
	const ResultPlayer& Result = Nty.playerlist(0);

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Result.objectid());
	ERROR_RETURN_TRUE(pPlayer != NULL);

	CBagModule* pBagModule = (CBagModule*)pPlayer->GetModuleByType(MT_BAG);
	ERROR_RETURN_TRUE(pBagModule != NULL);

	CRoleModule* pRoleModule = (CRoleModule*)pPlayer->GetModuleByType(MT_ROLE);
	ERROR_RETURN_TRUE(pRoleModule != NULL);

	StCopyInfo* pCopyInfo = CConfigData::GetInstancePtr()->GetCopyInfo(Nty.copyid());
	ERROR_RETURN_TRUE(pCopyInfo != NULL);

	std::vector<StItemData> vtItemList;
	CConfigData::GetInstancePtr()->GetItemsFromAwardID(pCopyInfo->dwAwardID, pRoleModule->m_pRoleDataObject->m_CarrerID, vtItemList);

	for(std::vector<StItemData>::size_type i = 0; i < vtItemList.size(); i++)
	{
		pBagModule->AddItem(vtItemList[i].dwItemID, vtItemList[i].dwItemNum);
	}

	pRoleModule->AddExp(pCopyInfo->dwGetMoneyRatio * pRoleModule->m_pRoleDataObject->m_Level);

	pRoleModule->CostAction(pCopyInfo->dwCostActID, pCopyInfo->dwCostActNum);

	pPlayer->SendMsgProtoBuf(MSG_MAINCOPY_RESULT_NTY, Req);

	return TRUE;
}