#include "stdafx.h"
#include "ProxyMsgHandler.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "ProxyPlayerMgr.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_LoginCltData.pb.h"

CProxyMsgHandler::CProxyMsgHandler()
{
    CProxyPlayerMgr::GetInstancePtr();
}

CProxyMsgHandler::~CProxyMsgHandler()
{

}

BOOL CProxyMsgHandler::Init(INT32 nReserved)
{
    return TRUE;
}

BOOL CProxyMsgHandler::Uninit()
{
    return TRUE;
}

BOOL CProxyMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
    PacketHeader* pPacketHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_FALSE(pPacketHeader != NULL);

    switch(pNetPacket->m_nMsgID)
    {
            PROCESS_MESSAGE_ITEM(MSG_GASVR_REGTO_PROXY_REQ,         OnMsgGameSvrRegister);
            PROCESS_MESSAGE_ITEM(MSG_BROAD_MESSAGE_NOTIFY,          OnMsgBroadMessageNty);
            PROCESS_MESSAGE_ITEM(MSG_NOTIFY_INTO_SCENE,             OnMsgNotifyIntoSceneNtf);
            PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,               OnMsgEnterSceneReq);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_ACK,                OnMsgRoleLoginAck);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGOUT_REQ,               OnMsgRoleLogoutReq);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_OTHER_LOGIN_NTY,          OnMsgKickoutNty);
            PROCESS_MESSAGE_ITEM(MSG_REMOVE_CONNECT_NTY,            OnMsgRemoveConnectNty);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_REQ,                 OnMsgRelayToLogic);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_CREATE_REQ,               OnMsgRelayToLogic);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_DELETE_REQ,               OnMsgRelayToLogic);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_REQ,                OnMsgRelayToLogic);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_RECONNECT_REQ,            OnMsgReconnectReq);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_RECONNECT_ACK,            OnMsgReconnectAck);

        default:
        {
            if((pPacketHeader->nMsgID >= MSG_LOGICSVR_MSGID_BEGIN) && (pPacketHeader->nMsgID <= MSG_LOGICSVR_MSGID_END))
            {
                if(pNetPacket->m_nConnID == CGameService::GetInstancePtr()->GetLogicConnID())
                {
                    RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
                }
                else //这是客户端发过来的消息
                {
                    CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(pNetPacket->m_nConnID);
                    ERROR_RETURN_TRUE(pConnection !=  NULL);

                    if (pConnection->GetConnectionData() != pPacketHeader->u64TargetID)
                    {
                        return TRUE;
                    }

                    RelayToLogicServer(pNetPacket->m_pDataBuffer);
                }
            }
            else if((pPacketHeader->nMsgID >= MSG_SCENESVR_MSGID_BEGIN) && (pPacketHeader->nMsgID <= MSG_SCENESVR_MSGID_END))
            {
                if(IsServerConnID(pNetPacket->m_nConnID))
                {
                    RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
                }
                else //这是客户端发过来的消息
                {
                    CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByRoleID(pPacketHeader->u64TargetID);
                    ERROR_RETURN_TRUE(pPlayer != NULL);

                    INT32 nConnID = GetGameSvrConnID(pPlayer->GetGameSvrID());
                    ERROR_RETURN_TRUE(nConnID != 0);

                    //pPacketHeader->dwUserData = pPlayer->GetCopyGuid();
                    //疑问, pPakcetHeader->dwUserData字段需不需要由客户端来填，现在proxyserver也可以获取到.
                    //这个可能会加快向游戏服的转发速度

                    RelayToConnect(nConnID, pNetPacket->m_pDataBuffer);
                }
            }
        }
    }

    return TRUE;
}

BOOL CProxyMsgHandler::OnNewConnect(INT32 nConnID)
{
    return TRUE;
}

BOOL CProxyMsgHandler::OnCloseConnect(INT32 nConnID)
{
    CConnection* pConn = ServiceBase::GetInstancePtr()->GetConnectionByID(nConnID);
    ERROR_RETURN_FALSE(pConn != NULL);

    if(pConn->GetConnectionData() == 0)
    {
        return TRUE;
    }

    RoleDisconnectReq Req;
    Req.set_roleid(pConn->GetConnectionData());
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_DISCONNECT_NTY, pConn->GetConnectionData(), 0,  Req);

    CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByRoleID(pConn->GetConnectionData());
    ERROR_RETURN_TRUE(pPlayer != NULL);
    ERROR_RETURN_TRUE(pPlayer->GetConnID() == nConnID);

    if (pPlayer->GetGameSvrID() != 0)
    {
        INT32 nConnID = GetGameSvrConnID(pPlayer->GetGameSvrID());
        ERROR_RETURN_TRUE(nConnID != 0);
        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_DISCONNECT_NTY, pPlayer->GetRoleID(), pPlayer->GetCopyGuid(), Req);
    }

    pPlayer->SetConnID(0);

    return TRUE;
}

BOOL CProxyMsgHandler::RelayToGameServer( CProxyPlayer* pClientObj, IDataBuffer* pBuffer )
{
    ERROR_RETURN_FALSE(pClientObj != NULL);

    return TRUE;
}

BOOL CProxyMsgHandler::RelayToLogicServer(IDataBuffer* pBuffer )
{
    if(!ServiceBase::GetInstancePtr()->SendMsgBuffer(CGameService::GetInstancePtr()->GetLogicConnID(), pBuffer))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CProxyMsgHandler::RelayToConnect(INT32 nConnID, IDataBuffer* pBuffer)
{
    if(!ServiceBase::GetInstancePtr()->SendMsgBuffer(nConnID, pBuffer))
    {
        return FALSE;
    }

    return TRUE;
}

INT32 CProxyMsgHandler::GetGameSvrConnID(INT32 nSvrID)
{
    std::map<INT32, INT32>::iterator itor = m_mapSvrIDtoConnID.find(nSvrID);
    if(itor != m_mapSvrIDtoConnID.end())
    {
        return itor->second;
    }

    return 0;
}

BOOL CProxyMsgHandler::IsServerConnID(INT32 nConnID)
{
    if(nConnID == CGameService::GetInstancePtr()->GetLogicConnID())
    {
        return TRUE;
    }

    for(std::map<INT32, INT32>::iterator itor = m_mapSvrIDtoConnID.begin(); itor != m_mapSvrIDtoConnID.end(); itor++)
    {
        if(itor->second == nConnID)
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CProxyMsgHandler::OnMsgGameSvrRegister(NetPacket* pPacket)
{
    SvrRegToSvrReq Req;
    Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

    m_mapSvrIDtoConnID.insert(std::make_pair(Req.serverid(), pPacket->m_nConnID));

    SvrRegToSvrAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_GASVR_REGTO_PROXY_ACK, 0, 0, Ack);

    CConnection* pConn = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacket->m_nConnID);
    ERROR_RETURN_FALSE(pConn != NULL);

    pConn->SetConnectionData(1);

    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgNotifyIntoSceneNtf(NetPacket* pPacket)
{
    NotifyIntoScene Nty;
    Nty.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pPacketHeader->u64TargetID != 0);

    RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);

    CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByRoleID(Nty.roleid());
    ERROR_RETURN_TRUE(pPlayer != NULL);

    pPlayer->SetGameSvrInfo(Nty.serverid(), Nty.copyguid());

    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgEnterSceneReq(NetPacket* pNetPacket)
{
    EnterSceneReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pPacketHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pPacketHeader->u64TargetID != 0);

    CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByRoleID(Req.roleid());
    ERROR_RETURN_TRUE(pPlayer != NULL);

    INT32 nConnID = GetGameSvrConnID(Req.serverid());
    ERROR_RETURN_TRUE(nConnID != 0)
    pPacketHeader->u64TargetID = pNetPacket->m_nConnID;
    RelayToConnect(nConnID, pNetPacket->m_pDataBuffer);
    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgBroadMessageNty(NetPacket* pPacket)
{
    BroadMessageNotify Nty;
    Nty.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

    for(int i = 0; i < Nty.connid_size(); i++)
    {
        ServiceBase::GetInstancePtr()->SendMsgRawData(Nty.connid(i), Nty.msgid(), 0, 0, Nty.msgdata().c_str(), (INT32)Nty.msgdata().size());
    }

    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgRoleLoginAck(NetPacket* pPacket)
{
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pPacketHeader != NULL);
    CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacketHeader->dwUserData);
    ERROR_RETURN_TRUE(pConnection != NULL);

    RoleLoginAck Ack;
    Ack.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
    if (Ack.retcode() != MRC_SUCCESSED)
    {
        RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);
        return TRUE;
    }

    pConnection->SetConnectionData(pPacketHeader->u64TargetID);

    RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);

    CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByRoleID(pPacketHeader->u64TargetID);
    if (pPlayer == NULL)
    {
        pPlayer = CProxyPlayerMgr::GetInstancePtr()->CreateProxyPlayer(pPacketHeader->u64TargetID);
    }

    pPlayer->SetConnID(pPacketHeader->dwUserData);

    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgRoleLogoutReq(NetPacket* pPacket)
{
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pPacketHeader != NULL);

    RelayToLogicServer(pPacket->m_pDataBuffer);

    CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacket->m_nConnID);
    ERROR_RETURN_TRUE(pConnection != NULL);
    pConnection->SetConnectionData(0);
    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgKickoutNty(NetPacket* pPacket)
{
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pPacketHeader != NULL);

    RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);
    CConnection* pConn = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacketHeader->dwUserData);
    if (pConn != NULL)
    {
        pConn->SetConnectionData(0);
    }

    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgRemoveConnectNty(NetPacket* pPacket)
{
    RemoveConnectNty Nty;
    Nty.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

    CProxyPlayerMgr::GetInstancePtr()->RemoveByRoleID(Nty.roleid());

    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgRelayToLogic(NetPacket* pPacket)
{
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_FALSE(pPacketHeader != NULL);

    pPacketHeader->dwUserData = pPacket->m_nConnID;
    RelayToLogicServer(pPacket->m_pDataBuffer);

    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgReconnectReq(NetPacket* pPacket)
{
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_FALSE(pPacketHeader != NULL);

    CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByRoleID(pPacketHeader->u64TargetID);
    if (pPlayer != NULL && pPlayer->GetConnID() != 0)
    {
        ERROR_RETURN_FALSE(pPlayer->GetConnID() != pPacket->m_nConnID);

        CConnection* pConn = ServiceBase::GetInstancePtr()->GetConnectionByID(pPlayer->GetConnID());
        if (pConn != NULL)
        {
            CLog::GetInstancePtr()->LogError("OnMsgReconnectReq RoleID:%lld, OrgConnID:%d, NewConnID:%d", pPacketHeader->u64TargetID, pPlayer->GetConnID(), pPacket->m_nConnID);
            pConn->SetConnectionData(0);
            pConn->Shutdown();
        }
        else
        {
            CLog::GetInstancePtr()->LogError("OnMsgReconnectReq Error orgin connect alreay removed:%d", pPlayer->GetConnID());
        }
    }

    pPacketHeader->dwUserData = pPacket->m_nConnID;

    if (!CGameService::GetInstancePtr()->m_bLogicConnect)
    {
        RoleReconnectAck Ack;
        Ack.set_retcode(MRC_CANNOT_RECONNECT); //内存己经没有玩家必须要重登
        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ROLE_RECONNECT_ACK, 0, 0, Ack);
        return TRUE;
    }
    RelayToLogicServer(pPacket->m_pDataBuffer);

    return TRUE;
}

BOOL CProxyMsgHandler::OnMsgReconnectAck(NetPacket* pPacket)
{
    PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pPacketHeader != NULL);
    CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacketHeader->dwUserData);
    ERROR_RETURN_TRUE(pConnection != NULL);

    if (pPacketHeader->u64TargetID == 0)
    {
        RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);
        return TRUE;
    }

    pConnection->SetConnectionData(pPacketHeader->u64TargetID);

    RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);

    CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByRoleID(pPacketHeader->u64TargetID);
    if (pPlayer == NULL)
    {
        pPlayer = CProxyPlayerMgr::GetInstancePtr()->CreateProxyPlayer(pPacketHeader->u64TargetID);
    }

    pPlayer->SetConnID(pPacketHeader->dwUserData);

    return TRUE;
}
