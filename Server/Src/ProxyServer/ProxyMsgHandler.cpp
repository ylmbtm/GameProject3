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
	if(pPacketHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	switch(pNetPacket->m_dwMsgID)
	{
	case MSG_ROLE_LIST_REQ:
		{
			pPacketHeader->dwUserData = pNetPacket->m_dwConnID;

			RelayToLogicServer(pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_LIST_ACK:
		{
			RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_CREATE_REQ:
		{
			pPacketHeader->dwUserData = pNetPacket->m_dwConnID;

			RelayToLogicServer(pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_CREATE_ACK:
		{
			RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_DELETE_REQ:
		{
			pPacketHeader->dwUserData = pNetPacket->m_dwConnID;

			RelayToLogicServer(pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_DELETE_ACK:
		{
			RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_LOGIN_REQ:
		{
			pPacketHeader->dwUserData = pNetPacket->m_dwConnID;

			RelayToLogicServer(pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_LOGIN_ACK:
		{
			RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_LOGOUT_REQ:
		{
			pPacketHeader->u64TargetID = pNetPacket->m_dwConnID;

			RelayToLogicServer(pNetPacket->m_pDataBuffer);
		}
		break;
	case MSG_ROLE_LOGOUT_ACK:
		{
			RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
		}
		break;
	default:
		{
			if((pPacketHeader->dwMsgID >= MSG_LOGICSVR_MSGID_BEGIN)&&(pPacketHeader->dwMsgID <= MSG_LOGICSVR_MSGID_END))
			{
				RelayToLogicServer(pNetPacket->m_pDataBuffer);
			}
			else if((pPacketHeader->dwMsgID >= MSG_SCENESVR_MSGID_BEGIN)&&(pPacketHeader->dwMsgID <= MSG_SCENESVR_MSGID_END))
			{

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

	CProxyPlayer *pStaticPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(pConn->GetConnectionData());
	if(pStaticPlayer == NULL)
	{
		CLog::GetInstancePtr()->AddLog("收到连接断开的事件， 但没有玩家连接数据!!!!!!");
		return TRUE;
	}

	if(pStaticPlayer->m_CharState == LS_OffLine)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	RoleDisconnectReq Req;
	Req.set_roleid(pConn->GetConnectionData());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(),MSG_DISCONNECT_NTY, 0, 0,  Req);
	pStaticPlayer->m_CharState = LS_OffLine;
	return TRUE;
}

BOOL CProxyMsgHandler::RelayToGameServer( CProxyPlayer *pClientObj, IDataBuffer *pBuffer )
{
	if(pClientObj == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	if(pClientObj->m_CharState == LS_OffLine)
	{
		ASSERT_FAIELD;

		return FALSE;
	}


	return TRUE;
}

BOOL CProxyMsgHandler::RelayToLogicServer(IDataBuffer *pBuffer )
{
	pBuffer->AddRef();
	if(!ServiceBase::GetInstancePtr()->SendMsgBuffer(CGameService::GetInstancePtr()->GetLogicConnID(), pBuffer))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	return TRUE;
}

BOOL CProxyMsgHandler::RelayToClient( CProxyPlayer *pStaticPlayer, IDataBuffer *pBuffer )
{
	if(pStaticPlayer == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	pBuffer->AddRef();
	if(!ServiceBase::GetInstancePtr()->SendMsgBuffer(pStaticPlayer->GetClientConnID(), pBuffer))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	return TRUE;
}

BOOL CProxyMsgHandler::RelayToConnect(UINT32 dwConnID, IDataBuffer *pBuffer)
{
	pBuffer->AddRef();
	if(!ServiceBase::GetInstancePtr()->SendMsgBuffer(dwConnID, pBuffer))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	return TRUE;
}
