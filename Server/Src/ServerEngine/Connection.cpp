#include "stdafx.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "CommonSocket.h"
#include "CommandDef.h"
#include "CommonFunc.h"
#include "PacketHeader.h"
#include "Log.h"

void NetIoOperatorData::Clear()
{
#ifdef WIN32
	memset(&Overlap, 0, sizeof(Overlap));
#endif

	dwCmdType = 0;
	dwConnID = 0;

	pDataBuffer = NULL;
}


CConnection::CConnection()
{
	m_hSocket			= INVALID_SOCKET;

	m_pDataHandler		= NULL;

	m_dwDataLen			= 0;

	m_bConnected		= FALSE;

	m_u64ConnData        = 0;

	m_dwConnID          = 0;

	m_pCurRecvBuffer    = NULL;

	m_pBufPos           = m_pRecvBuf;

	m_nCheckNo          = 0;

	m_IsSending			= FALSE;
}

CConnection::~CConnection(void)
{
	m_hSocket           = INVALID_SOCKET;

	m_pDataHandler		= NULL;

	m_dwDataLen			= 0;

	m_u64ConnData        = 0;

	m_dwConnID          = 0;

	m_bConnected		= FALSE;

	m_pCurRecvBuffer    = NULL;

	m_pBufPos           = m_pRecvBuf;

	m_nCheckNo          = 0;

	m_IsSending			= FALSE;
}

#ifdef WIN32

BOOL CConnection::DoReceive()
{
	WSABUF  DataBuf;

	DataBuf.len = RECV_BUF_SIZE - m_dwDataLen;
	DataBuf.buf = m_pRecvBuf + m_dwDataLen;

	DWORD dwRecvBytes = 0, dwFlags = 0;

	m_IoOverlapRecv.Clear();
	m_IoOverlapRecv.dwCmdType = NET_MSG_RECV;
	m_IoOverlapRecv.dwConnID = m_dwConnID;

	int nRet = WSARecv(m_hSocket, &DataBuf, 1, &dwRecvBytes, &dwFlags, (LPOVERLAPPED)&m_IoOverlapRecv, NULL);
	if(nRet != 0)
	{
		//对于WSARecv来说， 只要返回0,就表示没有错误发生。
		//当返回为ERROR_IO_PENDING时，表示提交读数据请求成功， 其它的返回值都是错误。
		int nError = CommonSocket::GetSocketLastError();
		if(nError != ERROR_IO_PENDING )
		{
			CLog::GetInstancePtr()->LogError("关闭连接，因为接收数据发生错误:%s!", CommonSocket::GetLastErrorStr(nError).c_str());

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
		if(nBytes == 0)
		{
			if(m_dwDataLen == RECV_BUF_SIZE)
			{
				CLog::GetInstancePtr()->LogError("buffer满了，需要再读一次!!");

				if(!ExtractBuffer())
				{
					return FALSE;
				}

				continue;
			}

			CLog::GetInstancePtr()->LogError("收到数据为0， 判断是对方关闭################!!");

			return FALSE;
		}
		else if(nBytes < 0)
		{
			int nErr = CommonSocket::GetSocketLastError();
			if( nErr == EAGAIN)
			{
				CLog::GetInstancePtr()->LogError("读成功了，缓冲区己经无数据可读!!");

				return TRUE;
			}
			else
			{
				CLog::GetInstancePtr()->LogError("读失败， 可能连接己断开 原因:%s!!", CommonSocket::GetLastErrorStr(nErr).c_str());

				return FALSE;
			}
		}
		else
		{
			m_dwDataLen += nBytes;

			if(!ExtractBuffer())
			{
				return FALSE;
			}

			continue;
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
	ASSERT(dwConnID != 0);
	ASSERT(!m_bConnected);

	m_dwConnID = dwConnID;

	return ;
}

VOID CConnection::SetConnectionData( UINT64 dwData )
{
	ASSERT(m_dwConnID != 0);
	m_u64ConnData = dwData;

	return ;
}


BOOL CConnection::ExtractBuffer()
{
	//在这方法里返回FALSE。
	//会在外面导致这个连接被关闭。

	ERROR_RETURN_TRUE(m_dwDataLen > 0);

	while(TRUE)
	{
		if(m_pCurRecvBuffer != NULL)
		{
			if ((m_pCurRecvBuffer->GetTotalLenth() + m_dwDataLen ) < m_pCurBufferSize)
			{
				memcpy(m_pCurRecvBuffer->GetBuffer() + m_pCurRecvBuffer->GetTotalLenth(), m_pBufPos, m_dwDataLen);
				m_dwDataLen = 0;
				m_pBufPos = m_pRecvBuf;
				m_pCurRecvBuffer->SetTotalLenth(m_pCurRecvBuffer->GetTotalLenth() + m_dwDataLen);
				break;
			}
			else
			{
				memcpy(m_pCurRecvBuffer->GetBuffer() + m_pCurRecvBuffer->GetTotalLenth(), m_pBufPos, m_pCurBufferSize - m_pCurRecvBuffer->GetTotalLenth());
				m_dwDataLen -= m_pCurBufferSize - m_pCurRecvBuffer->GetTotalLenth();
				m_pBufPos += m_pCurBufferSize - m_pCurRecvBuffer->GetTotalLenth();
				m_pCurRecvBuffer->SetTotalLenth(m_pCurBufferSize);
				m_pDataHandler->OnDataHandle(m_pCurRecvBuffer, this);
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
		/*
		1.首先验证包的验证吗
		2.包的长度
		3.包的序号*/
		if(pHeader->CheckCode != 0x88)
		{
			return FALSE;
		}

		if(pHeader->dwSize > 1024 * 1024)
		{
			return FALSE;
		}

		if(pHeader->dwMsgID > 4999999)
		{
			return FALSE;
		}
		/*if(m_nCheckNo == 0)
		{
			m_nCheckNo = pHeader->dwPacketNo - pHeader->wCommandID^pHeader->dwSize;
		}
		else
		{
			if(pHeader->dwPacketNo = pHeader->wCommandID^pHeader->dwSize+m_nCheckNo)
			{
				m_nCheckNo += 1;
			}
			else
			{
				return FALSE;
			}*/



		ERROR_RETURN_FALSE(pHeader->dwSize != 0);

		UINT32 dwPacketSize = pHeader->dwSize;

		//////////////////////////////////////////////////////////////////////////
		if((dwPacketSize > m_dwDataLen)  && (dwPacketSize < RECV_BUF_SIZE))
		{
			break;
		}

		if (dwPacketSize <= m_dwDataLen)
		{
			IDataBuffer* pDataBuffer =  CBufferManagerAll::GetInstancePtr()->AllocDataBuff(dwPacketSize);

			memcpy(pDataBuffer->GetBuffer(), m_pBufPos, dwPacketSize);

			m_dwDataLen -= dwPacketSize;

			m_pBufPos += dwPacketSize;

			pDataBuffer->SetTotalLenth(dwPacketSize);

			m_pDataHandler->OnDataHandle(pDataBuffer, this);
		}
		else
		{
			IDataBuffer* pDataBuffer =  CBufferManagerAll::GetInstancePtr()->AllocDataBuff(dwPacketSize);
			memcpy(pDataBuffer->GetBuffer(), m_pBufPos, m_dwDataLen);

			pDataBuffer->SetTotalLenth(m_dwDataLen);
			m_dwDataLen = 0;
			m_pBufPos = m_pRecvBuf;
			m_pCurRecvBuffer = pDataBuffer;
			m_pCurBufferSize = dwPacketSize;
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
	m_bConnected        = FALSE;
	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

	m_hSocket           = INVALID_SOCKET;
	m_bConnected        = FALSE;
	m_dwDataLen         = 0;
	m_IsSending			= FALSE;
	m_pDataHandler->OnCloseConnect(this);
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
	m_LastRecvTick = CommonFunc::GetTickCount();
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


BOOL CConnection::Clear()
{
	m_hSocket = INVALID_SOCKET;

	m_bConnected = FALSE;

	m_u64ConnData = 0;

	m_dwDataLen = 0;

	m_dwIpAddr  = 0;

	m_pBufPos   = m_pRecvBuf;

	if(m_pCurRecvBuffer != NULL)
	{
		m_pCurRecvBuffer->Release();
	}

	m_pCurRecvBuffer = NULL;

	m_nCheckNo = 0;

	m_IsSending	= FALSE;

	IDataBuffer* pBuff = NULL;
	while(m_SendBuffList.pop(pBuff))
	{
		pBuff->Release();
	}

	return TRUE;
}


BOOL CConnection::SendBuffer(IDataBuffer* pBuff)
{
	DoSend(pBuff);
	return TRUE;
}

BOOL CConnection::SendMessage(UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pData, UINT32 dwLen)
{
	IDataBuffer* pDataBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(dwLen + sizeof(PacketHeader));
	ERROR_RETURN_FALSE(pDataBuffer != NULL);

	PacketHeader* pHeader = (PacketHeader*)pDataBuffer->GetBuffer();
	pHeader->CheckCode = 0x88;
	pHeader->dwUserData = dwUserData;
	pHeader->u64TargetID = u64TargetID;
	pHeader->dwSize = dwLen + sizeof(PacketHeader);
	pHeader->dwMsgID = dwMsgID;
	pHeader->dwPacketNo = 1;

	memcpy(pDataBuffer->GetBuffer() + sizeof(PacketHeader), pData, dwLen);

	pDataBuffer->SetTotalLenth(pHeader->dwSize);

	return SendBuffer(pDataBuffer);
}

#ifdef WIN32
BOOL CConnection::DoSend(IDataBuffer* pBuff)
{
	if(pBuff != NULL)
	{
		m_SendBuffList.push(pBuff);
		if(!mCritSending.TryLock())
		{
			return FALSE;
		}

		///如果正在发送中就直接返回
		if (m_IsSending)
		{
			mCritSending.Unlock();
			return FALSE;
		}
	}
	else
	{
		mCritSending.Lock();
		m_IsSending = FALSE;
	}

	m_IsSending = TRUE;
	IDataBuffer* pFirstBuff = NULL;
	IDataBuffer* pSendingBuffer = NULL;
	int nSendSize = 0;
	int nCurPos = 0;

	IDataBuffer* pBuffer = NULL;
	while(m_SendBuffList.pop(pBuffer))
	{
		nSendSize += pBuffer->GetTotalLenth();

		if(pFirstBuff == NULL)
		{
			pFirstBuff = pBuffer;

			if(nSendSize >= RECV_BUF_SIZE)
			{
				pSendingBuffer = pBuffer;
				break;
			}

			pBuffer = NULL;
		}
		else
		{
			if(pSendingBuffer == NULL)
			{
				pSendingBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(RECV_BUF_SIZE);
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
			if(nSendSize >= RECV_BUF_SIZE)
			{
				break;
			}
		}
	}

	if(pSendingBuffer == NULL)
	{
		pSendingBuffer = pFirstBuff;
	}

	if(pSendingBuffer == NULL)
	{
		m_IsSending = FALSE;
		mCritSending.Unlock();
		return FALSE;
	}

	WSABUF  DataBuf;
	DataBuf.len = pSendingBuffer->GetTotalLenth();
	DataBuf.buf = pSendingBuffer->GetBuffer();
	m_IoOverlapSend.Clear();
	m_IoOverlapSend.dwCmdType   = NET_MSG_SEND;
	m_IoOverlapSend.pDataBuffer = pSendingBuffer;
	m_IoOverlapSend.dwConnID = m_dwConnID;

	DWORD dwSendBytes = 0;
	mCritSending.Unlock();
	int nRet = WSASend(m_hSocket, &DataBuf, 1, &dwSendBytes, 0, (LPOVERLAPPED)&m_IoOverlapSend, NULL);
	if(nRet == 0) //发送成功
	{
		if(dwSendBytes < DataBuf.len)
		{
			CLog::GetInstancePtr()->LogError("发送线程:直接发送功数据send:%d--Len:%d!", dwSendBytes, DataBuf.len);
		}
	}
	else if( nRet == -1 ) //发送出错
	{
		UINT32 errCode = CommonSocket::GetSocketLastError();
		if(errCode != ERROR_IO_PENDING)
		{
			Close();
			m_IsSending = FALSE;
			CLog::GetInstancePtr()->LogError("发送线程:发送失败, 连接关闭原因:%s!", CommonSocket::GetLastErrorStr(errCode).c_str());
		}
	}

	return TRUE;
}

#else
BOOL CConnection::DoSend(IDataBuffer* pBuff)
{
	if(pBuff != NULL)
	{
		m_SendBuffList.push(pBuff);
		if(!mCritSending.TryLock())
		{
			return FALSE;
		}

		///如果正在发送中就直接返回
		if (m_IsSending)
		{
			mCritSending.Unlock();
			return FALSE;
		}
	}
	else
	{
		mCritSending.Lock();
		m_IsSending = FALSE;
	}

	m_IsSending = TRUE;
	IDataBuffer* pFirstBuff = NULL;
	IDataBuffer* pSendingBuffer = NULL;
	int nSendSize = 0;
	int nCurPos = 0;

	IDataBuffer* pBuffer = NULL;
	while(m_SendBuffList.pop(pBuffer))
	{
		nSendSize += pBuffer->GetTotalLenth();

		if(pFirstBuff == NULL)
		{
			pFirstBuff = pBuffer;

			if(nSendSize >= RECV_BUF_SIZE)
			{
				pSendingBuffer = pBuffer;
				break;
			}

			pBuffer = NULL;
		}
		else
		{
			if(pSendingBuffer == NULL)
			{
				pSendingBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(RECV_BUF_SIZE);
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
			if(nSendSize >= RECV_BUF_SIZE)
			{
				break;
			}
		}
	}

	if(pSendingBuffer == NULL)
	{
		pSendingBuffer = pFirstBuff;
	}

	if(pSendingBuffer == NULL)
	{
		m_IsSending = FALSE;
		mCritSending.Unlock();
		return FALSE;
	}

	m_IoOverlapSend.Clear();
	m_IoOverlapSend.dwCmdType   = NET_MSG_SEND;
	m_IoOverlapSend.pDataBuffer = pSendingBuffer;
	m_IoOverlapSend.dwConnID    = m_dwConnID;


	//mudo库的处理方式
	//返回值为正数， 分为完全发送，和部分发送，部分发送，用另一个缓冲区装着继续发送
	//返回值为负数   错误码：
	//
	//if (errno != EWOULDBLOCK)
	//{
	//	//ERROR("TcpConnection sendInLoop");
	//	if (errno == EPIPE || errno == ECONNRESET)
	//	{
	//		faultError = true;//这就是真实的错误了
	//	}
	//}

	INT32 nRet = send(m_hSocket, pSendingBuffer->GetBuffer(), pSendingBuffer->GetTotalLenth(), 0);
	pSendingBuffer->Release();
	if(nRet < 0)
	{
		int nErr = CommonSocket::GetSocketLastError();
		CLog::GetInstancePtr()->LogError("发送线程:发送失败, 原因:%s!", CommonSocket::GetLastErrorStr(nErr).c_str());
		m_IsSending = FALSE;
	}
	else if(nRet < pSendingBuffer->GetTotalLenth())
	{
		CommonSocket::CloseSocket(m_hSocket);
		CLog::GetInstancePtr()->LogError("发送线程:发送失败, 缓冲区满了!");
		m_IsSending = FALSE;
	}
	return TRUE;
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
	ERROR_RETURN_NULL(m_pFreeConnRoot != NULL);

	CConnection* pTemp = NULL;
	m_CritSecConnList.Lock();
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
	m_CritSecConnList.Unlock();
	ERROR_RETURN_NULL(pTemp->GetConnectionID() != 0);
	ERROR_RETURN_NULL(pTemp->GetSocket() == INVALID_SOCKET);
	ERROR_RETURN_NULL(pTemp->IsConnectionOK() == FALSE);
	return pTemp;
}

CConnection* CConnectionMgr::GetConnectionByConnID( UINT32 dwConnID )
{
	UINT32 dwIndex = dwConnID % m_vtConnList.size();

	ERROR_RETURN_NULL(dwIndex < m_vtConnList.size())

	CConnection* pConnect = m_vtConnList.at(dwIndex - 1);
	if(pConnect->GetConnectionID() != dwConnID)
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

	m_CritSecConnList.Lock();

	if(m_pFreeConnTail == NULL)
	{
		if(m_pFreeConnRoot != NULL)
		{
			ASSERT_FAIELD;
		}

		m_pFreeConnTail = m_pFreeConnRoot = pConnection;
	}
	else
	{
		m_pFreeConnTail->m_pNext = pConnection;

		m_pFreeConnTail = pConnection;

		m_pFreeConnTail->m_pNext = NULL;

	}

	m_CritSecConnList.Unlock();

	UINT32 dwConnID = pConnection->GetConnectionID();

	pConnection->Clear();

	dwConnID += (UINT32)m_vtConnList.size();

	pConnection->SetConnectionID(dwConnID);

	return TRUE;
}

BOOL CConnectionMgr::CloseAllConnection()
{
	CConnection* pConn = NULL;
	for(size_t i = 0; i < m_vtConnList.size(); i++)
	{
		pConn = m_vtConnList.at(i);
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
		pConn->Close();
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
		CConnection* pTemp = m_vtConnList.at(i);
		if(!pTemp->IsConnectionOK())
		{
			continue;
		}

		if(curTick > (pTemp->m_LastRecvTick + 30000))
		{
			pTemp->Close();
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


