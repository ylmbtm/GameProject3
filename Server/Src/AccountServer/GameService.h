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

	BOOL		OnNewConnect(UINT32 nConnID);

	BOOL		OnCloseConnect(UINT32 nConnID);

	BOOL		OnSecondTimer();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL	    ConnectToLogServer();

	UINT32		GetLogSvrConnID();
public:
	UINT32					m_dwLogSvrConnID;

	CAccountMsgHandler		m_AccountMsgHandler;
public:
	//*********************消息处理定义开始******************************
	//*********************消息处理定义结束******************************
};

#endif
