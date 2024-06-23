#ifndef _PROXY_SERVER_MSG_HANDLER_H_
#define _PROXY_SERVER_MSG_HANDLER_H_

class  CProxyPlayer;

class CProxyMsgHandler
{
public:
    CProxyMsgHandler();

    ~CProxyMsgHandler();

    BOOL        Init(INT32 nReserved);

    BOOL        Uninit();

    BOOL        DispatchPacket(NetPacket* pNetPacket);

    BOOL        OnNewConnect(INT32 nConnID);

    BOOL        OnCloseConnect(INT32 nConnID);
public:
    BOOL        RelayToGameServer(CProxyPlayer* pClientObj, IDataBuffer* pBuffer);

    BOOL        RelayToLogicServer(IDataBuffer* pBuffer);

    BOOL        RelayToConnect(INT32 nConnID, IDataBuffer* pBuffer);

    INT32       GetGameSvrConnID(INT32 nSvrID);

    BOOL        IsServerConnID(INT32 nConnID);

public:
    //*********************消息处理定义开始******************************
    BOOL        OnMsgGameSvrRegister(NetPacket* pPacket);
    BOOL        OnMsgBroadMessageNty(NetPacket* pPacket);
    BOOL        OnMsgNotifyIntoSceneNtf(NetPacket* pPacket);
    BOOL        OnMsgEnterSceneReq(NetPacket* pPacket);
    BOOL        OnMsgRoleLoginAck(NetPacket* pPacket);
    BOOL        OnMsgRoleLogoutReq(NetPacket* pPacket);
    BOOL        OnMsgKickoutNty(NetPacket* pPacket);
    BOOL        OnMsgRemoveConnectNty(NetPacket* pPacket);
    BOOL        OnMsgRelayToLogic(NetPacket* pPacket);
    BOOL        OnMsgReconnectReq(NetPacket* pPacket);
    BOOL        OnMsgReconnectAck(NetPacket* pPacket);
    //*********************消息处理定义结束******************************
public:
    std::map<INT32, INT32> m_mapSvrIDtoConnID;

};

#endif //_PROXY_SERVER_MSG_HANDLER_H_
