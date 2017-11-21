#ifndef _WATCH_MSG_HANDLER_H_
#define _WATCH_MSG_HANDLER_H_

enum ProcessStatus
{
	stop,
	start,
	connSucceed,
	connClose,
	waitStart,
};

struct ServerProcessInfo
{
	UINT64			ProcessID;
	INT32			ConnID;
	INT32			Port;
	INT32			KillAll;
	UINT64			SendTime;
	UINT64			RecvTime;
	std::string		Params;
	std::string		serverName;
	std::string		BootUpParameter;
	ProcessStatus	ProscessStatus;
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
	BOOL OnMsUpdateServerReq(NetPacket* pNetPacket);  //更新服务器
	BOOL OnMsgStartServerReq(NetPacket* pNetPacket);
	BOOL OnMsgStopServerReq(NetPacket* pNetPacket);
	BOOL OnMsgServerHeartAck(NetPacket* pNetPacket);
	BOOL OnMsgWatchWebReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************


protected:

	BOOL BootUpProcessList();

	BOOL CheckProcessStatus(INT64 uTick);

	BOOL KillProcess(ServerProcessInfo& processData);

	BOOL ReadProcessList();

	BOOL GetProcessBootUpPar(std::string& bootUpPar, std::string& servername, std::string& pars);

	void PrintServerStatus();

private:

	std::vector<ServerProcessInfo> m_vtProcessVec;

	UINT64 m_uCheckProcessTime;
};

#endif //_WATCH_MSG_HANDLER_H_
