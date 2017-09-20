#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
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

	BOOL		ConnectToCenterSvr();

	BOOL		RegisterToLoginSvr();

	BOOL		RegisterToCenterSvr();

	UINT32      GetDBConnID();

	UINT32      GetLoginConnID();

	UINT32      GetServerID();

	UINT32      GetCenterID();

public:
	CLogicMsgHandler m_LogicMsgHandler;

	UINT32			m_dwLogConnID;
	UINT32			m_dwLoginConnID;
	UINT32			m_dwDBConnID;
	UINT32          m_dwCenterID;   //中心服的连接ID
};

#endif
