#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IBufferHandler.h"
#include "ReaderWriterQueue.h"

#define  NET_OP_RECV                1
#define  NET_OP_SEND                2
#define  NET_OP_CONNECT             3
#define  NET_OP_ACCEPT              4
#define  NET_OP_POST                5
#define  NET_OP_UDP_RECV            6

#define RECV_BUF_SIZE               8192
#define MAX_BUFF_SIZE               32768

#define E_SEND_SUCCESS              1
#define E_SEND_UNDONE               2
#define E_SEND_ERROR                3

enum ENetStatus
{
    ENS_INIT       = 1,
    ENS_CONNECTING = 2,
    ENS_CONNECTED  = 3,
    ENS_CLOSEING   = 4,
    ENS_CLOSED     = 5
};

struct NetIoOperatorData
{
#ifdef WIN32
    OVERLAPPED      Overlap;
#endif
    INT32           nOpType;
    INT32           nConnID;

    IDataBuffer*    pDataBuffer;

    void            Reset();
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

    BOOL    Shutdown();

    BOOL    Close();

    BOOL    SetSocket(SOCKET hSocket);

    SOCKET  GetSocket();

    BOOL    SetDataHandler(IDataHandler* pHandler);

    BOOL    ExtractBuffer();

    BOOL    DoReceive();

    ENetStatus GetConnectStatus();

    BOOL    SetConnectStatus(ENetStatus eConnStatus);

    BOOL    Reset();

    BOOL    SendBuffer(IDataBuffer* pBuff);

    BOOL    DoSend();

    BOOL    CheckHeader(CHAR* pNetPacket);

    BOOL    UpdateCheckNo(CHAR* pNetPacket);

    UINT32  GetIpAddr(BOOL bHost = TRUE);

    VOID    EnableCheck(BOOL bCheck);

public:
    SOCKET                      m_hSocket;

    ENetStatus                  m_eConnStatus;

    BOOL                        m_bNotified;

    BOOL                        m_bPacketNoCheck;

    NetIoOperatorData           m_IoOverlapRecv;

    NetIoOperatorData           m_IoOverlapSend;

    NetIoOperatorData           m_IoOverLapPost;

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

    sockaddr                    m_UdpAddr;

    //LINUX下专用， 用于发了一半的包
    IDataBuffer*                m_pSendingBuffer;
    INT32                       m_nSendingPos;
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