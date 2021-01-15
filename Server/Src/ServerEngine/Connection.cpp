#include "stdafx.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "CommonSocket.h"
#include "PacketHeader.h"

void NetIoOperatorData::Reset()
{
#ifdef WIN32
	memset(&Overlap, 0, sizeof(Overlap));
#endif

	dwOpType = 0;
	dwConnID = 0;

	pDataBuffer = NULL;
}


CConnection::CConnection()
{
	m_hSocket           = INVALID_SOCKET;

	m_pDataHandler      = NULL;

	m_dwDataLen         = 0;

	m_bConnected        = FALSE;

	m_u64ConnData       = 0;

	m_dwConnID          = 0;

	m_pCurRecvBuffer    = NULL;

	m_pBufPos           = m_pRecvBuf;

	m_nCheckNo          = 0;

	m_IsSending         = FALSE;

	m_pSendingBuffer    = NULL;

	m_nSendingPos       = 0;
}

CConnection::~CConnection(void)
{
	Reset();

	m_dwConnID          = 0;

	m_pDataHandler		= NULL;
}

#ifdef WIN32

BOOL CConnection::DoReceive()
{
	WSABUF  DataBuf;

	DataBuf.len = RECV_BUF_SIZE - m_dwDataLen;
	DataBuf.buf = m_pRecvBuf + m_dwDataLen;

	DWORD dwRecvBytes = 0, dwFlags = 0;

	m_IoOverlapRecv.Reset();
	m_IoOverlapRecv.dwOpType = NET_OP_RECV;
	m_IoOverlapRecv.dwConnID = m_dwConnID;

	int nRet = WSARecv(m_hSocket, &DataBuf, 1, &dwRecvBytes, &dwFlags, (LPOVERLAPPED)&m_IoOverlapRecv, NULL);
	if(nRet != 0)
	{
		//对于WSARecv来说， 只要返回0,就表示没有错误发生。
		//当返回为ERROR_IO_PENDING时，表示提交读数据请求成功， 其它的返回值都是错误。
		int nError = CommonSocket::GetSocketLastError();
		if(nError != ERROR_IO_PENDING )
		{
			CLog::GetInstancePtr()->LogError("关闭连接，因为接收数据发生错误:%s!", CommonFunc::GetLastErrorStr(nError).c_str());

			return FALSE;
		}
	}

	//对于WSARecv来说， 只要返回0,就表示没有错误发生。

	return TRUE;
}

#else

BOOL CConnection::DoReceive()
{
	while(TRUE)
	{
		int nBytes = recv(m_hSocket, m_pRecvBuf + m_dwDataLen, RECV_BUF_SIZE - m_dwDataLen, 0);
		if(nBytes < 0)
		{
			int nErr = CommonSocket::GetSocketLastError();
			if( nErr == EAGAIN)
			{
				return TRUE;
			}
			else
			{
				CLog::GetInstancePtr()->LogError("读失败， 可能连接己断开 原因:%s!!", CommonFunc::GetLastErrorStr(nErr).c_str());

				return FALSE;
			}
		}
		else if (nBytes == 0)
		{
			//对方断开连接
			return FALSE;
		}
		else
		{
			m_dwDataLen += nBytes;

			if (m_dwDataLen < RECV_BUF_SIZE)
			{
				return TRUE;
			}

			if(!ExtractBuffer())
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

#endif

UINT32 CConnection::GetConnectionID()
{
	return m_dwConnID;
}

UINT64 CConnection::GetConnectionData()
{
	return m_u64ConnData;
}

void CConnection::SetConnectionID( UINT32 dwConnID )
{
	ERROR_RETURN_NONE(dwConnID != 0);

	ERROR_RETURN_NONE(!m_bConnected);

	m_dwConnID = dwConnID;

	return ;
}

VOID CConnection::SetConnectionData( UINT64 dwData )
{
	ERROR_RETURN_NONE(m_dwConnID != 0);

	m_u64ConnData = dwData;

	return ;
}


BOOL CConnection::ExtractBuffer()
{
	//在这方法里返回FALSE。
	//会在外面导致这个连接被关闭。
	if (m_dwDataLen == 0)
	{
		return TRUE;
	}

	while(TRUE)
	{
		if(m_pCurRecvBuffer != NULL)
		{
			if ((m_pCurRecvBuffer->GetTotalLenth() + m_dwDataLen ) < m_nCurBufferSize)
			{
				memcpy(m_pCurRecvBuffer->GetBuffer() + m_pCurRecvBuffer->GetTotalLenth(), m_pBufPos, m_dwDataLen);
				m_pBufPos = m_pRecvBuf;
				m_pCurRecvBuffer->SetTotalLenth(m_pCurRecvBuffer->GetTotalLenth() + m_dwDataLen);
				m_dwDataLen = 0;
				break;
			}
			else
			{
				memcpy(m_pCurRecvBuffer->GetBuffer() + m_pCurRecvBuffer->GetTotalLenth(), m_pBufPos, m_nCurBufferSize - m_pCurRecvBuffer->GetTotalLenth());
				m_dwDataLen -= m_nCurBufferSize - m_pCurRecvBuffer->GetTotalLenth();
				m_pBufPos += m_nCurBufferSize - m_pCurRecvBuffer->GetTotalLenth();
				m_pCurRecvBuffer->SetTotalLenth(m_nCurBufferSize);
				m_LastRecvTick = CommonFunc::GetTickCount();
				m_pDataHandler->OnDataHandle(m_pCurRecvBuffer, GetConnectionID());
				m_pCurRecvBuffer = NULL;
			}
		}

		if(m_dwDataLen < sizeof(PacketHeader))
		{
			break;
		}

		PacketHeader* pHeader = (PacketHeader*)m_pBufPos;
		//////////////////////////////////////////////////////////////////////////
		//在这里对包头进行检查, 如果不合法就要返回FALSE;
		if (!CheckHeader(m_pBufPos))
		{
			//return FALSE;
		}

		UINT32 dwPacketSize = pHeader->dwSize;

		//////////////////////////////////////////////////////////////////////////
		if((dwPacketSize > m_dwDataLen)  && (dwPacketSize < RECV_BUF_SIZE))
		{
			break;
		}

		if (dwPacketSize <= m_dwDataLen)
		{
			IDataBuffer* pDataBuffer =  CBufferAllocator::GetInstancePtr()->AllocDataBuff(dwPacketSize);

			memcpy(pDataBuffer->GetBuffer(), m_pBufPos, dwPacketSize);

			m_dwDataLen -= dwPacketSize;

			m_pBufPos += dwPacketSize;

			pDataBuffer->SetTotalLenth(dwPacketSize);

			m_LastRecvTick = CommonFunc::GetTickCount();

			m_pDataHandler->OnDataHandle(pDataBuffer, GetConnectionID());
		}
		else
		{
			IDataBuffer* pDataBuffer =  CBufferAllocator::GetInstancePtr()->AllocDataBuff(dwPacketSize);
			memcpy(pDataBuffer->GetBuffer(), m_pBufPos, m_dwDataLen);

			pDataBuffer->SetTotalLenth(m_dwDataLen);
			m_dwDataLen = 0;
			m_pBufPos = m_pRecvBuf;
			m_pCurRecvBuffer = pDataBuffer;
			m_nCurBufferSize = dwPacketSize;
		}
	}

	if(m_dwDataLen > 0)
	{
		memmove(m_pRecvBuf, m_pBufPos, m_dwDataLen);
	}

	m_pBufPos = m_pRecvBuf;

	return TRUE;
}

BOOL CConnection::Close()
{
	CommonSocket::ShutDownSend(m_hSocket);

	CommonSocket::ShutDownRecv(m_hSocket);

	CommonSocket::CloseSocket(m_hSocket);

	m_hSocket           = INVALID_SOCKET;

	m_dwDataLen         = 0;

	m_IsSending			= FALSE;

	if(m_pDataHandler != NULL)
	{
		m_pDataHandler->OnCloseConnect(GetConnectionID());
	}

	m_bConnected = FALSE;

	return TRUE;
}

BOOL CConnection::HandleRecvEvent(UINT32 dwBytes)
{
#ifdef WIN32
	m_dwDataLen += dwBytes;

	if(!ExtractBuffer())
	{
		return FALSE;
	}

	if (!DoReceive())
	{
		return FALSE;
	}
#else
	if (!DoReceive())
	{
		return FALSE;
	}

	if(!ExtractBuffer())
	{
		return FALSE;
	}
#endif
	return TRUE;
}

BOOL CConnection::SetSocket( SOCKET hSocket )
{
	m_hSocket = hSocket;

	return TRUE;
}

BOOL CConnection::SetDataHandler( IDataHandler* pHandler )
{
	ERROR_RETURN_FALSE(pHandler != NULL);

	m_pDataHandler = pHandler;

	return TRUE;
}

SOCKET CConnection::GetSocket()
{
	return m_hSocket;
}

BOOL CConnection::IsConnectionOK()
{
	if((m_hSocket == INVALID_SOCKET) || (m_hSocket == 0))
	{
		return FALSE;
	}

	return m_bConnected;
}

BOOL CConnection::SetConnectionOK( BOOL bOk )
{
	m_bConnected = bOk;

	m_LastRecvTick = CommonFunc::GetTickCount();

	return TRUE;
}


BOOL CConnection::Reset()
{
	m_hSocket = INVALID_SOCKET;

	m_bConnected = FALSE;

	m_u64ConnData = 0;

	m_dwDataLen = 0;

	m_dwIpAddr  = 0;

	m_pBufPos   = m_pRecvBuf;

	m_LastRecvTick = 0;
	if(m_pCurRecvBuffer != NULL)
	{
		m_pCurRecvBuffer->Release();
		m_pCurRecvBuffer = NULL;
	}


	m_nCheckNo = 0;

	m_IsSending	= FALSE;

	IDataBuffer* pBuff = NULL;
	while(m_SendBuffList.try_dequeue(pBuff))
	{
		pBuff->Release();
	}

	return TRUE;
}


BOOL CConnection::SendBuffer(IDataBuffer* pBuff)
{
	return m_SendBuffList.enqueue(pBuff);
}

BOOL CConnection::CheckHeader(CHAR* m_pPacket)
{
	/*
	1.首先验证包的验证吗
	2.包的长度
	3.包的序号
	*/
	PacketHeader* pHeader = (PacketHeader*)m_pBufPos;
	if (pHeader->CheckCode != CODE_VALUE)
	{
		return FALSE;
	}

	if (pHeader->dwSize > 1024 * 1024)
	{
		return FALSE;
	}

	if (pHeader->dwSize <= 0)
	{
		CLog::GetInstancePtr()->LogError("验证-失败 pHeader->dwSize <= 0, pHeader->dwMsgID:%d", pHeader->dwSize, pHeader->dwMsgID);
		return FALSE;
	}

	if (pHeader->dwMsgID > 399999 || pHeader->dwMsgID == 0)
	{
		return FALSE;
	}

	UINT32 dwPktChkNo = pHeader->dwPacketNo - (pHeader->dwMsgID ^ pHeader->dwSize);

	if (dwPktChkNo <= 0)
	{
		return FALSE;
	}

	if(m_nCheckNo == 0)
	{
		m_nCheckNo = dwPktChkNo + 1;
		return TRUE;
	}

	if(m_nCheckNo == dwPktChkNo)
	{
		m_nCheckNo += 1;
		return TRUE;
	}

	return FALSE;
}

UINT32 CConnection::GetIpAddr(BOOL bHost)
{
	if (bHost)
	{
		return m_dwIpAddr;
	}

	return CommonSocket::HostToNet(m_dwIpAddr);
}

#ifdef WIN32
BOOL CConnection::DoSend()
{
	IDataBuffer* pFirstBuff = NULL;
	IDataBuffer* pSendingBuffer = NULL;
	int nSendSize = 0;
	int nCurPos = 0;

	IDataBuffer* pBuffer = NULL;
	while(m_SendBuffList.try_dequeue(pBuffer))
	{
		nSendSize += pBuffer->GetTotalLenth();

		if(pFirstBuff == NULL && pSendingBuffer == NULL)
		{
			pFirstBuff = pBuffer;

			pBuffer = NULL;
		}
		else
		{
			if(pSendingBuffer == NULL)
			{
				pSendingBuffer = CBufferAllocator::GetInstancePtr()->AllocDataBuff(RECV_BUF_SIZE);
				pFirstBuff->CopyTo(pSendingBuffer->GetBuffer() + nCurPos, pFirstBuff->GetTotalLenth());
				pSendingBuffer->SetTotalLenth(pSendingBuffer->GetTotalLenth() + pFirstBuff->GetTotalLenth());
				nCurPos += pFirstBuff->GetTotalLenth();
				pFirstBuff->Release();
				pFirstBuff = NULL;
			}

			pBuffer->CopyTo(pSendingBuffer->GetBuffer() + nCurPos, pBuffer->GetTotalLenth());
			pSendingBuffer->SetTotalLenth(pSendingBuffer->GetTotalLenth() + pBuffer->GetTotalLenth());
			nCurPos += pBuffer->GetTotalLenth();
			pBuffer->Release();
			pBuffer = NULL;
		}

		IDataBuffer** pPeekBuff = m_SendBuffList.peek();
		if (pPeekBuff == NULL)
		{
			break;
		}

		pBuffer = *pPeekBuff;
		if (nSendSize + pBuffer->GetTotalLenth() >= RECV_BUF_SIZE)
		{
			break;
		}

		pBuffer = NULL;
	}

	if(pSendingBuffer == NULL)
	{
		pSendingBuffer = pFirstBuff;
	}

	if(pSendingBuffer == NULL)
	{
		m_IsSending = FALSE;
		return TRUE;
	}

	WSABUF  DataBuf;
	DataBuf.len = pSendingBuffer->GetTotalLenth();
	DataBuf.buf = pSendingBuffer->GetBuffer();
	m_IoOverlapSend.Reset();
	m_IoOverlapSend.dwOpType = NET_OP_SEND;
	m_IoOverlapSend.pDataBuffer = pSendingBuffer;
	m_IoOverlapSend.dwConnID = m_dwConnID;

	DWORD dwSendBytes = 0;
	int nRet = WSASend(m_hSocket, &DataBuf, 1, &dwSendBytes, 0, (LPOVERLAPPED)&m_IoOverlapSend, NULL);
	if(nRet == 0) //发送成功
	{
		//if(dwSendBytes < DataBuf.len)
		//{
		//	CLog::GetInstancePtr()->LogError("发送线程:直接发送数据成功send:%d--Len:%d!", dwSendBytes, DataBuf.len);
		//}
	}
	else if( nRet == -1 ) //发送出错
	{
		UINT32 errCode = CommonSocket::GetSocketLastError();
		if(errCode != ERROR_IO_PENDING)
		{
			Close();
			CLog::GetInstancePtr()->LogError("发送线程:发送失败, 连接关闭原因:%s!", CommonFunc::GetLastErrorStr(errCode).c_str());
		}
	}

	return TRUE;
}

#else
BOOL CConnection::DoSend()
{
	//返回值为正数， 分为完全发送，和部分发送，部分发送，用另一个缓冲区装着继续发送
	//返回值为负数   错误码：
	//
	//if (errno != EAGAIN)
	//{
	//	//ERROR("TcpConnection sendInLoop");
	//	if (errno == EPIPE || errno == ECONNRESET)
	//	{
	//		faultError = true;//这就是真实的错误了
	//	}
	//}
	// #define E_SEND_SUCCESS				1
	// #define E_SEND_UNDONE				2
	// #define E_SEND_ERROR				    3


	if (m_pSendingBuffer != NULL)
	{
		INT32 nRet = send(m_hSocket, m_pSendingBuffer->GetBuffer() + m_nSendingPos, m_pSendingBuffer->GetTotalLenth() - m_nSendingPos, 0);
		if (nRet < (m_pSendingBuffer->GetTotalLenth() - m_nSendingPos))
		{
			if ((nRet < 0) && (errno != EAGAIN))
			{
				m_pSendingBuffer->Release();
				return E_SEND_ERROR;
				//这就表示出错了
			}
			else
			{
				//这就表示发送了一半的数据
				m_nSendingPos += nRet;
				return E_SEND_UNDONE;
			}
		}
		else
		{
			m_pSendingBuffer->Release();
			m_pSendingBuffer = NULL;
			m_nSendingPos = 0;
		}
	}

	IDataBuffer* pBuffer = NULL;
	while(m_SendBuffList.try_dequeue(pBuffer))
	{
		if (pBuffer == NULL)
		{
			return TRUE;
		}

		INT32 nRet = send(m_hSocket, pBuffer->GetBuffer(), pBuffer->GetTotalLenth(), 0);
		if (nRet < pBuffer->GetTotalLenth())
		{
			if ((nRet < 0) && (errno != EAGAIN))
			{
				pBuffer->Release();
				return E_SEND_ERROR;
				//这就表示出错了
			}
			else
			{
				//这就表示发送了一半的数据
				m_pSendingBuffer = pBuffer;
				m_nSendingPos = nRet;
				return E_SEND_UNDONE;
			}
		}
		else
		{
			pBuffer->Release();
		}
	}
	return E_SEND_SUCCESS;
}
#endif

CConnectionMgr::CConnectionMgr()
{
	m_pFreeConnRoot = NULL;
	m_pFreeConnTail = NULL;
}

CConnectionMgr::~CConnectionMgr()
{
	DestroyAllConnection();
	m_pFreeConnRoot = NULL;
	m_pFreeConnTail = NULL;
}

CConnection* CConnectionMgr::CreateConnection()
{
	CConnection* pTemp = NULL;
	m_ConnListMutex.lock();
	if (m_pFreeConnRoot == NULL)
	{
		//表示己到达连接的上限，不能再创建新的连接了
		m_ConnListMutex.unlock();
		return NULL;
	}

	if(m_pFreeConnRoot == m_pFreeConnTail)
	{
		pTemp = m_pFreeConnRoot;
		m_pFreeConnTail = m_pFreeConnRoot = NULL;
	}
	else
	{
		pTemp = m_pFreeConnRoot;
		m_pFreeConnRoot = pTemp->m_pNext;
		pTemp->m_pNext = NULL;
	}
	m_ConnListMutex.unlock();
	ERROR_RETURN_NULL(pTemp->GetConnectionID() != 0);
	ERROR_RETURN_NULL(pTemp->GetSocket() == INVALID_SOCKET);
	ERROR_RETURN_NULL(pTemp->IsConnectionOK() == FALSE);
	return pTemp;
}

CConnection* CConnectionMgr::GetConnectionByID( UINT32 dwConnID )
{
	ERROR_RETURN_NULL(dwConnID != 0);

	UINT32 dwIndex = dwConnID % m_vtConnList.size();

	CConnection* pConnect = m_vtConnList.at(dwIndex == 0 ? (m_vtConnList.size() - 1) : (dwIndex - 1));

	if (pConnect->GetConnectionID() != dwConnID)
	{
		return NULL;
	}

	return pConnect;
}


CConnectionMgr* CConnectionMgr::GetInstancePtr()
{
	static CConnectionMgr ConnectionMgr;

	return &ConnectionMgr;
}


BOOL CConnectionMgr::DeleteConnection(CConnection* pConnection)
{
	ERROR_RETURN_FALSE(pConnection != NULL);

	m_ConnListMutex.lock();

	if(m_pFreeConnTail == NULL)
	{
		ERROR_RETURN_FALSE(m_pFreeConnRoot == NULL);

		m_pFreeConnTail = m_pFreeConnRoot = pConnection;
	}
	else
	{
		m_pFreeConnTail->m_pNext = pConnection;

		m_pFreeConnTail = pConnection;

		m_pFreeConnTail->m_pNext = NULL;

	}

	m_ConnListMutex.unlock();

	UINT32 dwConnID = pConnection->GetConnectionID();

	pConnection->Reset();

	dwConnID += (UINT32)m_vtConnList.size();

	pConnection->SetConnectionID(dwConnID);

	return TRUE;
}

BOOL CConnectionMgr::DeleteConnection(UINT32 nConnID)
{
	ERROR_RETURN_FALSE(nConnID != 0);
	CConnection* pConnection = GetConnectionByID(nConnID);
	ERROR_RETURN_FALSE(pConnection != NULL);

	return DeleteConnection(pConnection);
}

BOOL CConnectionMgr::CloseAllConnection()
{
	CConnection* pConn = NULL;
	for(size_t i = 0; i < m_vtConnList.size(); i++)
	{
		pConn = m_vtConnList.at(i);
		if (!pConn->IsConnectionOK())
		{
			continue;
		}

		if (pConn->m_hSocket == INVALID_SOCKET)
		{
			continue;
		}

		pConn->Close();
	}

	return TRUE;
}

BOOL CConnectionMgr::DestroyAllConnection()
{
	CConnection* pConn = NULL;
	for(size_t i = 0; i < m_vtConnList.size(); i++)
	{
		pConn = m_vtConnList.at(i);
		if (pConn->IsConnectionOK())
		{
			pConn->Close();
		}
		delete pConn;
	}

	m_vtConnList.clear();

	return TRUE;
}

BOOL CConnectionMgr::CheckConntionAvalible()
{
	return TRUE;
	UINT64 curTick = CommonFunc::GetTickCount();

	for(std::vector<CConnection*>::size_type i = 0; i < m_vtConnList.size(); i++)
	{
		CConnection* pConnection = m_vtConnList.at(i);
		if(!pConnection->IsConnectionOK())
		{
			continue;
		}

		if (pConnection->GetConnectionData() == 1)
		{
			continue;
		}

		if (pConnection->m_LastRecvTick <= 0)
		{
			continue;
		}
		if(curTick > (pConnection->m_LastRecvTick + 30000))
		{
			CLog::GetInstancePtr()->LogError("CConnectionMgr::CheckConntionAvalible 超时主动断开连接 ConnID:%d", pConnection->GetConnectionID());
			pConnection->Close();
		}
	}

	return TRUE;
}

BOOL CConnectionMgr::InitConnectionList(UINT32 nMaxCons)
{
	ERROR_RETURN_FALSE(m_pFreeConnRoot == NULL);
	ERROR_RETURN_FALSE(m_pFreeConnTail == NULL);

	m_vtConnList.assign(nMaxCons, NULL);
	for(UINT32 i = 0; i < nMaxCons; i++)
	{
		CConnection* pConn = new CConnection();

		m_vtConnList[i] = pConn;

		pConn->SetConnectionID(i + 1) ;

		if (m_pFreeConnRoot == NULL)
		{
			m_pFreeConnRoot = pConn;
			pConn->m_pNext = NULL;
			m_pFreeConnTail = pConn;
		}
		else
		{
			m_pFreeConnTail->m_pNext = pConn;
			m_pFreeConnTail = pConn;
			m_pFreeConnTail->m_pNext = NULL;
		}
	}

	return TRUE;
}


