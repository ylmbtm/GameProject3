#ifndef _WATCH_MSG_HANDLER_H_
#define _WATCH_MSG_HANDLER_H_

class CWatchMsgHandler
{
public:
	CWatchMsgHandler();

	~CWatchMsgHandler();

	BOOL		Init(UINT32 dwReserved);

	BOOL		Uninit();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL		OnUpdate(UINT64 uTick);


public:
	//*********************消息处理定义开始******************************
	BOOL OnMsUpdateServerReq(NetPacket* pNetPacket);  //更新服务器
	BOOL OnMsgStartServerReq(NetPacket* pNetPacket);
	BOOL OnMsgStopServerReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************




};

#endif //_WATCH_MSG_HANDLER_H_
