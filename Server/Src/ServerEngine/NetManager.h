#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_
#include "IBufferHandler.h"
#include "Connection.h"

class CNetManager
{
    CNetManager(void);

    virtual ~CNetManager(void);
public:
    static CNetManager* GetInstancePtr()
    {
        static CNetManager NetManager;

        return &NetManager;
    }
public:
    BOOL    Start(UINT16 nPortNum,  INT32 nMaxConn, IDataHandler* pBufferHandler, std::string strListenIp);

    BOOL    Stop();

    BOOL    SendMessageData(INT32 nConnID,  INT32 nMsgID, UINT64 u64TargetID, INT32 nUserData,  const char* pData, INT32 nLen);

    BOOL    SendMessageBuff(INT32 nConnID, IDataBuffer* pBuffer);

    CConnection* ConnectTo_Sync(std::string strIpAddr, UINT16 sPort);

    CConnection* ConnectTo_Async(std::string strIpAddr, UINT16 sPort);

    BOOL    EnableCheck(BOOL bCheck);

    //以下是完成端口部分
public:
    BOOL    InitNetwork();

    BOOL    UninitNetwork();

    BOOL    StartNetListen(UINT16 nPortNum, std::string strIpAddr);

    BOOL    StopListen();

    BOOL    CreateCompletePort();

    BOOL    DestroyCompletePort();

    BOOL    CreateEventThread(INT32 nNum);

    BOOL    CloseEventThread();

    BOOL    WorkThread_ProcessEvent(INT32 nParam);

    BOOL    WorkThread_Listen();

    BOOL    EventDelete(CConnection* pConnection);

    BOOL    PostSendOperation(CConnection* pConnection);

    CConnection*    AssociateCompletePort(SOCKET hSocket, BOOL bConnect);

    BOOL            WaitForConnect();
public:
    SOCKET              m_hListenSocket;
    NetIoOperatorData   m_IoOverlapAccept;
    SOCKET              m_hCurAcceptSocket;
    HANDLE              m_hCompletePort;
    CHAR                m_AddressBuf[128];
    BOOL                m_bCloseEvent;      //是否关闭事件处理线程

    //包序号检测
    BOOL                m_bPacketNoCheck;

    IDataHandler*       m_pBufferHandler;
    std::vector<std::thread*> m_vtEventThread;

};

#endif

