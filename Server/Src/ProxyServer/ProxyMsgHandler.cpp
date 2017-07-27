#include "stdafx.h"
#include "ProxyMsgHandler.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "Utility/CommonSocket.h"
#include "StaticPlayerMgr.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"
#include "../Message/Msg_RetCode.pb.h"


CProxyMsgHandler::CProxyMsgHandler()
{
	CProxyPlayerMgr::GetInstancePtr();
}

CProxyMsgHandler::~CProxyMsgHandler()
{

}

BOOL CProxyMsgHandler::Init(UINT32 dwReserved)
{
	return TRUE;
}

BOOL CProxyMsgHandler::Uninit()
{
	return TRUE;
}

BOOL CProxyMsgHandler::DispatchPacket(NetPacket *pNetPacket)
{
	PacketHeader *pPacketHeader = (PacketHeader *)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_FALSE(pPacketHeader != NULL);

	switch(pNetPacket->m_dwMsgID)
	{
	PROCESS_MESSAGE_ITEM(MSG_GASVR_REGTO_PROXY_REQ,			OnMsgGameSvrRegister);

	case MSG_ROLE_LIST_REQ:
	case MSG_ROLE_CREATE_REQ:
	case MSG_ROLE_DELETE_REQ:
	case MSG_ROLE_LOGIN_REQ:
	case MSG_ROLE_LOGOUT_REQ:
		{
			pPacketHeader->dwUserData = pNetPacket->m_dwConnID;
			RelayToLogicServer(pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_LIST_ACK:
	case MSG_ROLE_CREATE_ACK:
	case MSG_ROLE_DELETE_ACK:
	case MSG_ROLE_LOGOUT_ACK:
		{
			RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_LOGIN_ACK:
		{
			CConnection *pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacketHeader->dwUserData);
			ERROR_RETURN_TRUE(pConnection != NULL);
			pConnection->SetConnectionData(pPacketHeader->u64TargetID);
			RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ENTER_SCENE_REQ:
		{
			//创建proxyplayer对象
			OnMsgEnterSceneReq(pNetPacket);
		}
		break;
	default:
		{
			if((pPacketHeader->dwMsgID >= MSG_LOGICSVR_MSGID_BEGIN)&&(pPacketHeader->dwMsgID <= MSG_LOGICSVR_MSGID_END))
			{
				if(pNetPacket->m_dwConnID == CGameService::GetInstancePtr()->GetLogicConnID())
				{
					RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
				}
				else
				{
					RelayToLogicServer(pNetPacket->m_pDataBuffer);
				}
			}
			else if((pPacketHeader->dwMsgID >= MSG_SCENESVR_MSGID_BEGIN)&&(pPacketHeader->dwMsgID <= MSG_SCENESVR_MSGID_END))
			{
				if(IsServerConnID(pNetPacket->m_dwConnID))
				{
					RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
				}
				else
				{
					CProxyPlayer *pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(pPacketHeader->u64TargetID);
					ERROR_RETURN_TRUE(pPlayer != NULL);
					
					UINT32 dwConnID = GetGameSvrConnID(pPlayer->GetGameSvrID());
					ERROR_RETURN_TRUE(dwConnID != 00);

					RelayToConnect(dwConnID, pNetPacket->m_pDataBuffer);
				}
			}
		}
	}

	return TRUE;
}

BOOL CProxyMsgHandler::OnNewConnect(CConnection *pConn)
{
	return TRUE;
}

BOOL CProxyMsgHandler::OnCloseConnect(CConnection *pConn)
{
	if(pConn->GetConnectionData() == 0)
	{
		return TRUE;
	}

	RoleDisconnectReq Req;
	Req.set_roleid(pConn->GetConnectionData());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_DISCONNECT_NTY, pConn->GetConnectionData(), 0,  Req);
	
	CProxyPlayer *pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(pConn->GetConnectionData());
	if(pPlayer == NULL)
	{
		return TRUE;
	}


	UINT32 dwConnID = GetGameSvrConnID(pPlayer->GetGameSvrID());
	ERROR_RETURN_TRUE(dwConnID != 0);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_DISCONNECT_NTY, pPlayer->GetCharID(), pPlayer->GetCopyID(),  Req);
	return TRUE;
}

BOOL CProxyMsgHandler::RelayToGameServer( CProxyPlayer *pClientObj, IDataBuffer *pBuffer )
{
	ERROR_RETURN_FALSE(pClientObj != NULL);

	return TRUE;
}

BOOL CProxyMsgHandler::RelayToLogicServer(IDataBuffer *pBuffer )
{
	if(!ServiceBase::GetInstancePtr()->SendMsgBuffer(CGameService::GetInstancePtr()->GetLogicConnID(), pBuffer))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CProxyMsgHandler::RelayToClient( CProxyPlayer *pStaticPlayer, IDataBuffer *pBuffer )
{
	ERROR_RETURN_FALSE(pStaticPlayer != NULL);

	if(!ServiceBase::GetInstancePtr()->SendMsgBuffer(pStaticPlayer->GetConnID(), pBuffer))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CProxyMsgHandler::RelayToConnect(UINT32 dwConnID, IDataBuffer *pBuffer)
{
	if(!ServiceBase::GetInstancePtr()->SendMsgBuffer(dwConnID, pBuffer))
	{
		return FALSE;
	}

	return TRUE;
}

UINT32 CProxyMsgHandler::GetGameSvrConnID(UINT32 dwSvrID)
{
	std::map<UINT32,UINT32>::iterator itor = m_mapSvrIDtoConnID.find(dwSvrID);
	if(itor != m_mapSvrIDtoConnID.end())
	{
		return itor->second;
	}

	return 0;
}

BOOL CProxyMsgHandler::IsServerConnID(UINT32 dwConnID)
{
	if(dwConnID == CGameService::GetInstancePtr()->GetLogicConnID())
	{
		return TRUE;
	}

	for(std::map<UINT32,UINT32>::iterator itor = m_mapSvrIDtoConnID.begin(); itor != m_mapSvrIDtoConnID.end(); itor++)
	{
		if(itor->second == dwConnID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CProxyMsgHandler::OnMsgGameSvrRegister(NetPacket *pPacket)
{
	SvrRegToSvrReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	m_mapSvrIDtoConnID.insert(std::make_pair(Req.serverid(), pPacket->m_dwConnID));

	SvrRegToSvrAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_GASVR_REGTO_PROXY_ACK, 0, 0, Ack);

	return TRUE;
}

BOOL CProxyMsgHandler::OnMsgEnterSceneReq(NetPacket *pNetPacket)
{
	EnterSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	PacketHeader *pPacketHeader = (PacketHeader *)pNetPacket->m_pDataBuffer->GetBuffer();


	CProxyPlayer *pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(Req.roleid());
	if(pPlayer != NULL)
	{
		pPlayer->SetGameSvrInfo(Req.serverid(), Req.copyid());
	}
	else
	{
		pPlayer = CProxyPlayerMgr::GetInstancePtr()->CreateProxyPlayer(Req.roleid());
		pPlayer->SetGameSvrInfo(Req.serverid(), Req.copyid());
	}

	UINT32 dwConnID = GetGameSvrConnID(pPacketHeader->u64TargetID);
	ERROR_RETURN_TRUE(dwConnID != 0)
	pPacketHeader->u64TargetID = pNetPacket->m_dwConnID;
	RelayToConnect(dwConnID, pNetPacket->m_pDataBuffer);
	RelayToLogicServer(pNetPacket->m_pDataBuffer);
	return TRUE;
}
