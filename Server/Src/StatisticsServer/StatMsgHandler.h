#ifndef _STAT_MSG_HANDLER_H_
#define _STAT_MSG_HANDLER_H_

class CStatMsgHandler
{
public:
	CStatMsgHandler();

	~CStatMsgHandler();

	BOOL		Init(UINT32 dwReserved);

	BOOL		Uninit();

	BOOL		DispatchPacket( NetPacket *pNetPacket);
	//*********************消息处理定义开始******************************
public:
	//BOOL OnMsgSvrRunningStateReq(NetPacket *pPacket);


	//*********************消息处理定义结束******************************
	//日志处理函数
	//BOOL OnSvrReport_NormalLog(CBufferHelper *pBufferHelper);

};

#endif //_STAT_MSG_HANDLER_H_
