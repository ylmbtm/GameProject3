#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IBufferHandler.h"
#include "CritSec.h"
#include "LockFreeQueue.h"

#define  NET_MSG_RECV				1
#define  NET_MSG_SEND				2
#define  NET_MSG_CONNECT			3

#define RECV_BUF_SIZE               8192

#define Hash_Map                    std::map

struct NetIoOperatorData
{
#ifdef WIN32
	OVERLAPPED		Overlap;
#endif
	UINT32			dwCmdType;
	UINT32			dwConnID;

	IDataBuffer*	pDataBuffer;

	void			Clear();
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

	BOOL    Clear();

	BOOL    SendBuffer(IDataBuffer*	pBuff);

	BOOL    SendMessage(UINT32 dwMsgID, UINT64 uTargetID, UINT32 dwUserData, const char* pData, UINT32 dwLen);

	BOOL    DoSend(IDataBuffer* pBuff);

	BOOL	CheckHeader(CHAR* m_pPacket);

public:
	SOCKET						m_hSocket;

	BOOL						m_bConnected;

	NetIoOperatorData			m_IoOverlapRecv;

	NetIoOperatorData			m_IoOverlapSend;

	UINT32                      m_dwConnID;
	UINT64                      m_u64ConnData;

	IDataHandler*				m_pDataHandler;

	UINT32						m_dwIpAddr;

	UINT32						m_dwDataLen;
	CHAR						m_pRecvBuf[RECV_BUF_SIZE];
	CHAR*						m_pBufPos;

	IDataBuffer*				m_pCurRecvBuffer;
	UINT32						m_pCurBufferSize;
	UINT32						m_nCheckNo;

	CConnection*                m_pNext;

	UINT64						m_LastRecvTick;

	ArrayLockFreeQueue < IDataBuffer*, 1 << 10 > m_SendBuffList;

	BOOL				        m_IsSending;
	CCritSec                    mCritSending;

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

	CConnection*    GetConnectionByConnID(UINT32 dwConnID);

	///////////////////////////////////////////
	BOOL		    CloseAllConnection();

	BOOL		    DestroyAllConnection();

	BOOL			CheckConntionAvalible();

public:

	CConnection*				m_pFreeConnRoot;
	CConnection*				m_pFreeConnTail;
	std::vector<CConnection*>	m_vtConnList;            //连接列表
	CCritSec					m_CritSecConnList;
};

#endif