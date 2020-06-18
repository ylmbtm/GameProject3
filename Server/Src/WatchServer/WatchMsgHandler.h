#ifndef _WATCH_MSG_HANDLER_H_
#define _WATCH_MSG_HANDLER_H_

#include "RapidXml.h"
#include "WebActionDef.h"
#include "HttpParameter.h"

enum EProcessStatus
{
	EPS_Stop,       //停止状态
	EPS_Stoping,    //正在停止中
	EPS_Starting,   //启动中
	EPS_Connected,  //己连接
	EPS_Checking,   //检索中
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

	BOOL		OnNewConnect(UINT32 nConnID);

	BOOL		OnCloseConnect(UINT32 nConnID);

	BOOL		OnSecondTimer();
public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgServerHeartReq(NetPacket* pNetPacket);
	BOOL OnMsgWebCommandReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************

public:
	//*********************WebAction处理定义开始******************************
	void OnGmServerStart(HttpParameter& hParams, UINT32 nConnID);
	void OnGmServerStop(HttpParameter& hParams, UINT32 nConnID);
	void OnGmServerUpdate(HttpParameter& hParams, UINT32 nConnID);
	void OnGmServerInfo(HttpParameter& hParams, UINT32 nConnID);
	//*********************WebAction处理定义开始******************************

protected:
	BOOL CheckProcessStatus(UINT32 nIndex);

	BOOL StartProcess(ServerProcessInfo& processData, INT32 nIndex);

	BOOL StartAllProcess();

	BOOL KillProcess(ServerProcessInfo& processData);

	BOOL KillProcessByMsg(ServerProcessInfo& processData);

	BOOL KillAllProcess();

	BOOL LoadProcessList();

	void PrintServerStatus();

	BOOL GetStartWatch();

	void SetStartWatch(BOOL bStart);

	BOOL CanStartServer();

	BOOL CanStopServer();

	BOOL SendWebResult(UINT32 nConnID, EWebResult eResult);

private:

	std::vector<ServerProcessInfo> m_vtProcess;

	BOOL m_bStartWatch;
};

#endif //_WATCH_MSG_HANDLER_H_
