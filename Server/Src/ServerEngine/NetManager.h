#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_
#include "IBufferHandler.h"
#include "CommonThreadFunc.h"
#include "CommonMsgQueue.h"

Th_RetName _NetEventThread( void* pParam );

Th_RetName _NetListenThread( void* pParam );

Th_RetName _NetEventDispatchThread(void* pParam ); //only for linux

////以下为linux专有//////////////////////////////
#define EVENT_READ  1
#define EVENT_WRITE 2

struct EventNode
{
	UINT32		dwEvent;
	void*		pPtr;
};

////////////////////////////////////////////////

class CNetManager
{
	CNetManager(void);

	virtual ~CNetManager(void);
public:
	static CNetManager* GetInstancePtr()
	{
		static CNetManager NetManager;

		return &NetManager;
	}
public:
	BOOL	Start(UINT16 nPortNum,  UINT32 nMaxConn, IDataHandler* pBufferHandler);

	BOOL	Close();

	BOOL	SendMessageByConnID(UINT32 dwConnID,  UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData,  const char* pData, UINT32 dwLen);

	BOOL    SendMsgBufByConnID(UINT32 dwConnID, IDataBuffer* pBuffer);
public:
	BOOL	InitNetwork();

	BOOL	UninitNetwork();

	BOOL	StartListen(UINT16 nPortNum);

	BOOL	StopListen();

	//以下是完成端口部分
public:
	BOOL	CreateCompletePort();

	CConnection*	AssociateCompletePort(SOCKET hSocket);

	CConnection*	ConnectToOtherSvr(std::string strIpAddr, UINT16 sPort);

	CConnection*	ConnectToOtherSvrEx(std::string strIpAddr, UINT16 sPort);

	BOOL	DestroyCompletePort();

	BOOL	CreateDispatchThread();

	BOOL    CloseDispatchThread();

	BOOL	CreateEventThread(int nNum);

	BOOL    CloseEventThread();

	BOOL	WorkThread_DispathEvent();

	BOOL	WorkThread_ProcessEvent();

	BOOL	WorkThread_Listen();

	SOCKET				m_hListenSocket;

	HANDLE				m_hCompletePort;

	BOOL				m_bCloseSend;		//是否关闭发送数据的线程

	BOOL				m_bCloseEvent;		//是否关闭事件处理线程

	BOOL				m_bCloseDispath;	//是否关闭分发线程

	IDataHandler*		m_pBufferHandler;
public:
	CommonQueue::CMessageQueue<EventNode>	m_DispatchEventList;

	THANDLE				 m_hListenThread;
	THANDLE				 m_hDispathThread;
	std::vector<THANDLE> m_vtEventThread;

#ifndef WIN32

	static void SignalHandler(int nValue)
	{

	}

	BOOL  ClearSignal()
	{
		m_NewAct.sa_handler = CNetManager::SignalHandler;

		sigemptyset(&m_NewAct.sa_mask); //清空此信号集

		m_NewAct.sa_flags = 0;

		sigaction(SIGPIPE, &m_NewAct, &m_OldAct);

		return TRUE;
	}

	BOOL RestoreSignal()
	{
		sigaction(SIGPIPE, &m_OldAct, NULL); //恢复成原始状态

		return TRUE;
	}

	struct sigaction m_NewAct, m_OldAct;

#endif

};

#endif

