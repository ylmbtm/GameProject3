#ifndef _DB_MSG_HANDLER_H_
#define _DB_MSG_HANDLER_H_
#include "DBManager.h"
#include "LockFreeQueue.h"

class CDBMsgHandler
{
public:
	CDBMsgHandler();

	~CDBMsgHandler();

	BOOL AddPacket(NetPacket* pNetPacket);

	BOOL DispatchPacket(NetPacket* pNetPacket);

	BOOL OnUpdate( UINT64 uTick );

	BOOL Init(INT32 nReserved);

	BOOL Run();

	BOOL Uninit();
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgRoleListReq(NetPacket* pPacket);
	BOOL OnMsgRoleLoginReq(NetPacket* pPacket);
	BOOL OnMsgRoleDeleteReq(NetPacket* pPacket);
	BOOL OnMsgExeSqlReq(NetPacket* pPacket);
	BOOL OnMsgLogicSvrRegReq(NetPacket* pPacket);
	//*********************消息处理定义结束******************************

public:
	//数据库定义
	CDBManager							m_DBManager;

	ArrayLockFreeQueue<NetPacket*>		m_PacketQueue;

	std::thread*					   	m_pThread;

	BOOL								m_bRun;
};

#endif //_DB_MSG_HANDLER_H_
