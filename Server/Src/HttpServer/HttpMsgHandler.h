#ifndef _HTTP_MSG_HANDLER_H_
#define _HTTP_MSG_HANDLER_H_

class CHttpMsgHandler
{
public:
	CHttpMsgHandler();

	~CHttpMsgHandler();

	BOOL		Init(UINT32 dwReserved);

	BOOL		Uninit();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgPhpWebReq(NetPacket* pNetPacket);

	//*********************消息处理定义结束******************************




};

#endif //_STAT_MSG_HANDLER_H_
