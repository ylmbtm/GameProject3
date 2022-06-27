#ifndef __COMMON_SOCKET__
#define __COMMON_SOCKET__

#define SOCKET_ERROR        (-1)

namespace CommonSocket
{
//设置套接字为可重用状态
BOOL        SetSocketReuseable(SOCKET hSocket);

//设置套接字为非阻塞状态
BOOL        SetSocketBlock(SOCKET hSocket, BOOL bBlock);

BOOL        SetSocketNoDelay(SOCKET hSocket);

BOOL        SetSocketKeepAlive( SOCKET hSocket, int nKeepInterval, int nKeepCount, int nKeepIdle );

BOOL        InitNetwork();

BOOL        UninitNetwork();

SOCKET      CreateSocket( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0);

BOOL        BindSocket( SOCKET hSocket, const struct sockaddr* pAddr, int nNamelen);

BOOL        ListenSocket( SOCKET hSocket, int nBacklog);

BOOL        ConnectSocket(SOCKET hSocket, const char* pAddr, short sPort);

INT32       GetSocketLastError();

BOOL        IsSocketValid(SOCKET hSocket);

void        ShutdownSend(SOCKET hSocket);

void        ShutdownRecv(SOCKET hSocket);

void        _ShutdownSocket(SOCKET hSocket);

void        CloseSocket(SOCKET hSocket);

std::string GetLocalIP();

UINT32      IpAddrStrToInt(CHAR* pszIpAddr);

UINT32      IpAddrStrToInt(const CHAR* pszIpAddr);

std::string IpAddrIntToStr(UINT32 dwIpAddr);

BOOL        SetSocketBuffSize(SOCKET hSocket, INT32 nRecvSize, INT32 nSendSize);

BOOL        SetSocketTimeOut(SOCKET hSocket, INT32 nSendTime, INT32 nRecvTime);

std::string GetRemoteIP(SOCKET hSocket);

UINT32      HostToNet(INT32 nValue);

UINT32      NetToHost(INT32 nValue);

VOID        IgnoreSignal();

std::string HttpGet(std::string strHost, INT32 nPort, std::string strPath, std::string strContent);

#ifdef WIN32
BOOL        ConnectSocketEx(SOCKET hSocket, const char* pAddr, short sPort, LPOVERLAPPED lpOverlapped);

BOOL        AcceptSocketEx(SOCKET hListenSocket, SOCKET hAcceptSocket, CHAR* pBuff, LPOVERLAPPED lpOverlapped);

BOOL        GetSocketAddress(SOCKET hSocket, CHAR* pDataBuffer, sockaddr_in*& pAddrClient, sockaddr_in*& pAddrLocal);

BOOL        DisconnectEx(SOCKET hSocket, LPOVERLAPPED lpOverlapped, BOOL bReuse);
#endif
}


#endif /* __COMMON_SOCKET__*/