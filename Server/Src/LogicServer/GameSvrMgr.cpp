#include "stdafx.h"
#include "GameSvrMgr.h"
#include "CommandDef.h"
#include "../LogicServer/GameService.h"
#include "../Message/Msg_Login.pb.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"

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


UINT32 CGameSvrMgr::GetServerIDByCopyID(UINT32 dwCopyID)
{
	return 1;
}

BOOL CGameSvrMgr::CreateScene(UINT32 dwCopyType, UINT64 CreateParam)
{
	//选择一个可用的副本服务器
	UINT32 dwServerID = GetFreeGameServerID();
	if(dwServerID == INVALIDE_SERVERID)
	{
		ASSERT_FAIELD;
		//没有找到可用的场景服务器，或者说没有找到可用的副本服务器
		return FALSE;
	}

	//向副本服务器发送创建副本的消息
	if(SendCreateSceneCmd(dwServerID, dwCopyType, CreateParam))
	{
		//发送创建副本的消息失败

		ASSERT_FAIELD;

		return FALSE;
	}

	return TRUE;
}

BOOL CGameSvrMgr::SendCreateSceneCmd( UINT32 dwServerID, UINT32 dwCopyType, UINT64 CreateParam )
{
	CreateNewSceneReq Req;
	Req.set_copytype(dwCopyType);
	Req.set_createparam(CreateParam);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(GetConnIDBySvrID(dwServerID), MSG_CREATE_SCENE_REQ, 0, 0, Req);
	return TRUE;
}


UINT32 CGameSvrMgr::GetConnIDBySvrID(UINT32 dwServerID)
{
	return 0;
}

BOOL CGameSvrMgr::GetMainScene(UINT32 &dwServerID, UINT32 &dwConnID, UINT32 &dwCopyID)
{
	dwServerID = 1;
	dwCopyID = 1;
	dwConnID = GetConnIDBySvrID(dwServerID);
	return TRUE;
}


BOOL CGameSvrMgr::SendPlayerToScene(UINT64 uID, UINT32 dwCopyID, UINT32 dwSvrID)
{
	CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(uID);
	if(pPlayer == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}


}

BOOL CGameSvrMgr::OnMsgGameSvrRegister(NetPacket *pNetPacket)
{
	GmsvrRegToLogicReq Req;
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

BOOL CGameSvrMgr::OnCloseConnect(UINT32 dwConnID)
{


	return TRUE;
}

UINT32 CGameSvrMgr::GetFreeGameServerID()
{
	return 0;
}
