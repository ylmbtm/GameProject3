#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "LogicMsgHandler.h"

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

	BOOL		OnNewConnect(CConnection* pConn);

	BOOL		OnCloseConnect(CConnection* pConn);

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL        SendCmdToDBConnection(IDataBuffer* pBuffer);

	BOOL	    ConnectToLogServer();

	BOOL		ConnectToLoginSvr();

	BOOL		ConnectToDBSvr();

	BOOL		RegisterToLoginSvr();

	BOOL		OnTimer(UINT32 dwUserData);

	UINT32      GetDBConnID();

	UINT32      GetLoginConnID();

	UINT32      GetServerID();

public:
	CLogicMsgHandler m_LogicMsgHandler;

	UINT32			m_dwLogConnID;
	UINT32			m_dwLoginConnID;
	UINT32			m_dwDBConnID;
};

#endif
