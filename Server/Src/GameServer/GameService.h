#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "SceneManager.h"

class CGameService : public IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

	BOOL		Init(UINT32 dwServerID, UINT32 dwPort);

	BOOL		Uninit();

	BOOL		Run();

	BOOL		OnNewConnect(UINT32 nConnID);

	BOOL		OnCloseConnect(UINT32 nConnID);

	BOOL		OnSecondTimer();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL		SetLogicConnID(UINT32 dwConnID);

	UINT32		GetLogicConnID();

	UINT32      GetProxyConnID();

	BOOL		ConnectToLogicSvr();

	BOOL		ConnectToProxySvr();

	BOOL		ConnectToWatchServer();

	BOOL		RegisterToLogicSvr();

	BOOL		RegisterToProxySvr();

	UINT32      GetServerID();

	BOOL		SendWatchHeartBeat();

	BOOL		SetWatchIndex(UINT32 nIndex);
protected:
	CSceneManager		m_SceneManager;

	UINT32				m_dwLogicConnID;
	UINT32              m_dwProxyConnID;
	UINT32				m_dwServerID;
	UINT32				m_dwWatchSvrConnID;
	UINT32				m_dwWatchIndex;
public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgDefautReq(NetPacket* pNetPacket);
	BOOL OnMsgRegToProxyAck(NetPacket* pNetPacket);
	BOOL OnMsgWatchHeartBeatAck(NetPacket* pNetPacket);
	BOOL OnMsgGmStopServerReq(NetPacket* pNetPacket);
	BOOL OnMsgWebCommandReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif
