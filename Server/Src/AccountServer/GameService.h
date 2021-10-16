#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "AccountMsgHandler.h"
class CConnection;

class CGameService  : public IPacketDispatcher
{
private:
    CGameService(void);
    virtual ~CGameService(void);

public:
    static CGameService* GetInstancePtr();

public:
    BOOL        Init();

    BOOL        Uninit();

    BOOL        Run();

    BOOL        OnNewConnect(INT32 nConnID);

    BOOL        OnCloseConnect(INT32 nConnID);

    BOOL        OnSecondTimer();

    BOOL        DispatchPacket( NetPacket* pNetPacket);

    BOOL        ConnectToLogServer();

    INT32       GetLogSvrConnID();
public:
    INT32                   m_nLogSvrConnID;

    CAccountMsgHandler      m_AccountMsgHandler;
public:
    //*********************消息处理定义开始******************************
    //*********************消息处理定义结束******************************
};

#endif
