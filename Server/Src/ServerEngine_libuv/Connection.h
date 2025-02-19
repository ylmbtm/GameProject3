#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IBufferHandler.h"
#include "ReaderWriterQueue.h"

#define RECV_BUF_SIZE               8192

enum ENetStatus
{
    ENS_INIT       = 1,
    ENS_CONNECTING = 2,
    ENS_CONNECTED  = 3,
    ENS_CLOSEING   = 4,
    ENS_CLOSED     = 5
};

class CConnection
{
public:
    CConnection();
    virtual ~CConnection();

public:
    BOOL    HandleRecvEvent(INT32 nBytes);

    INT32   GetConnectionID();

    UINT64  GetConnectionData();

    VOID    SetConnectionID(INT32 nConnID);

    VOID    SetConnectionData(UINT64 uData);

    BOOL    Close();

    BOOL    SetSocket(SOCKET hSocket);

    uv_tcp_t*  GetSocket();

    BOOL    SetDataHandler(IDataHandler* pHandler);

    BOOL    ExtractBuffer();

    BOOL    DoReceive();

    ENetStatus GetConnectStatus();

    BOOL    SetConnectStatus(ENetStatus eConnStatus);

    BOOL    Reset();

    BOOL    SendBuffer(IDataBuffer* pBuff);

    BOOL    DoSend();

    void    HandReaddata(size_t len);

    void    HandWritedata(size_t len);

    BOOL    CheckHeader(CHAR* pNetPacket);

    BOOL    UpdateCheckNo(CHAR* pNetPacket);

    INT32   GetIpAddr(BOOL bHost = TRUE);

    VOID    EnableCheck(BOOL bCheck);
public:
    uv_tcp_t                    m_hSocket;
    uv_connect_t                m_ConnectReq;
    uv_write_t                  m_WriteReq;
    uv_shutdown_t               m_ShutdownReq;
    uv_async_t                  m_AsyncReq;

    ENetStatus                  m_eConnStatus;

    BOOL                        m_bPacketNoCheck;

    INT32                       m_nConnID;
    UINT64                      m_uConnData;

    IDataHandler*               m_pDataHandler;

    INT32                       m_nIpAddr;

    INT32                       m_nDataLen;
    CHAR                        m_pRecvBuf[RECV_BUF_SIZE];
    CHAR*                       m_pBufPos;

    IDataBuffer*                m_pCurRecvBuffer;
    INT32                       m_nCurBufferSize;
    INT32                       m_nCheckNo;

    volatile BOOL               m_IsSending;

    CConnection*                m_pNext;

    UINT64                      m_uLastRecvTick;

    moodycamel::ReaderWriterQueue< IDataBuffer*> m_SendBuffList;

    IDataBuffer*                m_pSendingBuffer;


};


class CConnectionMgr
{
private:
    CConnectionMgr();

    ~CConnectionMgr();

public:
    static CConnectionMgr* GetInstancePtr();

public:
    BOOL            InitConnectionList(INT32 nMaxCons);

    CConnection*    CreateConnection();

    BOOL            DeleteConnection(CConnection* pConnection);

    BOOL            DeleteConnection(INT32 nConnID);

    CConnection*    GetConnectionByID(INT32 nConnID);

    ///////////////////////////////////////////
    BOOL            CloseAllConnection();

    BOOL            DestroyAllConnection();

    BOOL            CheckConntionAvalible(INT32 nInterval);

public:

    CConnection*                m_pFreeConnRoot;
    CConnection*                m_pFreeConnTail;
    std::vector<CConnection*>   m_vtConnList;            //连接列表
    std::mutex                  m_ConnListMutex;
};

#endif