#ifndef __COMMON_SOCKET__
#define __COMMON_SOCKET__

#define SOCKET_ERROR 		(-1)

namespace CommonSocket
{
	//设置套接字为可重用状态
	bool		SetSocketReuseable(SOCKET hSocket);

    //设置套接字为非阻塞状态
	bool		SetSocketUnblock(SOCKET hSocket);

	 //设置套接字为阻塞状态
	bool		SetSocketBlock(SOCKET hSocket);

	bool		SetSocketNoDelay(SOCKET hSocket);

	bool		SetSocketKeepAlive( SOCKET hSocket, int keepInterval, int keepCount, int keepIdle );

	//初始化网络
	bool		InitNetwork();

	//反初始化网络
	bool		UninitNetwork(); 

	SOCKET		CreateSocket( int af, int type, int protocol);

	BOOL		BindSocket( SOCKET hSocket, const struct sockaddr *pAddr, int nNamelen);

	BOOL		ListenSocket( SOCKET hSocket, int nBacklog);

	BOOL		ConnectSocket(SOCKET hSocket, const char *pAddr, short sPort);

	INT32		GetSocketLastError();

	BOOL		IsSocketValid(SOCKET hSocket);

	//关闭套接字发送
	void		ShutDownSend(SOCKET hSocket);

	//关闭套接字接收
	void		ShutDownRecv(SOCKET hSocket);

	//关闭套接字
	void		CloseSocket(SOCKET hSocket);

	std::string GetLastErrorStr(INT32 nError);

	UINT32		IpAddrStrToInt(CHAR *pszIpAddr);

	std::string IpAddrIntToStr(UINT32 dwIpAddr);
	
#ifdef WIN32
	BOOL		ConnectSocketEx(SOCKET hSocket, const char *pAddr, short sPort, LPOVERLAPPED lpOverlapped);
#endif

}


#endif /* __COMMON_SOCKET__*/