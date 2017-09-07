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

	BOOL		DispatchPacket(NetPacket* pNetPacket);

	BOOL		OnNewConnect(CConnection* pConn);

	BOOL		OnCloseConnect(CConnection* pConn);
public:
	BOOL		RelayToGameServer(CProxyPlayer* pClientObj, IDataBuffer* pBuffer);

	BOOL		RelayToLogicServer(IDataBuffer* pBuffer);

	BOOL		RelayToClient(CProxyPlayer* pClientObj, IDataBuffer* pBuffer);

	BOOL        RelayToConnect(UINT32 dwConnID, IDataBuffer* pBuffer);

	UINT32      GetGameSvrConnID(UINT32 dwSvrID);

	BOOL		IsServerConnID(UINT32 dwConnID);
	//*********************消息处理定义开始******************************
public:
	BOOL		OnMsgGameSvrRegister(NetPacket* pPacket);
	BOOL		OnMsgEnterSceneReq(NetPacket* pPacket);
	BOOL		OnMsgBroadMessageNty(NetPacket* pPacket);
public:
	std::map<UINT32, UINT32> m_mapSvrIDtoConnID;

};

#endif //_PROXY_SERVER_MSG_HANDLER_H_
