#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IBufferHandler.h"
#include "ReaderWriterQueue.h"

#define RECV_BUF_SIZE               8192


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

	uv_tcp_t*  GetSocket();

	BOOL	SetDataHandler(IDataHandler* pHandler);

	BOOL	ExtractBuffer();

	BOOL	DoReceive();

	BOOL	IsConnectionOK();

	BOOL	SetConnectionOK(BOOL bOk);

	BOOL    Reset();

	BOOL    SendBuffer(IDataBuffer*	pBuff);

	BOOL    DoSend();

	void	HandReaddata(size_t len);

	void	HandWritedata(size_t len);

	BOOL	CheckHeader(CHAR* m_pPacket);

	UINT32  GetIpAddr(BOOL bHost = TRUE);

	VOID    EnableCheck(BOOL bCheck);
public:
	uv_tcp_t					m_hSocket;
	uv_connect_t				m_ConnectReq;
	uv_write_t					m_WriteReq;
	uv_shutdown_t				m_ShutdownReq;
	uv_async_t					m_AsyncReq;

	BOOL						m_bConnected;

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

	IDataBuffer*				m_pSendingBuffer;


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

	BOOL			CheckConntionAvalible(INT32 nInterval);

public:

	CConnection*				m_pFreeConnRoot;
	CConnection*				m_pFreeConnTail;
	std::vector<CConnection*>	m_vtConnList;            //连接列表
	std::mutex					m_ConnListMutex;
};

#endif