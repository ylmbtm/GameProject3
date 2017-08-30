#ifndef _CENTER_MSG_HANDLER_H_
#define _CENTER_MSG_HANDLER_H_

class CCenterMsgHandler
{
public:
	CCenterMsgHandler();

	~CCenterMsgHandler();

	BOOL		Init(UINT32 dwReserved);

	BOOL		Uninit();

	BOOL		DispatchPacket( NetPacket* pNetPacket);
	//*********************消息处理定义开始******************************
public:


};

#endif //_CENTER_MSG_HANDLER_H_
