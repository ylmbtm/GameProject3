#ifndef _PROXY_SERVER_MSG_HANDLER_H_
#define _PROXY_SERVER_MSG_HANDLER_H_
#include "CommonMsgHandler.h"

class  CProxyPlayer;

class CProxyMsgHandler
{
public:
	CProxyMsgHandler();

	~CProxyMsgHandler();

	BOOL		Init(UINT32 dwReserved);

	BOOL		Uninit();

	BOOL		DispatchPacket(NetPacket *pNetPacket);

	BOOL		OnNewConnect(CConnection *pConn);

	BOOL		OnCloseConnect(CConnection *pConn);
public:
	BOOL		RelayToGameServer(CProxyPlayer *pClientObj, IDataBuffer *pBuffer);

	BOOL		RelayToLogicServer(IDataBuffer *pBuffer);

	BOOL		RelayToClient(CProxyPlayer *pClientObj, IDataBuffer *pBuffer);

	BOOL        RelayToConnect(UINT32 dwConnID, IDataBuffer *pBuffer);

	//*********************消息处理定义开始******************************
public:

};

#endif //_PROXY_SERVER_MSG_HANDLER_H_
