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

	BOOL		Init();

	BOOL		Uninit();

	BOOL		Run();

	void		KillWatchProcess(void);

	BOOL		OnNewConnect(CConnection* pConn);

	BOOL		OnCloseConnect(CConnection* pConn);

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL		AutoRun();

public:

	CWatchMsgHandler		m_WatchMsgHandler;
};

#endif
