#ifndef _WATCH_MSG_HANDLER_H_
#define _WATCH_MSG_HANDLER_H_

#include "../StaticData/StaticStruct.h"
#include "RapidXml.h"

enum EProcessStatus
{
	EPS_Stop,  //停止状态
	EPS_Starting,//启动中
	EPS_Connected, //己连接
};

struct ServerProcessInfo
{
	UINT64			ProcessID;
	UINT32			ConnID;
	INT32			KillAll;
	UINT64			LastHeartTick;
	std::string		Params;
	std::string		serverName;
	std::string		BootUpParameter;
	EProcessStatus	ProscessStatus;
};

class CWatchMsgHandler
{
public:
	CWatchMsgHandler();

	~CWatchMsgHandler();

	BOOL		Init(UINT32 dwReserved);

	BOOL		Uninit();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL		OnUpdate(UINT64 uTick);

	BOOL		OnNewConnect(UINT32 nConnID);

	BOOL		OnCloseConnect(UINT32 nConnID);

	BOOL		OnSecondTimer();
public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgUpdateServerReq(NetPacket* pNetPacket);  //更新服务器
	BOOL OnMsgStartServerReq(NetPacket* pNetPacket);
	BOOL OnMsgStopServerReq(NetPacket* pNetPacket);
	BOOL OnMsgServerHeartReq(NetPacket* pNetPacket);
	BOOL OnMsgWebCommandReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************

	BOOL UpdateServer_Thread();

protected:
	BOOL CheckProcessStatus(UINT64 uTick, UINT32 nIndex);

	BOOL StartProcess(ServerProcessInfo& processData, INT32 nIndex);

	BOOL StartAllProcess();

	BOOL KillProcess(ServerProcessInfo& processData);

	BOOL KillAllProcess();

	BOOL LoadProcessList();

	void PrintServerStatus();

	BOOL GetStartWatch();

	void SetStartWatch(BOOL bStart);

	BOOL CanStartServer();

	BOOL CanStopServer();

private:

	std::vector<ServerProcessInfo> m_vtProcess;

	UINT64 m_uLaskTick;

	BOOL m_bStartWatch;
};

#endif //_WATCH_MSG_HANDLER_H_
