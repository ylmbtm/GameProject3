#include "stdafx.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "CommandDef.h"
#include <boost/asio/placeholders.hpp>
#include "PacketHeader.h"

#include <boost/asio.hpp>
#include "CommonSocket.h"


CConnection::CConnection(boost::asio::io_service& ioservice): m_hSocket(ioservice)
{
	m_pDataHandler		= NULL;

	m_dwDataLen			= 0;

	m_bConnected		= FALSE;

	m_u64ConnData        = 0;

	m_dwConnID          = 0;

	m_pCurRecvBuffer    = NULL;

	m_pBufPos           = m_pRecvBuf;

	m_nCheckNo          = 0;

	m_IsSending			= FALSE;

	m_pSendingBuffer	= NULL;
}

CConnection::~CConnection(void)
{
	Reset();

	m_dwConnID          = 0;

	m_pDataHandler		= NULL;
}

BOOL CConnection::DoReceive()
{
	//boost::asio::async_read(m_hSocket, boost::asio::buffer(m_pRecvBuf + m_dwDataLen,CONST_BUFF_SIZE - m_dwDataLen), boost::bind(&CConnection::handReaddata, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	//boost::asio::async_read_some(m_hSocket, boost::asio::buffer(m_pRecvBuf + m_dwDataLen,CONST_BUFF_SIZE - m_dwDataLen), boost::bind(&CConnection::handReaddata, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	m_hSocket.async_read_some(boost::asio::buffer(m_pRecvBuf + m_dwDataLen, RECV_BUF_SIZE - m_dwDataLen), boost::bind(&CConnection::HandReaddata, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	return TRUE;
}

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
			return FALSE;
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
	m_hSocket.shutdown(boost::asio::socket_base::shutdown_receive);
	m_hSocket.shutdown(boost::asio::socket_base::shutdown_send);
	m_hSocket.close();
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
	m_dwDataLen += dwBytes;

	if(!ExtractBuffer())
	{
		return FALSE;
	}

	if (!DoReceive())
	{
		return FALSE;
	}

	m_LastRecvTick = CommonFunc::GetTickCount();
	return TRUE;
}


BOOL CConnection::SetDataHandler( IDataHandler* pHandler )
{
	ERROR_RETURN_FALSE(pHandler != NULL);

	m_pDataHandler = pHandler;

	return TRUE;
}

boost::asio::ip::tcp::socket& CConnection::GetSocket()
{
	return m_hSocket;
}

BOOL CConnection::IsConnectionOK()
{
	return m_bConnected && m_hSocket.is_open();
}

BOOL CConnection::SetConnectionOK( BOOL bOk )
{
	m_bConnected = bOk;

	m_LastRecvTick = CommonFunc::GetTickCount();

	return TRUE;
}

BOOL CConnection::Reset()
{
	m_bConnected = FALSE;

	m_u64ConnData = 0;

	m_dwDataLen = 0;

	m_pBufPos   = m_pRecvBuf;

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

	if(m_nCheckNo == 0)
	{
		m_nCheckNo = pHeader->dwPacketNo - (pHeader->dwMsgID ^ pHeader->dwSize) + 1;
		return TRUE;
	}

	if(pHeader->dwPacketNo == (pHeader->dwMsgID ^ pHeader->dwSize) + m_nCheckNo)
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
		return m_hSocket.remote_endpoint().address().to_v4().to_ulong();
	}

	return CommonSocket::HostToNet(m_hSocket.remote_endpoint().address().to_v4().to_ulong());
}

BOOL CConnection::DoSend()
{
	m_IsSending = TRUE;

	if (m_pSendingBuffer != NULL)
	{
		m_pSendingBuffer->Release();
		m_pSendingBuffer = NULL;
	}

	IDataBuffer* pFirstBuff = NULL;
	int nSendSize = 0;
	int nCurPos = 0;

	IDataBuffer* pBuffer = NULL;
	while(m_SendBuffList.try_dequeue(pBuffer))
	{
		nSendSize += pBuffer->GetTotalLenth();

		if(pFirstBuff == NULL && m_pSendingBuffer == NULL)
		{
			pFirstBuff = pBuffer;

			pBuffer = NULL;
		}
		else
		{
			if(m_pSendingBuffer == NULL)
			{
				m_pSendingBuffer = CBufferAllocator::GetInstancePtr()->AllocDataBuff(RECV_BUF_SIZE);
				pFirstBuff->CopyTo(m_pSendingBuffer->GetBuffer() + nCurPos, pFirstBuff->GetTotalLenth());
				m_pSendingBuffer->SetTotalLenth(m_pSendingBuffer->GetTotalLenth() + pFirstBuff->GetTotalLenth());
				nCurPos += pFirstBuff->GetTotalLenth();
				pFirstBuff->Release();
				pFirstBuff = NULL;
			}

			pBuffer->CopyTo(m_pSendingBuffer->GetBuffer() + nCurPos, pBuffer->GetTotalLenth());
			m_pSendingBuffer->SetTotalLenth(m_pSendingBuffer->GetTotalLenth() + pBuffer->GetTotalLenth());
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

	if(m_pSendingBuffer == NULL)
	{
		m_pSendingBuffer = pFirstBuff;
	}

	if(m_pSendingBuffer == NULL)
	{
		m_IsSending = FALSE;
		return TRUE;
	}

	boost::asio::async_write(m_hSocket, boost::asio::buffer(m_pSendingBuffer->GetBuffer(), m_pSendingBuffer->GetBufferSize()), boost::bind(&CConnection::HandWritedata, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	return TRUE;
}


void CConnection::HandReaddata(const boost::system::error_code& error, size_t len)
{
	if (!error)
	{
		if (HandleRecvEvent((UINT32)len))
		{
			return;
		}
	}

	Close();

	return;
}


void CConnection::HandWritedata(const boost::system::error_code& error, size_t len)
{
	if (!error)
	{
		DoSend();

		return;
	}


	Close();

	return;
}

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
	ERROR_RETURN_NULL(pTemp->m_hSocket.is_open() == FALSE);
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

BOOL CConnectionMgr::CheckConntionAvalible(INT32 nInterval)
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

		if(curTick > (pConnection->m_LastRecvTick + nInterval * 1000))
		{
			CLog::GetInstancePtr()->LogError("CConnectionMgr::CheckConntionAvalible 超时主动断开连接 ConnID:%d", pConnection->GetConnectionID());
			pConnection->Close();
		}
	}

	return TRUE;
}
BOOL CConnectionMgr::InitConnectionList(UINT32 nMaxCons, boost::asio::io_service& ioservice)
{
	ERROR_RETURN_FALSE(m_pFreeConnRoot == NULL);
	ERROR_RETURN_FALSE(m_pFreeConnTail == NULL);

	m_vtConnList.assign(nMaxCons, NULL);
	for(UINT32 i = 0; i < nMaxCons; i++)
	{
		CConnection* pConn = new CConnection(ioservice);

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


