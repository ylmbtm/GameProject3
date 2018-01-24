#ifndef _WATCH_MSG_HANDLER_H_
#define _WATCH_MSG_HANDLER_H_

#include "../ConfigData/ConfigStruct.h"
#include "RapidXml.h"

enum EProcessStatus
{
	EPS_Init,  //状态也没有
	EPS_Start, //己经执行了启动
	EPS_ConnSucceed,
};

struct ServerProcessInfo
{
	UINT64			ProcessID;
	UINT32			ConnID;
	INT32			Port;
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

	BOOL		OnNewConnect(CConnection* pConn);

	BOOL		OnCloseConnect(CConnection* pConn);
public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgUpdateServerReq(NetPacket* pNetPacket);  //更新服务器
	BOOL OnMsgStartServerReq(NetPacket* pNetPacket);
	BOOL OnMsgStopServerReq(NetPacket* pNetPacket);
	BOOL OnMsgServerHeartAck(NetPacket* pNetPacket);
	BOOL OnMsgGmCommandReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************


protected:

	BOOL BootUpProcessList();

	BOOL CheckProcessStatus(UINT64 uTick, UINT32 nIndex);

	BOOL KillProcess(ServerProcessInfo& processData);

	BOOL KillAllProcess();

	BOOL LoadProcessList();

	void PrintServerStatus();

	BOOL GetStartWatch();

	void SetStartWatch(BOOL bStart);

	bool CanStartServer();

	bool CancloseServer();

private:

	std::vector<ServerProcessInfo> m_vtProcess;

	UINT64 m_uLaskTick;

	BOOL m_bStartWatch;
};

#endif //_WATCH_MSG_HANDLER_H_
