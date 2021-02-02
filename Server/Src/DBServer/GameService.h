#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "DBMsgHandler.h"
#include "DBWriterManager.h"
class CConnection;

#define DB_THREAD_NUM 10

class CGameService  : public IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

public:
	BOOL		Init();

	BOOL		Uninit();

	BOOL		Run();

	BOOL		OnNewConnect(UINT32 nConnID);

	BOOL		OnCloseConnect(UINT32 nConnID);

	BOOL		OnSecondTimer();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL        SetLogicConnID(UINT32 dwConnID);

	UINT32      GetLogicConnID();

	BOOL        SetLogicProcessID(UINT32 dwProcesssID);

	UINT32      GetLogicProcessID();

	BOOL        CheckLogicServer();

public:
	CDBMsgHandler		m_DBMsgHandler;

	CDBWriterManager    m_DBWriterManger;

	UINT32              m_dwLogicConnID;

	UINT32              m_dwLogicProcessID;
public:
	//*********************消息处理定义开始******************************
	//*********************消息处理定义结束******************************
};

#endif
