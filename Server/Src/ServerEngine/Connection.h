#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IBufferHandler.h"
#include "ReaderWriterQueue.h"

#define  NET_OP_RECV				1
#define  NET_OP_SEND				2
#define  NET_OP_CONNECT				3
#define  NET_OP_ACCEPT				4
#define  NET_OP_POST				5
#define  NET_OP_UDP_RECV            6

#define RECV_BUF_SIZE               8192
#define MAX_BUFF_SIZE				32768

#define E_SEND_SUCCESS				1
#define E_SEND_UNDONE				2
#define E_SEND_ERROR				3

struct NetIoOperatorData
{
#ifdef WIN32
	OVERLAPPED		Overlap;
#endif
	UINT32			dwOpType;
	UINT32			dwConnID;

	IDataBuffer*	pDataBuffer;

	void			Reset();
};

class CConnection
{
public:
	CConnection();
	virtual ~CConnection();

public:
	BOOL	HandleRecvEvent(UINT32 dwBytes);

	UINT32  GetConnectionID();

	UINT64  GetConnectionData();

	VOID    SetConnectionID(UINT32 dwConnID);

	VOID	SetConnectionData(UINT64 dwData);

	BOOL	Close();

	BOOL	SetSocket(SOCKET hSocket);

	SOCKET  GetSocket();

	BOOL	SetDataHandler(IDataHandler* pHandler);

	BOOL	ExtractBuffer();

	BOOL	DoReceive();

	BOOL	IsConnectionOK();

	BOOL	SetConnectionOK(BOOL bOk);

	BOOL    Reset();

	BOOL    SendBuffer(IDataBuffer*	pBuff);

	BOOL    DoSend();

	BOOL	CheckHeader(CHAR* m_pPacket);

	UINT32  GetIpAddr(BOOL bHost = TRUE);

public:
	SOCKET                      m_hSocket;

	BOOL                        m_bConnected;

	NetIoOperatorData           m_IoOverlapRecv;

	NetIoOperatorData			m_IoOverlapSend;

	NetIoOperatorData			m_IoOverLapPost;

	UINT32                      m_dwConnID;
	UINT64                      m_u64ConnData;

	IDataHandler*				m_pDataHandler;

	UINT32						m_dwIpAddr;

	UINT32						m_dwDataLen;
	CHAR						m_pRecvBuf[RECV_BUF_SIZE];
	CHAR*						m_pBufPos;

	IDataBuffer*				m_pCurRecvBuffer;
	UINT32						m_nCurBufferSize;
	UINT32						m_nCheckNo;

	volatile BOOL				m_IsSending;

	CConnection*                m_pNext;

	UINT64						m_LastRecvTick;

	moodycamel::ReaderWriterQueue< IDataBuffer*> m_SendBuffList;

	//LINUX下专用， 用于发了一半的包
	IDataBuffer*				m_pSendingBuffer;
	UINT32						m_nSendingPos;
};


class CConnectionMgr
{
private:
	CConnectionMgr();

	~CConnectionMgr();

public:
	static CConnectionMgr* GetInstancePtr();

public:
	BOOL            InitConnectionList(UINT32 nMaxCons);

	CConnection*    CreateConnection();

	BOOL		    DeleteConnection(CConnection* pConnection);

	BOOL            DeleteConnection(UINT32 nConnID);

	CConnection*    GetConnectionByID(UINT32 dwConnID);

	///////////////////////////////////////////
	BOOL		    CloseAllConnection();

	BOOL		    DestroyAllConnection();

	BOOL			CheckConntionAvalible();

public:

	CConnection*				m_pFreeConnRoot;
	CConnection*				m_pFreeConnTail;
	std::vector<CConnection*>	m_vtConnList;            //连接列表
	std::mutex					m_ConnListMutex;
};

#endif