#include "stdafx.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "CommandDef.h"
#include "PacketHeader.h"
#include "CommonSocket.h"

void On_AllocBuff(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{

    CConnection* pConnection = (CConnection*)uv_handle_get_data(handle);

    buf->base = pConnection->m_pRecvBuf + pConnection->m_nDataLen;

    buf->len = RECV_BUF_SIZE - pConnection->m_nDataLen;

    return;
}

void On_ReadData(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    CConnection* pConnection = (CConnection*)uv_handle_get_data((uv_handle_t*)stream);
    if (nread >= 0)
    {
        pConnection->HandReaddata(nread);

        return;
    }

    pConnection->Close();

    return;
}

void On_WriteData(uv_write_t* req, int status)
{
    CConnection* pConnection = (CConnection*)req->data;

    ERROR_RETURN_NONE(pConnection != NULL);

    pConnection->DoSend();

    if (status == 0)
    {
        //成功
    }
    else
    {
        //失败
    }

    return;
}

void On_Shutdown(uv_shutdown_t* req, int status)
{
    CConnection* pConnection = (CConnection*)req->data;

    ERROR_RETURN_NONE(pConnection != NULL);

    if (status == 0)
    {
        //成功
    }
    else
    {
        //失败
    }

    return;
}

void On_Close(uv_handle_t* handle)
{
    CConnection* pConnection = (CConnection*)handle->data;

    ERROR_RETURN_NONE(pConnection != NULL);

    return;
}

CConnection::CConnection()
{
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

    m_hSocket.data      = (void*)this;

    m_bPacketNoCheck    = FALSE;
}

CConnection::~CConnection(void)
{
    Reset();

    m_nConnID         = 0;

    m_pDataHandler    = NULL;

    m_bPacketNoCheck  = FALSE;
}

BOOL CConnection::DoReceive()
{
    ERROR_RETURN_FALSE (0 == uv_read_start((uv_stream_t*)&m_hSocket, On_AllocBuff, On_ReadData))

    return TRUE;
}

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

        if(m_nDataLen < sizeof(PacketHeader))
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
    m_ShutdownReq.data = (void*)this;
    uv_read_stop((uv_stream_t*)&m_hSocket);
    uv_shutdown(&m_ShutdownReq, (uv_stream_t*)&m_hSocket, On_Shutdown);
    uv_close((uv_handle_t*)&m_hSocket, On_Close);
    m_nDataLen         = 0;

    m_IsSending         = FALSE;
    if(m_pDataHandler != NULL)
    {
        m_pDataHandler->OnCloseConnect(GetConnectionID());
    }

    m_eConnStatus = ENS_INIT;

    return TRUE;
}

BOOL CConnection::HandleRecvEvent(INT32 nBytes)
{
    m_nDataLen += nBytes;

    if(!ExtractBuffer())
    {
        return FALSE;
    }

    //if (!DoReceive())
    //{
    //  return FALSE;
    //}

    m_uLastRecvTick = CommonFunc::GetTickCount();
    return TRUE;
}


BOOL CConnection::SetDataHandler( IDataHandler* pHandler )
{
    ERROR_RETURN_FALSE(pHandler != NULL);

    m_pDataHandler = pHandler;

    return TRUE;
}

uv_tcp_t* CConnection::GetSocket()
{
    return &m_hSocket;
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
    m_eConnStatus = ENS_INIT;

    m_uConnData = 0;

    m_nDataLen = 0;

    m_nIpAddr  = 0;

    m_pBufPos   = m_pRecvBuf;

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

INT32 CConnection::GetIpAddr(BOOL bHost)
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

    uv_handle_set_data((uv_handle_t*)&m_WriteReq, (void*)this);
    uv_buf_t buf = uv_buf_init(m_pSendingBuffer->GetBuffer(), m_pSendingBuffer->GetBufferSize());
    uv_write(&m_WriteReq, (uv_stream_t*)&m_hSocket, &buf, 1, On_WriteData);

    return TRUE;
}


void CConnection::HandReaddata(size_t len)
{
    HandleRecvEvent((INT32)len);
}


void CConnection::HandWritedata(size_t len)
{
    DoSend();

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
    ERROR_RETURN_NULL(uv_is_closing((uv_handle_t*)pTemp->GetSocket()));
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

        if (pConn->GetConnectStatus() != ENS_INIT)
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

        if(uCurTick > (pConnection->m_uLastRecvTick + nInterval * 1000))
        {
            CLog::GetInstancePtr()->LogError("CConnectionMgr::CheckConntionAvalible 超时主动断开连接 ConnID:%d", pConnection->GetConnectionID());
            pConnection->Close();
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


