#ifndef _WATCH_MSG_HANDLER_H_
#define _WATCH_MSG_HANDLER_H_

#include "../ConfigData/ConfigStruct.h"
#include "RapidXml.h"

enum EProcessStatus
{
	EPS_Init,
	EPS_Stop,
	EPS_Start,
	EPS_ConnSucceed,
	EPS_ConnClose,
	EPS_WaitStart,
};

struct ServerProcessInfo
{
	UINT64			ProcessID;
	UINT32			ConnID;
	INT32			Port;
	INT32			KillAll;
	UINT64			SendTime;
	UINT64			RecvTime;
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

	BOOL        KillWatchProcess();

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsUpdateServerReq(NetPacket* pNetPacket);  //更新服务器
	BOOL OnMsgStartServerReq(NetPacket* pNetPacket);
	BOOL OnMsgStopServerReq(NetPacket* pNetPacket);
	BOOL OnMsgServerHeartAck(NetPacket* pNetPacket);
	BOOL OnMsgWatchWebReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************


protected:

	BOOL BootUpProcessList();

	BOOL CheckProcessStatus(UINT64 uTick);

	BOOL KillProcess(ServerProcessInfo& processData);

	BOOL ReadProcessList();

	BOOL GetProcessBootUpPar(std::string& bootUpPar, std::string& servername, std::string& pars);

	void PrintServerStatus();

	BOOL GetWatchStatus();

	void SetWatchStatus(bool flag);

	bool CanStartServer();

	bool CancloseServer();

private:

	std::vector<ServerProcessInfo> m_vtProcessVec;

	UINT64 m_uCheckProcessTime;

	BOOL m_bStartWatch;
};

#endif //_WATCH_MSG_HANDLER_H_
