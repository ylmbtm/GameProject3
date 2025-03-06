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

    nOpType = 0;
    nConnID = 0;

    pDataBuffer = NULL;
}


CConnection::CConnection()
{
    m_hSocket           = INVALID_SOCKET;

    m_pDataHandler      = NULL;

    m_nDataLen          = 0;

    m_eConnStatus       = ENS_INIT;

    m_uConnData         = 0;

    m_nConnID           = 0;

    m_pCurRecvBuffer    = NULL;

    m_pBufPos           = m_pRecvBuf;

    m_nCheckNo          = 0;

    m_IsSending         = FALSE;

    m_pSendingBuffer    = NULL;

    m_nSendingPos       = 0;

    m_bNotified         = FALSE;

    m_bPacketNoCheck    = FALSE;

    m_uLastRecvTick      = 0;
}

CConnection::~CConnection(void)
{
    Reset();

    m_nConnID         = 0;

    m_pDataHandler    = NULL;

    m_bPacketNoCheck  = FALSE;
}

#ifdef WIN32

BOOL CConnection::DoReceive()
{
    WSABUF  DataBuf;

    DataBuf.len = RECV_BUF_SIZE - m_nDataLen;
    DataBuf.buf = m_pRecvBuf + m_nDataLen;

    DWORD nRecvBytes = 0, nFlags = 0;

    m_IoOverlapRecv.Reset();
    m_IoOverlapRecv.nOpType = NET_OP_RECV;
    m_IoOverlapRecv.nConnID = m_nConnID;

    int nRet = WSARecv(m_hSocket, &DataBuf, 1, &nRecvBytes, &nFlags, (LPOVERLAPPED)&m_IoOverlapRecv, NULL);
    if(nRet != 0)
    {
        //对于WSARecv来说， 只要返回0,就表示没有错误发生。
        //当返回为ERROR_IO_PENDING时，表示提交读数据请求成功， 其它的返回值都是错误。
        int nError = CommonSocket::GetSocketLastError();
        if(nError != ERROR_IO_PENDING )
        {
            CLog::GetInstancePtr()->LogWarn("关闭连接，因为接收数据发生错误:%s!", CommonFunc::GetLastErrorStr(nError).c_str());

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
        int nBytes = recv(m_hSocket, m_pRecvBuf + m_nDataLen, RECV_BUF_SIZE - m_nDataLen, 0);
        if(nBytes < 0)
        {
            int nErr = CommonSocket::GetSocketLastError();
            if( nErr == EAGAIN)
            {
                return TRUE;
            }

            CLog::GetInstancePtr()->LogWarn("读失败， 可能连接己断开 原因:%s!!", CommonFunc::GetLastErrorStr(nErr).c_str());
            return FALSE;
        }

        if (nBytes == 0)
        {
            return FALSE;
        }

        m_nDataLen += nBytes;

        if(!ExtractBuffer())
        {
            return FALSE;
        }
    }

    return TRUE;
}

#endif

INT32 CConnection::GetConnectionID()
{
    return m_nConnID;
}

UINT64 CConnection::GetConnectionData()
{
    return m_uConnData;
}

void CConnection::SetConnectionID( INT32 nConnID )
{
    ERROR_RETURN_NONE(nConnID != 0);

    ERROR_RETURN_NONE(m_eConnStatus == ENS_INIT);

    m_nConnID = nConnID;

    return ;
}

VOID CConnection::SetConnectionData( UINT64 uData )
{
    ERROR_RETURN_NONE(m_nConnID != 0);

    m_uConnData = uData;

    return ;
}

BOOL CConnection::Shutdown()
{
    m_eConnStatus = ENS_CLOSEING;

    m_uLastRecvTick = CommonFunc::GetTickCount();

    CommonSocket::_ShutdownSocket(m_hSocket);

    return TRUE;
}

BOOL CConnection::ExtractBuffer()
{
    //在这方法里返回FALSE。
    //会在外面导致这个连接被关闭。
    if (m_nDataLen == 0)
    {
        return TRUE;
    }

    while (TRUE)
    {
        if (m_pCurRecvBuffer != NULL)
        {
            if ((m_pCurRecvBuffer->GetTotalLenth() + m_nDataLen) < m_nCurBufferSize)
            {
                memcpy(m_pCurRecvBuffer->GetBuffer() + m_pCurRecvBuffer->GetTotalLenth(), m_pBufPos, m_nDataLen);
                m_pBufPos = m_pRecvBuf;
                m_pCurRecvBuffer->SetTotalLenth(m_pCurRecvBuffer->GetTotalLenth() + m_nDataLen);
                m_nDataLen = 0;
                break;
            }
            else
            {
                memcpy(m_pCurRecvBuffer->GetBuffer() + m_pCurRecvBuffer->GetTotalLenth(), m_pBufPos, m_nCurBufferSize - m_pCurRecvBuffer->GetTotalLenth());
                m_nDataLen -= m_nCurBufferSize - m_pCurRecvBuffer->GetTotalLenth();
                m_pBufPos += m_nCurBufferSize - m_pCurRecvBuffer->GetTotalLenth();
                m_pCurRecvBuffer->SetTotalLenth(m_nCurBufferSize);
                m_uLastRecvTick = CommonFunc::GetTickCount();
                UpdateCheckNo(m_pCurRecvBuffer->GetBuffer());
                m_pDataHandler->OnDataHandle(m_pCurRecvBuffer, GetConnectionID());
                m_pCurRecvBuffer = NULL;
            }
        }

        if (m_nDataLen < sizeof(PacketHeader))
        {
            if (m_nDataLen >= 1 && *(BYTE*)m_pBufPos != 0x88)
            {
                CLog::GetInstancePtr()->LogInfo("验证首字节失改!, m_nDataLen:%d--ConnID:%d", m_nDataLen, m_nConnID);
                return FALSE;
            }

            break;
        }

        PacketHeader* pHeader = (PacketHeader*)m_pBufPos;
        //////////////////////////////////////////////////////////////////////////
        //在这里对包头进行检查, 如果不合法就要返回FALSE;
        if (!CheckHeader(m_pBufPos))
        {
            //return FALSE;
        }

        INT32 nPacketSize = pHeader->nSize;

        //////////////////////////////////////////////////////////////////////////
        if ((nPacketSize > m_nDataLen) && (nPacketSize < RECV_BUF_SIZE))
        {
            break;
        }

        if (nPacketSize <= m_nDataLen)
        {
            IDataBuffer* pDataBuffer = CBufferAllocator::GetInstancePtr()->AllocDataBuff(nPacketSize);

            memcpy(pDataBuffer->GetBuffer(), m_pBufPos, nPacketSize);

            m_nDataLen -= nPacketSize;

            m_pBufPos += nPacketSize;

            pDataBuffer->SetTotalLenth(nPacketSize);

            m_uLastRecvTick = CommonFunc::GetTickCount();
            UpdateCheckNo(pDataBuffer->GetBuffer());
            m_pDataHandler->OnDataHandle(pDataBuffer, GetConnectionID());
        }
        else
        {
            IDataBuffer* pDataBuffer = CBufferAllocator::GetInstancePtr()->AllocDataBuff(nPacketSize);
            memcpy(pDataBuffer->GetBuffer(), m_pBufPos, m_nDataLen);

            pDataBuffer->SetTotalLenth(m_nDataLen);
            m_nDataLen = 0;
            m_pBufPos = m_pRecvBuf;
            m_pCurRecvBuffer = pDataBuffer;
            m_nCurBufferSize = nPacketSize;
        }
    }

    if (m_nDataLen > 0)
    {
        memmove(m_pRecvBuf, m_pBufPos, m_nDataLen);
    }

    m_pBufPos = m_pRecvBuf;

    return TRUE;
}

BOOL CConnection::Close()
{
    CommonSocket::CloseSocket(m_hSocket);

    m_hSocket           = INVALID_SOCKET;

    m_nDataLen         = 0;

    m_IsSending         = FALSE;

    if(m_pDataHandler != NULL && !m_bNotified)
    {
        m_bNotified = TRUE;
        m_pDataHandler->OnCloseConnect(GetConnectionID());
    }

    m_eConnStatus = ENS_INIT;

    return TRUE;
}

BOOL CConnection::HandleRecvEvent(INT32 nBytes)
{
#ifdef WIN32
    m_nDataLen += nBytes;

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

//   if(!ExtractBuffer())
//   {
//       return FALSE;
//   }
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

ENetStatus CConnection::GetConnectStatus()
{
    return m_eConnStatus;
}

BOOL CConnection::SetConnectStatus(ENetStatus eConnStatus)
{
    m_eConnStatus = eConnStatus;

    m_uLastRecvTick = CommonFunc::GetTickCount();

    return TRUE;
}

BOOL CConnection::Reset()
{
    m_hSocket = INVALID_SOCKET;

    m_eConnStatus = ENS_INIT;

    m_uConnData = 0;

    m_nDataLen = 0;

    m_nIpAddr  = 0;

    m_pBufPos   = m_pRecvBuf;

    m_bNotified = FALSE;

    m_bPacketNoCheck = FALSE;

    m_uLastRecvTick = 0;

    memset(&m_UdpAddr, 0, sizeof(m_UdpAddr));

    if(m_pCurRecvBuffer != NULL)
    {
        m_pCurRecvBuffer->Release();
        m_pCurRecvBuffer = NULL;
    }


    m_nCheckNo = 0;

    m_IsSending = FALSE;

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

BOOL CConnection::CheckHeader(CHAR* pNetPacket)
{
    PacketHeader* pHeader = (PacketHeader*)pNetPacket;
    if (pHeader->CheckCode != CODE_VALUE)
    {
        CLog::GetInstancePtr()->LogInfo("验证-失败 pHeader->CheckCode error");
        return FALSE;
    }

    if ((pHeader->nSize > 1024 * 1024) || (pHeader->nSize <= 0))
    {
        CLog::GetInstancePtr()->LogInfo("验证-失败 packetsize < 0, pHeader->nMsgID:%d, roleid:%lld", pHeader->nMsgID, pHeader->u64TargetID);
        return FALSE;
    }

    if (pHeader->nMsgID > 399999 || pHeader->nMsgID <= 0)
    {
        CLog::GetInstancePtr()->LogInfo("验证-失败 Invalid MessageID roleid:%lld", pHeader->u64TargetID);
        return FALSE;
    }

    if (!m_bPacketNoCheck)
    {
        return TRUE;
    }

    INT32 nPktChkNo = pHeader->nPacketNo - (pHeader->nMsgID ^ pHeader->nSize);

    if (nPktChkNo <= 0)
    {
        CLog::GetInstancePtr()->LogInfo("nPktChkNo <= 0");
        return FALSE;
    }

    if(m_nCheckNo == 0)
    {
        return TRUE;
    }

    if(m_nCheckNo == nPktChkNo)
    {
        return TRUE;
    }

    CLog::GetInstancePtr()->LogInfo("验证-失败 m_nCheckNo:%d, nPktChkNo:%ld", m_nCheckNo, nPktChkNo);

    return FALSE;
}

BOOL CConnection::UpdateCheckNo(CHAR* pNetPacket)
{
    PacketHeader* pHeader = (PacketHeader*)pNetPacket;

    INT32 nPktChkNo = pHeader->nPacketNo - (pHeader->nMsgID ^ pHeader->nSize);

    if (m_nCheckNo == 0)
    {
        m_nCheckNo = nPktChkNo + 1;
        return TRUE;
    }

    m_nCheckNo += 1;

    return TRUE;
}

UINT32 CConnection::GetIpAddr(BOOL bHost)
{
    if (bHost)
    {
        return m_nIpAddr;
    }

    return CommonSocket::HostToNet(m_nIpAddr);
}

VOID CConnection::EnableCheck(BOOL bCheck)
{
    m_bPacketNoCheck = bCheck;
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
    m_IoOverlapSend.nOpType = NET_OP_SEND;
    m_IoOverlapSend.pDataBuffer = pSendingBuffer;
    m_IoOverlapSend.nConnID = m_nConnID;

    DWORD nSendBytes = 0;
    int nRet = WSASend(m_hSocket, &DataBuf, 1, &nSendBytes, 0, (LPOVERLAPPED)&m_IoOverlapSend, NULL);
    if(nRet == 0) //发送成功
    {
        //if(nSendBytes < DataBuf.len)
        //{
        //  CLog::GetInstancePtr()->LogError("发送线程:直接发送数据成功send:%d--Len:%d!", nSendBytes, DataBuf.len);
        //}
    }
    else if( nRet == -1 ) //发送出错
    {
        INT32 errCode = CommonSocket::GetSocketLastError();
        if(errCode != ERROR_IO_PENDING)
        {
            pSendingBuffer->Release();
            pSendingBuffer = NULL;
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
    //  //ERROR("TcpConnection sendInLoop");
    //  if (errno == EPIPE || errno == ECONNRESET)
    //  {
    //      faultError = true;//这就是真实的错误了
    //  }
    //}
    // #define E_SEND_SUCCESS               1
    // #define E_SEND_UNDONE                2
    // #define E_SEND_ERROR                 3

    if (m_pSendingBuffer != NULL)
    {
        INT32 nDataLen = m_pSendingBuffer->GetTotalLenth() - m_nSendingPos;
        INT32 nRet = send(m_hSocket, m_pSendingBuffer->GetBuffer() + m_nSendingPos, nDataLen, 0);
        if (nRet < 0)
        {
            if (errno != EAGAIN)
            {
                m_pSendingBuffer->Release();
                m_pSendingBuffer = NULL;
                m_nSendingPos = 0;
                CLog::GetInstancePtr()->LogWarn("发送线程:发送失败, 连接关闭原因:%s!", CommonFunc::GetLastErrorStr(errno).c_str());
                return E_SEND_ERROR;
            }

            return E_SEND_SUCCESS;
        }

        if (nRet < nDataLen)
        {
            //这就表示发送了一半的数据
            m_nSendingPos += nRet;
            return E_SEND_UNDONE;
        }

        m_pSendingBuffer->Release();
        m_pSendingBuffer = NULL;
        m_nSendingPos = 0;
    }

    IDataBuffer* pBuffer = NULL;
    while(m_SendBuffList.try_dequeue(pBuffer))
    {
        if (pBuffer == NULL)
        {
            continue;
        }

        INT32 nRet = send(m_hSocket, pBuffer->GetBuffer(), pBuffer->GetTotalLenth(), 0);
        if (nRet < 0)
        {
            if (errno != EAGAIN)
            {
                pBuffer->Release();
                pBuffer = NULL;
                CLog::GetInstancePtr()->LogWarn("发送线程:发送失败, 连接关闭原因2:%s!", CommonFunc::GetLastErrorStr(errno).c_str());
                return E_SEND_ERROR;
            }

            m_pSendingBuffer = pBuffer;
            m_nSendingPos = 0;

            return E_SEND_SUCCESS;
        }

        if (nRet < pBuffer->GetTotalLenth())
        {
            //这就表示发送了一半的数据
            m_pSendingBuffer = pBuffer;
            m_nSendingPos = nRet;
            return E_SEND_UNDONE;
        }

        pBuffer->Release();
        pBuffer = NULL;
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
    ERROR_RETURN_NULL(pTemp->GetConnectStatus() == ENS_INIT);
    return pTemp;
}

CConnection* CConnectionMgr::GetConnectionByID( INT32 nConnID )
{
    ERROR_RETURN_NULL(nConnID != 0);

    INT32 nIndex = nConnID % m_vtConnList.size();

    CConnection* pConnect = m_vtConnList.at(nIndex == 0 ? (m_vtConnList.size() - 1) : (nIndex - 1));

    if (pConnect->GetConnectionID() != nConnID)
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


BOOL CConnectionMgr::DeleteConnection(INT32 nConnID)
{
    ERROR_RETURN_FALSE(nConnID != 0);

    INT32 nIndex = nConnID % m_vtConnList.size();

    CConnection* pConnection = m_vtConnList.at(nIndex == 0 ? (m_vtConnList.size() - 1) : (nIndex - 1));

    ERROR_RETURN_FALSE(pConnection->GetConnectionID() == nConnID)

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

    pConnection->Reset();

    nConnID += (INT32)m_vtConnList.size();

    if (nConnID <= 0)
    {
        nConnID = nIndex + 1;
    }

    pConnection->SetConnectionID(nConnID);

    return TRUE;
}

BOOL CConnectionMgr::CloseAllConnection()
{
    CConnection* pConn = NULL;
    for(size_t i = 0; i < m_vtConnList.size(); i++)
    {
        pConn = m_vtConnList.at(i);
        if (pConn->GetConnectStatus() != ENS_CONNECTED)
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
        if (pConn == NULL)
        {
            continue;
        }

        if (pConn->GetConnectStatus() != ENS_INIT && pConn->m_hSocket != INVALID_SOCKET)
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
    UINT64 uCurTick = CommonFunc::GetTickCount();

    for(std::vector<CConnection*>::size_type i = 0; i < m_vtConnList.size(); i++)
    {
        CConnection* pConnection = m_vtConnList.at(i);
        if(pConnection->GetConnectStatus() == ENS_INIT)
        {
            continue;
        }

        if (pConnection->m_uLastRecvTick <= 0)
        {
            continue;
        }

        if (pConnection->GetConnectionData() == 1)
        {
            continue;
        }

        //如果当前是等待关闭的状态
        if (pConnection->GetConnectStatus() == ENS_CLOSEING)
        {
            if (uCurTick > (pConnection->m_uLastRecvTick + 10 * 1000))
            {
                CLog::GetInstancePtr()->LogError("CConnectionMgr::CheckConntionAvalible WAIT超时主动断开连接 ConnID:%d", pConnection->GetConnectionID());
                pConnection->Close();
            }

            continue;
        }

        if(uCurTick > (pConnection->m_uLastRecvTick + nInterval * 1000))
        {
            pConnection->Shutdown();
        }
    }

    return TRUE;
}

BOOL CConnectionMgr::InitConnectionList(INT32 nMaxCons)
{
    ERROR_RETURN_FALSE(m_pFreeConnRoot == NULL);
    ERROR_RETURN_FALSE(m_pFreeConnTail == NULL);

    m_vtConnList.assign(nMaxCons, NULL);
    for(INT32 i = 0; i < nMaxCons; i++)
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


