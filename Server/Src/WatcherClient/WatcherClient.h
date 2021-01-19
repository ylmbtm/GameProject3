#ifndef _WATCHER_CLIENT_H_
#define _WATCHER_CLIENT_H_

class CWatcherClient
{
private:
	CWatcherClient(void);
	virtual ~CWatcherClient(void);

public:
	static CWatcherClient* GetInstancePtr();

	BOOL		OnNewConnect(UINT32 nConnID);

	BOOL		OnCloseConnect(UINT32 nConnID);

	BOOL		OnSecondTimer();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL		ConnectToWatchServer();

	BOOL		SendWatchHeartBeat();

	BOOL		SetWatchIndex(UINT32 nIndex);

	BOOL        IsRun();

	BOOL        StopServer();

	UINT32					m_dwWatchSvrConnID;
	UINT32					m_dwWatchIndex;
	UINT64                  m_uLastHeartTime;
	BOOL                    m_bRun;
public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgWatchHeartBeatAck(NetPacket* pNetPacket);
	BOOL OnMsgGmStopServerReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif //_WATCHER_CLIENT_H_
