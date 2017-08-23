#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IBufferHandler.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include "../ServerEngine/LockFreeQueue.h"
#include "../ServerEngine/CritSec.h"

#define RECV_BUF_SIZE               8192
#define Hash_Map                    std::map

class CConnection
{
public:
	CConnection(boost::asio::io_service& ioservice);
	virtual ~CConnection();

public:
	BOOL	HandleRecvEvent(UINT32 dwBytes);

	UINT32  GetConnectionID();

	UINT64  GetConnectionData();

	VOID    SetConnectionID(UINT32 dwConnID);

	VOID	SetConnectionData(UINT64 dwData);

	BOOL	Close();

	BOOL	SetSocket(SOCKET hSocket);

	boost::asio::ip::tcp::socket&  GetSocket();

	BOOL	SetDataHandler(IDataHandler* pHandler);

	BOOL	ExtractBuffer();

	BOOL	DoReceive();

	BOOL	IsConnectionOK();

	BOOL	SetConnectionOK(BOOL bOk);

	BOOL    Clear();

	BOOL    SendBuffer(IDataBuffer*	pBuff);

	BOOL    SendMessage(UINT32 dwMsgID, UINT64 uTargetID, UINT32 dwUserData, const char* pData, UINT32 dwLen);

	BOOL    DoSend();

	void	HandReaddata(const boost::system::error_code& error, size_t len);
public:
	boost::asio::ip::tcp::socket m_hSocket;

	BOOL						m_bConnected;

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

	volatile BOOL				m_IsSending;

	CConnection*                m_pNext;

	UINT32						m_LastRecvTick;

	ArrayLockFreeQueue < IDataBuffer*, 1 << 10 > m_SendBuffList;
};


class CConnectionMgr
{
private:
	CConnectionMgr();

	~CConnectionMgr();

public:
	static CConnectionMgr* GetInstancePtr();

public:
	BOOL            InitConnectionList(UINT32 nMaxCons, boost::asio::io_service& ioservice);

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