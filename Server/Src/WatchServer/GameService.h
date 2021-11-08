#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "WatchMsgHandler.h"

class CGameService : public IPacketDispatcher
{
private:
    CGameService(void);
    virtual ~CGameService(void);

public:
    static CGameService* GetInstancePtr();

    BOOL        Init();

    BOOL        Uninit();

    BOOL        Run();

    BOOL        OnNewConnect(INT32 nConnID);

    BOOL        OnCloseConnect(INT32 nConnID);

    BOOL        OnSecondTimer();

    BOOL        DispatchPacket( NetPacket* pNetPacket);

    CWatchMsgHandler        m_WatchMsgHandler;

    BOOL                    m_bIsRun;
};

#endif
