#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "AccountMsgHandler.h"
class CConnection;

#define DB_THREAD_NUM 10

class CGameService  : public IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

public:
	BOOL		Init();

	BOOL		Uninit();

	BOOL		Run();

	BOOL		OnNewConnect(CConnection *pConn);

	BOOL		OnCloseConnect(CConnection *pConn);

	BOOL		DispatchPacket( NetPacket *pNetPacket);

public:
	CAccountMsgHandler		m_AccountMsgHandler;
};

#endif
