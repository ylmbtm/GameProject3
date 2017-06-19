#include "stdafx.h"
#include "CommonSocket.h"

bool  CommonSocket::SetSocketReuseable(SOCKET hSocket)
{
	char nReuse = 1;

	if(0 != setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nReuse, sizeof(int)))
	{
		return false;
	}

	return true;
}


//设置套接字为非阻塞状态
bool    CommonSocket::SetSocketUnblock(SOCKET hSocket)
{
#ifdef WIN32
	u_long iMode = 1;
	ioctlsocket(hSocket, FIONBIO, &iMode);
#else
	int flags = fcntl(hSocket, F_GETFL, 0);
	fcntl(hSocket, F_SETFL, flags | O_NONBLOCK);
#endif

	return true;
}

bool    CommonSocket::SetSocketBlock(SOCKET hSocket)
{
#ifdef WIN32
	u_long iMode = 0;
	ioctlsocket(hSocket, FIONBIO, &iMode);
#else
	int flags = fcntl(hSocket, F_GETFL, 0);
	fcntl(hSocket, F_SETFL, flags & (~O_NONBLOCK));
#endif

	return true;
}

bool    CommonSocket::SetSocketNoDelay(SOCKET hSocket)
{
	int bOn = 1;

	if(0 != setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&bOn,sizeof(bOn)))
	{
		return false;
	}

	return true;
}


bool   CommonSocket::InitNetwork()
{
#if WIN32
	WSADATA wsaData;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return false;
	}
#endif

	return true;
}

bool   CommonSocket::UninitNetwork()
{
#if WIN32
	return (0 == WSACleanup()); 
#endif

	return true;
}

void   CommonSocket::CloseSocket(SOCKET hSocket)
{
#ifdef WIN32
	closesocket(hSocket);
#else
	close(hSocket);
#endif
}

void   CommonSocket::ShutDownSend(SOCKET hSocket)
{
	shutdown(hSocket, 0);
}

void   CommonSocket::ShutDownRecv(SOCKET hSocket)
{
	shutdown(hSocket, 1);
}


SOCKET	CommonSocket::CreateSocket( int af, int type, int protocol)
{
	return socket(af, type, protocol);
}


BOOL	CommonSocket::BindSocket( SOCKET hSocket, const struct sockaddr *pAddr, int nNamelen)
{
	if(0 != bind(hSocket, pAddr, nNamelen))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL	CommonSocket::ListenSocket( SOCKET hSocket, int nBacklog)
{
	if(0 != listen(hSocket, nBacklog))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL	CommonSocket::ConnectSocket(SOCKET hSocket, const char *pAddr, short sPort)
{
	if(pAddr == NULL)
	{
		return FALSE;
	}

	sockaddr_in  svrAddr;
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port   = htons(sPort);
	inet_pton(AF_INET, pAddr, &svrAddr.sin_addr);

	if(0 == connect(hSocket, (const sockaddr *)&svrAddr, sizeof(sockaddr_in)))
	{
		return TRUE;
	}

	int nError = CommonSocket::GetSocketLastError();
	if((WSAEWOULDBLOCK == nError)||(WSAEINPROGRESS == nError))
	{
		return TRUE;
	}

	return FALSE;
}

INT32   CommonSocket::GetSocketLastError()
{
#ifdef WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}


BOOL CommonSocket::IsSocketValid(SOCKET hSocket)
{
	if((hSocket == 0)||(hSocket == INVALID_SOCKET))
	{
		return FALSE;
	}

	return TRUE;
}


std::string  CommonSocket::GetLastErrorStr(INT32 nError)
{
	std::string strErrorText;
#ifdef WIN32
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,nError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL);

	strErrorText = (LPTSTR)lpMsgBuf;

	LocalFree(lpMsgBuf);
#else
	strErrorText = strerror(nError);
#endif


	return strErrorText;
}

UINT32  CommonSocket::IpAddrStrToInt(CHAR *pszIpAddr)
{
	sockaddr_in SvrAddr;
	
	inet_pton(AF_INET, pszIpAddr, &SvrAddr.sin_addr);

	return SvrAddr.sin_addr.s_addr;
}


#ifdef WIN32
BOOL	CommonSocket::ConnectSocketEx(SOCKET hSocket, const char *pAddr, short sPort, LPOVERLAPPED lpOverlapped)
{
	LPFN_CONNECTEX lpfnConnectEx = NULL;

	DWORD dwBytes;
	GUID GuidConnectEx = WSAID_CONNECTEX;
	if(SOCKET_ERROR == WSAIoctl(hSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&GuidConnectEx, sizeof(GuidConnectEx), 
		&lpfnConnectEx, sizeof(lpfnConnectEx), 
		&dwBytes, NULL, NULL))
	{
		return FALSE;
	}

	sockaddr_in  svrAddr;
	svrAddr.sin_family		= AF_INET;
	svrAddr.sin_port		= htons(0);
	svrAddr.sin_addr.s_addr = INADDR_ANY;  

	CommonSocket::BindSocket(hSocket, (const sockaddr *)&svrAddr, sizeof(sockaddr_in));

	svrAddr.sin_port   = htons(sPort);
	inet_pton(AF_INET, pAddr, &svrAddr.sin_addr);

	if(!lpfnConnectEx(hSocket,(const sockaddr *)&svrAddr, sizeof(sockaddr_in), NULL, NULL, NULL, lpOverlapped))
	{
		if(ERROR_IO_PENDING != CommonSocket::GetSocketLastError())
		{
			return FALSE;
		}
	}

	return TRUE;
}
#endif

std::string CommonSocket::IpAddrIntToStr( UINT32 dwIpAddr )
{
	in_addr Addr;

	Addr.s_addr = dwIpAddr;
	
	CHAR szIpBuffer[20] = {0};
	inet_ntop(AF_INET, &Addr, szIpBuffer, 20);
	return std::string(szIpBuffer);
}

bool CommonSocket::SetSocketKeepAlive( SOCKET hSocket, int keepInterval, int keepCount, int keepIdle )
{
#ifdef WIN32
	BOOL bKeepAlive = TRUE;

	setsockopt(hSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));

#else
	setsockopt(hSocket, SOL_TCP, TCP_KEEPIDLE,  (void *)&keepIdle, sizeof(keepIdle));
	setsockopt(hSocket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
	setsockopt(hSocket, SOL_TCP, TCP_KEEPCNT,   (void *)&keepCount, sizeof(keepCount));
#endif
	

	return true;	
}
