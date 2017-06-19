#ifndef _DB_MSG_HANDLER_H_
#define _DB_MSG_HANDLER_H_
#include "CommonMsgHandler.h"
#include "DBProcManager.h"
#include "DBConnection.h"
#include "DBStoredProcMgr.h"


class CDBMsgHandler : public CCommonMsgHandler
{
public:
	CDBMsgHandler();

	~CDBMsgHandler();

	BOOL DispatchPacket(NetPacket *pNetPacket);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	BOOL OnThreadBegin();

	BOOL OnThreadEnd();

	BOOL OnCommandHandle(UINT32 dwMsgID, UINT64 u64ConnID, IDataBuffer *pDataBuffer);
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgRoleListReq(NetPacket *pPacket);
	BOOL OnMsgCreateRoleReq(NetPacket *pPacket);
	BOOL OnMsgDeleteRoleReq(NetPacket *pPacket);
	BOOL OnMsgRoleLoginReq(NetPacket *pPacket);
	
	//*********************消息处理定义结束******************************

public:
	//真实的mysql
	CDBConnection   m_DBConnection;

	CDBStoredProcedureMgr m_DBProceduceMgr;

	//数据库定义
	CDBProcManager   m_DBProcManager;
};

#endif //_DB_MSG_HANDLER_H_
