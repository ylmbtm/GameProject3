#ifndef _ACCOUNT_MSG_HANDLER_H_
#define _ACCOUNT_MSG_HANDLER_H_

#include "DBProcManager.h"
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
	CDBProcManager   m_DBProcManager;
};

#endif //_DB_MSG_HANDLER_H_
