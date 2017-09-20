#ifndef _DB_MSG_HANDLER_H_
#define _DB_MSG_HANDLER_H_
#include "DBManager.h"
#include "CommonThreadFunc.h"

Th_RetName _DBWorkThread(void* pParam);

class CDBMsgHandler
{
public:
	CDBMsgHandler();

	~CDBMsgHandler();

	BOOL AddPacket(NetPacket* pNetPacket);

	BOOL DispatchPacket(NetPacket* pNetPacket);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Run();

	BOOL Uninit();
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgRoleListReq(NetPacket* pPacket);
	BOOL OnMsgRoleLoginReq(NetPacket* pPacket);
	//*********************消息处理定义结束******************************

public:
	//数据库定义
	CDBManager							m_DBManager;

	ArrayLockFreeQueue<NetPacket*>		m_PacketQueue;

	THANDLE								m_hThread;

	BOOL								m_bRun;
};

#endif //_DB_MSG_HANDLER_H_
