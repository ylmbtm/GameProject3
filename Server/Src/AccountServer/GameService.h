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

	BOOL		ConnectToWatchServer();

	BOOL		SendWatchHeartBeat();

	BOOL		SetWatchIndex(UINT32 nIndex);
public:
	CAccountMsgHandler		m_AccountMsgHandler;
	UINT32					m_dwLogSvrConnID;

	UINT32					m_dwWatchSvrConnID;
	UINT32					m_dwWatchIndex;
public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgWatchHeartBeatAck(NetPacket* pNetPacket);
	BOOL OnMsgGmStopServerReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif
