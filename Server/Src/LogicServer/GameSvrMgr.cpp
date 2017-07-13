#include "stdafx.h"
#include "GameSvrMgr.h"
#include "CommandDef.h"
#include "../LogicServer/GameService.h"
#include "../Message/Msg_Login.pb.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "Utility/Log/Log.h"

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


BOOL CGameSvrMgr::DispatchPacket(NetPacket *pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
		PROCESS_MESSAGE_ITEM(MSG_GMSVR_REGTO_LOGIC_REQ,		OnMsgGameSvrRegister);
		PROCESS_MESSAGE_ITEM(MSG_CREATE_SCENE_ACK,			OnMsgCreateSceneAck);
		//PROCESS_MESSAGE_ITEM(MSG_COPYINFO_REPORT_REQ,		OnMsgCopyReportReq);
	default:
		{
			return FALSE;
		}
		break;
	}

	return TRUE;
}

UINT32 CGameSvrMgr::GetServerIDByCopyID(UINT32 dwCopyID)
{
	return 1;
}

BOOL CGameSvrMgr::CreateScene(UINT32 dwCopyType, UINT64 CreateParam)
{
	//选择一个可用的副本服务器
	UINT32 dwServerID = GetFreeGameServerID();
	if(dwServerID == 0)
	{
		CLog::GetInstancePtr()->LogError("没有找到可用的场景服务器，或者说没有找到可用的副本服务器");
		return FALSE;
	}

	//向副本服务器发送创建副本的消息
	if(SendCreateSceneCmd(dwServerID, dwCopyType, CreateParam))
	{
		//发送创建副本的消息失败
		CLog::GetInstancePtr()->LogError("发送创建副本的消息失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGameSvrMgr::SendCreateSceneCmd( UINT32 dwServerID, UINT32 dwCopyType, UINT64 CreateParam )
{
	CreateNewSceneReq Req;
	Req.set_copytype(dwCopyType);
	Req.set_createparam(CreateParam);
	if(!ServiceBase::GetInstancePtr()->SendMsgProtoBuf(GetConnIDBySvrID(dwServerID), MSG_CREATE_SCENE_REQ, 0, 0, Req))
	{
		ASSERT_FAIELD;
		return FALSE;
	}
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

BOOL CGameSvrMgr::GetMainScene(UINT32 &dwServerID, UINT32 &dwConnID, UINT32 &dwCopyID)
{
	dwServerID = 1;
	dwCopyID = 1<<24|2;
	dwConnID = GetConnIDBySvrID(dwServerID);
	return TRUE;
}


BOOL CGameSvrMgr::SendPlayerToScene(UINT64 uID, UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwSvrID)
{
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(uID);
    ERROR_RETURN_FALSE(pPlayer != NULL);
    UINT32 dwConnID = GetConnIDBySvrID(dwSvrID);
    ERROR_RETURN_FALSE(dwConnID != 0);
    ERROR_RETURN_FALSE(pPlayer->SendToCopy(dwCopyType, dwCopyID, dwConnID));
    return TRUE;
}

BOOL CGameSvrMgr::OnMsgGameSvrRegister(NetPacket *pNetPacket)
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

	m_mapGameSvr.insert(std::make_pair(Req.serverid(),GameSvrInfo(Req.serverid(), pNetPacket->m_dwConnID)));
	
	return TRUE;
}

// BOOL CGameSvrMgr::OnMsgCopyReportReq(NetPacket *pNetPacket)
// {
// 
// }

BOOL CGameSvrMgr::OnMsgCreateSceneAck(NetPacket *pNetPacket)
{
	CreateNewSceneAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	//if(Ack.copytype() == 1)
	//{

	//}

	//通知玩家可以进入
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Ack.createparam());
	ERROR_RETURN_TRUE(pPlayer != NULL);

	pPlayer->SendToCopy(Ack.copytype(), Ack.copyid(), Ack.serverid());
	
	return TRUE;
}

BOOL CGameSvrMgr::OnCloseConnect(UINT32 dwConnID)
{


	return TRUE;
}

UINT32 CGameSvrMgr::GetFreeGameServerID()
{
	UINT32 dwMax = 100000;
	UINT32 dwSvrID = 0;
	for(std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.begin(); itor != m_mapGameSvr.end(); itor++)
	{
		if(itor->second.dwLoad > dwMax)
		{
			dwSvrID = itor->second.dwSvrID;
			dwMax = itor->second.dwLoad;
		}
	}
	
	return dwSvrID;
}
