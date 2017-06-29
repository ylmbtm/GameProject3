#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "ProxyMsgHandler.h"
class  CConnection;

class CGameService  : public IPacketDispatcher
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

	UINT32		GetLogicConnID();

	BOOL		ConnectToLogicSvr();
public:

public:
	//处理普通的网络连接
	CProxyMsgHandler	m_ProxyMsgHandler;

	//本服务器的信息
	std::string         m_strIpAddr;
	UINT16              m_sPort;

public:
protected:
	UINT32			m_dwLogicConnID;
};

#endif
