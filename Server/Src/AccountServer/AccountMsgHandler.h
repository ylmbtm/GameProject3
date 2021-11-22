#ifndef _ACCOUNT_MSG_HANDLER_H_
#define _ACCOUNT_MSG_HANDLER_H_

#include "AccountManager.h"
class CAccountMsgHandler
{
public:
	CAccountMsgHandler();

	~CAccountMsgHandler();

	BOOL Init(INT32 nReserved);

	BOOL Uninit();

	BOOL DispatchPacket(NetPacket* pNetPacket);

	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgAccountRegReq(NetPacket* pPacket);
	BOOL OnMsgAccontLoginReq(NetPacket* pPacket);
	BOOL OnMsgSealAccountReq(NetPacket* pPacket);
	BOOL OnMsgSetLastServerNty(NetPacket* pPacket);
	//*********************消息处理定义结束******************************

public:

	CAccountObjectMgr m_AccountManager;
};

#endif //_DB_MSG_HANDLER_H_
