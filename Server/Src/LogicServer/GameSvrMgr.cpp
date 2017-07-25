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
        PROCESS_MESSAGE_ITEM(MSG_TRANS_ROLE_DATA_ACK,	    OnMsgTransRoleDataAck);
        PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,		    OnMsgEnterSceneReq);
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

BOOL CGameSvrMgr::CreateScene(UINT32 dwCopyType, UINT64 CreateParam, UINT32 dwPlayerNum )
{
	ERROR_RETURN_TRUE(dwCopyType != 0);
	ERROR_RETURN_TRUE(CreateParam != 0);

	//选择一个可用的副本服务器
	UINT32 dwServerID = GetFreeGameServerID();
	if(dwServerID == 0)
	{
		CLog::GetInstancePtr()->LogError("没有找到可用的场景服务器，或者说没有找到可用的副本服务器");
		return FALSE;
	}

	//向副本服务器发送创建副本的消息
	if(!SendCreateSceneCmd(dwServerID, dwCopyType, CreateParam, dwPlayerNum))
	{
		//发送创建副本的消息失败
		CLog::GetInstancePtr()->LogError("发送创建副本的消息失败");
		return FALSE;
	}

	return TRUE;
}


BOOL CGameSvrMgr::SendCreateSceneCmd( UINT32 dwServerID, UINT32 dwCopyType, UINT64 CreateParam, UINT32 dwPlayerNum )
{
	CreateNewSceneReq Req;
	Req.set_copytype(dwCopyType);
	Req.set_createparam(CreateParam);
	Req.set_logictype(1);
	Req.set_playernum(dwPlayerNum);
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


BOOL CGameSvrMgr::SendPlayerToCopy( UINT64 u64RoleID, UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwSvrID )
{
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(u64RoleID);
    ERROR_RETURN_FALSE(pPlayer != NULL);
    UINT32 dwConnID = GetConnIDBySvrID(dwSvrID);
    ERROR_RETURN_FALSE(dwConnID != 0);
    ERROR_RETURN_FALSE(dwCopyID != 0);
    TransRoleDataReq Req;
    ERROR_RETURN_FALSE(pPlayer->ToTransRoleData(Req));
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_TRANS_ROLE_DATA_REQ, u64RoleID, dwCopyID, Req);
    pPlayer->m_dwToCopyID = dwCopyID;
    pPlayer->m_dwToCopyType = dwCopyType;
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
	ERROR_RETURN_TRUE(Ack.copytype() != 0);
	ERROR_RETURN_TRUE(Ack.copyid() != 0);
	ERROR_RETURN_TRUE(Ack.serverid() != 0);
	ERROR_RETURN_TRUE(Ack.createparam() != 0);
	ERROR_RETURN_TRUE(Ack.playernum() != 0);
	ERROR_RETURN_TRUE(Ack.logictype() != 0);
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

BOOL CGameSvrMgr::OnCreateMainCopy(CreateNewSceneAck &Ack)
{
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Ack.createparam());
	ERROR_RETURN_FALSE(pPlayer != NULL);
	ERROR_RETURN_TRUE(CGameSvrMgr::GetInstancePtr()->SendPlayerToCopy(Ack.createparam(), Ack.copytype(), Ack.copyid(), Ack.serverid()));
	return TRUE;
}

BOOL CGameSvrMgr::OnMsgTransRoleDataAck(NetPacket *pNetPacket)
{
    TransRoleDataAck Ack;
    Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);
    ERROR_RETURN_TRUE(pHeader->u64TargetID == Ack.roleid());
    CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Ack.roleid());
    ERROR_RETURN_FALSE(pPlayer != NULL);
    ERROR_RETURN_TRUE(Ack.copyid() != 0);
    ERROR_RETURN_TRUE(Ack.copytype() != 0);
    ERROR_RETURN_TRUE(Ack.serverid() != 0);
    pPlayer->SendIntoSceneNotify(Ack.copyid(), Ack.copytype(), Ack.serverid());
    return TRUE;
}

BOOL CGameSvrMgr::OnMsgEnterSceneReq(NetPacket *pNetPacket)
{
    EnterSceneReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

    CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
    if(pPlayer->m_dwToCopyID != Req.copyid())
    {
        ASSERT_FAIELD;
    }

    //如果原来在主城副本，需要通知离开
    if(pPlayer->m_dwCopyType == 6)
    {
        pPlayer->SendLeaveScene(pPlayer->m_dwCopyID,pPlayer->m_dwCopySvrID);
    }

    pPlayer->m_dwCopyID = Req.copyid();
    pPlayer->m_dwCopySvrID = Req.serverid();
    pPlayer->m_dwToCopyID = 0;
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
