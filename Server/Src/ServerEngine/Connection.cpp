#include "stdafx.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "Utility/CommonSocket.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "PacketHeader.h"

void NetIoOperatorData::Clear()
{
#ifdef WIN32
	memset(&Overlap, 0, sizeof(Overlap));
#endif

	dwCmdType = 0;

	pDataBuffer = NULL;
}


CConnection::CConnection()
{
	m_hSocket			= INVALID_SOCKET;

	m_pDataHandler		= NULL;

	m_dwDataLen			= 0;

	m_bConnected		= FALSE;

	m_dwConnData        = 0;

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

	m_dwConnData        = 0;

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

	DataBuf.len = CONST_BUFF_SIZE - m_dwDataLen;
	DataBuf.buf = m_pRecvBuf + m_dwDataLen;

	DWORD dwRecvBytes = 0, dwFlags = 0;

	m_IoOverlapRecv.Clear();
	m_IoOverlapRecv.dwCmdType = NET_MSG_RECV;

	int nRet = WSARecv(m_hSocket, &DataBuf, 1, &dwRecvBytes, &dwFlags, (LPOVERLAPPED)&m_IoOverlapRecv, NULL);
	if(nRet != 0)
	{
		//对于WSARecv来说， 只要返回0,就表示没有错误发生。
		//当返回为ERROR_IO_PENDING时，表示提交读数据请求成功， 其它的返回值都是错误。
		int nError = CommonSocket::GetSocketLastError();
		if(nError != ERROR_IO_PENDING )
		{
			CLog::GetInstancePtr()->AddLog("关闭连接，因为接收数据发生错误:%s!", CommonSocket::GetLastErrorStr(nError).c_str());

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
		int nBytes = recv(m_hSocket, m_pRecvBuf+m_dwDataLen, CONST_BUFF_SIZE - m_dwDataLen, 0);
		if(nBytes == 0)
		{
			if(m_dwDataLen == CONST_BUFF_SIZE)
			{
				CLog::GetInstancePtr()->AddLog("buffer满了，需要再读一次!!");

				if(!ExtractBuffer())
				{
					return FALSE;
				}
				
				continue;
			}

			CLog::GetInstancePtr()->AddLog("收到数据为0， 判断是对方关闭################!!");

			return FALSE;
		}
		else if(nBytes < 0)
		{
			int nErr = CommonSocket::GetSocketLastError();
			if( nErr == EAGAIN)
			{
				CLog::GetInstancePtr()->AddLog("读成功了，缓冲区己经无数据可读!!");

				return TRUE;
			}
			else
			{
				CLog::GetInstancePtr()->AddLog("读失败， 可能连接己断开 原因:%s!!", CommonSocket::GetLastErrorStr(nErr).c_str());

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
	return m_dwConnData;
}

void CConnection::SetConnectionID( UINT32 dwConnID )
{
    ASSERT(m_dwConnID == 0);
    ASSERT(dwConnID != 0);
    ASSERT(!m_bConnected);

    m_dwConnID = dwConnID;

	return ;
}

VOID CConnection::SetConnectionData( UINT64 dwData )
{
	ASSERT(m_dwConnID != 0);

	m_dwConnData = dwData;

	return ;
}


BOOL CConnection::ExtractBuffer()
{
	//在这方法里返回FALSE。	
	//会在外面导致这个连接被关闭。

	if(m_dwDataLen <= 0)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	while(TRUE)
	{
		if(m_pCurRecvBuffer != NULL)
		{
			if ((m_pCurRecvBuffer->GetTotalLenth() + m_dwDataLen ) < m_pCurBufferSize)
			{
				memcpy(m_pCurRecvBuffer->GetBuffer()+m_pCurRecvBuffer->GetTotalLenth(), m_pBufPos, m_dwDataLen);
				m_dwDataLen = 0; 
				m_pBufPos = m_pRecvBuf; 
				m_pCurRecvBuffer->SetTotalLenth(m_pCurRecvBuffer->GetTotalLenth() + m_dwDataLen);
				break;
			}
			else
			{
				memcpy(m_pCurRecvBuffer->GetBuffer()+m_pCurRecvBuffer->GetTotalLenth(), m_pBufPos, m_pCurBufferSize-m_pCurRecvBuffer->GetTotalLenth());
				m_dwDataLen -= m_pCurBufferSize-m_pCurRecvBuffer->GetTotalLenth(); 
				m_pBufPos += m_pCurBufferSize-m_pCurRecvBuffer->GetTotalLenth(); 
				m_pCurRecvBuffer->SetTotalLenth(m_pCurBufferSize);
				m_pDataHandler->OnDataHandle(m_pCurRecvBuffer, this);
				m_pCurRecvBuffer = NULL;
			}
		}

		if(m_dwDataLen < sizeof(PacketHeader))
		{
			break;
		}

		PacketHeader *pHeader = (PacketHeader *)m_pBufPos;
		//////////////////////////////////////////////////////////////////////////
		//在这里对包头进行检查, 如果不合法就要返回FALSE;
		/*
		1.首先验证包的验证吗
		2.包的长度
		3.包的序号*/
		if(pHeader->CheckCode == 0x88)
		{
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
		}
		else
		{
			return FALSE;
		}


		if(pHeader->dwSize == 0)
		{
			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
		if((pHeader->dwSize > m_dwDataLen)  && (pHeader->dwSize < CONST_BUFF_SIZE))
		{
			break;
		}

		if (pHeader->dwSize <= m_dwDataLen)
		{
			IDataBuffer *pDataBuffer =  CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pHeader->dwSize);

			memcpy(pDataBuffer->GetBuffer(), m_pBufPos, pHeader->dwSize);

			m_dwDataLen -= pHeader->dwSize;

			m_pBufPos += pHeader->dwSize;

			pDataBuffer->SetTotalLenth(pHeader->dwSize);

			m_pDataHandler->OnDataHandle(pDataBuffer, this);
		}
		else
		{
			IDataBuffer *pDataBuffer =  CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pHeader->dwSize);
			memcpy(pDataBuffer->GetBuffer(), m_pBufPos, m_dwDataLen);

			pDataBuffer->SetTotalLenth(m_dwDataLen);
			m_dwDataLen = 0;
			m_pBufPos = m_pRecvBuf;
			m_pCurRecvBuffer = pDataBuffer;
			m_pCurBufferSize = pHeader->dwSize;
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
    m_pDataHandler->OnCloseConnect(this);
	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

    m_hSocket           = INVALID_SOCKET;
    m_bConnected        = FALSE;
    m_dwDataLen         = 0;

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

BOOL CConnection::SetDataHandler( IDataHandler *pHandler )
{
	if(pHandler == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_pDataHandler = pHandler;

	return TRUE;
}

SOCKET CConnection::GetSocket()
{
	return m_hSocket;
}

BOOL CConnection::IsConnectionOK()
{
	if((m_hSocket == INVALID_SOCKET)||(m_hSocket == 0))
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

	m_dwConnData = 0;

    m_dwDataLen = 0;

    m_dwIpAddr  = 0;

	m_pBufPos   = m_pRecvBuf;

	m_pCurRecvBuffer = NULL;

	m_nCheckNo = 0;

	m_IsSending	= FALSE;

	m_CritSecSendList.Lock();
	for(int i = 0; i < (int)m_SendBuffList.size(); i++)
	{
		m_SendBuffList[i]->Release();
	}
	
	m_SendBuffList.clear();

	m_CritSecSendList.Unlock();

    return TRUE;
}


BOOL CConnection::SendBuffer(IDataBuffer *pBuff)
{
	CAutoLock Lock(&m_CritSecSendList);
	m_SendBuffList.push_back(pBuff);
	DoSend();
	return TRUE;
}

BOOL CConnection::SendMessage(UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char *pData, UINT32 dwLen)
{
	IDataBuffer *pDataBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(dwLen + sizeof(PacketHeader));
	if(pDataBuffer == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	PacketHeader *pHeader = (PacketHeader*)pDataBuffer->GetBuffer();
	pHeader->CheckCode = 0x88;
	pHeader->dwUserData = dwUserData;
	pHeader->u64TargetID = u64TargetID;
	pHeader->dwSize = dwLen + sizeof(PacketHeader);
	pHeader->dwMsgID = dwMsgID;

	memcpy(pDataBuffer->GetBuffer() + sizeof(PacketHeader), pData, dwLen);

	pDataBuffer->SetTotalLenth(pHeader->dwSize);

	return SendBuffer(pDataBuffer);
}

#ifdef WIN32
BOOL CConnection::DoSend()
{
	///如果正在发送中就直接返回
	if (m_IsSending)
	{
		return FALSE;
	}

	if(m_SendBuffList.empty())
	{
		return FALSE;
	}

	IDataBuffer *pSendBuffer = NULL;
	int nSendCount = 0;
	int nSendSize = 0;
	for(int i = 0; i < m_SendBuffList.size(); i++)
	{
		IDataBuffer *pDataBuffer = (IDataBuffer *)m_SendBuffList[i];

		if((nSendSize + pDataBuffer->GetTotalLenth())>CONST_BUFF_SIZE)
		{
			break;
		}

		nSendSize += pDataBuffer->GetTotalLenth();
		nSendCount+= 1;
	}
	
	if(nSendCount <= 1)
	{
		pSendBuffer = m_SendBuffList[0];

		m_SendBuffList.erase(m_SendBuffList.begin());
	}
	else
	{
		pSendBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(CONST_BUFF_SIZE);
		int nCurPos = 0;
		for(int i = 0; i < nSendCount; i++)
		{
			IDataBuffer *pDataBuffer = (IDataBuffer *)m_SendBuffList[i];
			pDataBuffer->CopyTo(pSendBuffer->GetBuffer()+nCurPos, pDataBuffer->GetTotalLenth());
			pSendBuffer->SetTotalLenth(pSendBuffer->GetTotalLenth()+pDataBuffer->GetTotalLenth());
			nCurPos += pDataBuffer->GetTotalLenth();
			pDataBuffer->Release();
		}
		m_SendBuffList.erase(m_SendBuffList.begin(), m_SendBuffList.begin() + nSendCount);
	}

	WSABUF  DataBuf;
	DataBuf.len = pSendBuffer->GetTotalLenth();
	DataBuf.buf = pSendBuffer->GetBuffer();

	m_IoOverlapSend.Clear();
	m_IoOverlapSend.dwCmdType   = NET_MSG_SEND;
	m_IoOverlapSend.pDataBuffer = pSendBuffer;

	DWORD dwSendBytes = 0;
	int nRet = WSASend(m_hSocket, &DataBuf, 1, &dwSendBytes, 0, (LPOVERLAPPED)&m_IoOverlapSend, NULL);
	if(nRet == 0) //发送成功
	{
		if(dwSendBytes < DataBuf.len)
		{
			CLog::GetInstancePtr()->AddLog("发送线程:直接发送功数据%d!", dwSendBytes);
		}

	}
	else if( nRet == -1 ) //发送出错
	{
		UINT32 errCode = CommonSocket::GetSocketLastError();
		if(errCode != ERROR_IO_PENDING)
		{
			Close();
			pSendBuffer->Release();
			CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 连接关闭原因:%s!", CommonSocket::GetLastErrorStr(errCode).c_str());
		}
	}

	return TRUE;
}

#else
BOOL CConnection::DoSend()
{
	///如果正在发送中就直接返回
	if (m_IsSending)
	{
		return FALSE;
	}

	if(m_SendBuffList.empty())
	{
		return FALSE;
	}

	IDataBuffer *pSendBuffer = NULL;
	int nSendCount = 0;
	int nSendSize = 0;
	for(int i = 0; i < m_SendBuffList.size(); i++)
	{
		IDataBuffer *pDataBuffer = (IDataBuffer *)m_SendBuffList[i];

		if((nSendSize + pDataBuffer->GetTotalLenth())>CONST_BUFF_SIZE)
		{
			break;
		}

		nSendSize += pDataBuffer->GetTotalLenth();
		nSendCount+= 1;
	}

	if(nSendCount <= 1)
	{
		pSendBuffer = m_SendBuffList[0];

		m_SendBuffList.erase(m_SendBuffList.begin());
	}
	else
	{
		pSendBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(CONST_BUFF_SIZE);
		int nCurPos = 0;
		for(int i = 0; i < nSendCount; i++)
		{
			IDataBuffer *pDataBuffer = (IDataBuffer *)m_SendBuffList[i];
			pDataBuffer->CopyTo(pSendBuffer->GetBuffer()+nCurPos, pDataBuffer->GetTotalLenth());
			pSendBuffer->SetTotalLenth(pSendBuffer->GetTotalLenth()+pDataBuffer->GetTotalLenth());
			nCurPos += pDataBuffer->GetTotalLenth();
			pDataBuffer->Release();
		}
		m_SendBuffList.erase(m_SendBuffList.begin(), m_SendBuffList.begin() + nSendCount);
	}

	m_IoOverlapSend.Clear();
	m_IoOverlapSend.dwCmdType   = NET_MSG_SEND;
	m_IoOverlapSend.pDataBuffer = pSendBuffer;

	INT32 nRet = send(m_hSocket, pSendBuffer->GetBuffer(),pSendBuffer->GetTotalLenth(), 0);
	if(nRet < 0)
	{
		int nErr = CommonSocket::GetSocketLastError();

		CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 原因:%s!", CommonSocket::GetLastErrorStr(nErr).c_str());
	}
	else if(nRet < pSendBuffer->GetTotalLenth())
	{
		CommonSocket::CloseSocket(m_hSocket);

		CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 缓冲区满了!");
	}

	return TRUE;
}


#endif

CConnectionMgr::CConnectionMgr()
{
    m_pFreeConnRoot = NULL;
}

CConnectionMgr::~CConnectionMgr()
{

}

CConnection* CConnectionMgr::CreateConnection() 
{
    CAutoLock Lock(&m_CritSecConnList);

    if(m_pFreeConnRoot == NULL)
    {
        ASSERT_FAIELD;
        return NULL;
    }

    CConnection* pTemp = m_pFreeConnRoot;

    m_pFreeConnRoot = pTemp->m_pNext;

    if(pTemp->GetConnectionID() == 0)
    {
        ASSERT_FAIELD;
        return NULL;
    }

    return pTemp;
}

CConnection* CConnectionMgr::GetConnectionByConnID( UINT32 dwConnID )
{
    if(dwConnID >= m_vtConnList.size())
    {
        ASSERT_FAIELD;
        return NULL;
    }

    return m_vtConnList.at(dwConnID);
}


CConnectionMgr* CConnectionMgr::GetInstancePtr()
{
	static CConnectionMgr ConnectionMgr;

	return &ConnectionMgr;
}


VOID CConnectionMgr::DeleteConnection( CConnection *pConnection )
{
    CAutoLock Lock(&m_CritSecConnList);
    
    if(pConnection == NULL)
    {
        ASSERT_FAIELD;
        return ;
    }

    pConnection->m_pNext = m_pFreeConnRoot;

    m_pFreeConnRoot = pConnection;

	pConnection->Clear();

	return ;
}

BOOL CConnectionMgr::CloseAllConnection()
{
    CConnection *pConn = NULL;
    for(size_t i = 0; i < m_vtConnList.size(); i++)
    {
		pConn = m_vtConnList.at(i);
		pConn->Close();
    }

    return TRUE;
}

BOOL CConnectionMgr::DestroyAllConnection()
{
    m_vtConnList.clear();

	return TRUE;
}

BOOL CConnectionMgr::CheckConntionAvalible()
{
	return TRUE;
	UINT32 curTick = CommonFunc::GetTickCount();

	for(int i = 1; i < m_vtConnList.size(); i++)
	{
		 CConnection *pTemp = m_vtConnList.at(i);
		 if(!pTemp->IsConnectionOK())
		 {
			 continue;
		 }

		if(curTick > (pTemp->m_LastRecvTick+3000))
		{
			pTemp->Close();
		}
	}

	return TRUE;
}

BOOL CConnectionMgr::InitConnectionList(UINT32 nMaxCons)
{
    ASSERT(m_pFreeConnRoot == NULL);

    CConnection *pTemp = NULL;

    m_vtConnList.assign(nMaxCons+1, NULL);
    for(UINT32 i = 1; i < nMaxCons+1; i++)
    {
		CConnection *pConn = new CConnection();

        m_vtConnList[i] = pConn;

        pConn->SetConnectionID(i) ;

        if (m_pFreeConnRoot == NULL)
        {
            m_pFreeConnRoot = pConn;
			pConn->m_pNext = NULL;
			pTemp = pConn;
        }
        else
        {
			pTemp->m_pNext = pConn;
			pTemp = pConn;
			pTemp->m_pNext = NULL;
        }
    }

	 return TRUE;
}


