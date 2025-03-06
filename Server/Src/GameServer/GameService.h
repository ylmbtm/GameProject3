#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "SceneManager.h"

class CGameService : public IPacketDispatcher
{
private:
    CGameService(void);
    virtual ~CGameService(void);

public:
    static CGameService* GetInstancePtr();

    BOOL        Init(INT32 nServerID, INT32 nPort);

    BOOL        Uninit();

    BOOL        Run();

    BOOL        OnNewConnect(INT32 nConnID);

    BOOL        OnCloseConnect(INT32 nConnID);

    BOOL        OnSecondTimer();

    BOOL        DispatchPacket( NetPacket* pNetPacket);

    BOOL        SetLogicConnID(INT32 nConnID);

    UINT32      GetLogicConnID();

    UINT32      GetProxyConnID();

    BOOL        ConnectToLogicSvr();

    BOOL        ConnectToProxySvr();

    BOOL        RegisterToLogicSvr();

    BOOL        RegisterToProxySvr();

    UINT32      GetServerID();

protected:
    CSceneManager       m_SceneManager;

    INT32               m_nLogicConnID;
    INT32               m_nProxyConnID;
    INT32               m_nServerID;
public:
    //*********************消息处理定义开始******************************
    BOOL OnMsgDefautReq(NetPacket* pNetPacket);
    BOOL OnMsgRegToProxyAck(NetPacket* pNetPacket);
    BOOL OnMsgWebCommandReq(NetPacket* pNetPacket);
    //*********************消息处理定义结束******************************
};

#endif
