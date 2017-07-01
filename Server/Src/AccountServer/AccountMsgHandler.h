#ifndef _ACCOUNT_MSG_HANDLER_H_
#define _ACCOUNT_MSG_HANDLER_H_

#include "DBManager.h"
#include "AccountManager.h"
class CAccountMsgHandler
{
public:
	CAccountMsgHandler();

	~CAccountMsgHandler();

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	BOOL DispatchPacket(NetPacket *pNetPacket);

	BOOL OnUpdate( UINT32 dwTick );

	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgAccountRegReq(NetPacket *pPacket);
	BOOL OnMsgAccontLoginReq(NetPacket *pPacket);
	//*********************消息处理定义结束******************************

public:

	//数据库定义
	CDBManager   m_DBManager;

	CAccountObjectMgr m_AccountManager;
};

#endif //_DB_MSG_HANDLER_H_
