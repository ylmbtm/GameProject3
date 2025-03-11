#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_
#include "IBufferHandler.h"
#include "boost/system/error_code.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/ip/udp.hpp"
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include "boost/asio/io_service.hpp"
#include "boost/asio/connect.hpp"
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

public:
    void HandleConnect(CConnection* pConnection, const boost::system::error_code& e);

    void HandleAccept(CConnection* pConnection, const boost::system::error_code& e);

    BOOL PostSendOperation(CConnection* pConnection);

    BOOL WaitForConnect();
public:
    boost::asio::ip::tcp::acceptor* m_pAcceptor;
    boost::asio::io_service         m_IoService;
    boost::thread*                  m_pWorkThread;

    //包序号检测
    BOOL                m_bPacketNoCheck;
    IDataHandler*                   m_pBufferHandler;
};

#endif

