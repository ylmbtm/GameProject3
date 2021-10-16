#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "LogMsgHandler.h"

class CGameService : public IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

	BOOL		Init();

	BOOL		Uninit();

	BOOL		Run();

	BOOL		OnNewConnect(INT32 nConnID);

	BOOL		OnCloseConnect(INT32 nConnID);

	BOOL		OnSecondTimer();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

public:
	CLogMsgHandler		m_LogMsgHandler;

public:
	//*********************消息处理定义开始******************************
	//*********************消息处理定义结束******************************
};

#endif
