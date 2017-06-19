#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "WorldMsgHandler.h"

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

	BOOL		OnNewConnect(CConnection *pConn);

	BOOL		OnCloseConnect(CConnection *pConn);

	BOOL		DispatchPacket( NetPacket *pNetPacket);

	BOOL        SendCmdToDBConnection(IDataBuffer *pBuffer);

	BOOL	    ConnectToStatServer();

	BOOL		ConnectToLoginSvr();

	BOOL		ConnectToDBSvr();

	BOOL		ConnectToProxySvr();

	BOOL		RegisterToLoginSvr();

	BOOL		OnTimer(UINT32 dwUserData);

public:
	CWorldMsgHandler		m_WorldMsgHandler;

	CConnection*			m_pStatSvrConn;
	CConnection*			m_pLoginSvrConn;
	CConnection*			m_pDBServerConn;
	CConnection*			m_pProxySvrConn;
};

#endif
