#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <errno.h>

#if defined(WIN32) || defined(WIN64)
#define OS_PLATFORM_WINDOWS
#elif defined(LINUX32) || defined(LINUX64)
#define OS_PLATFORM_LINUX
#endif


#ifdef OS_PLATFORM_WINDOWS
#include <io.h>
#include <direct.h>
#include <process.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <mswsock.h>
#include <time.h>
#include <stdarg.h>
#pragma  comment(lib,"ws2_32")
#pragma  comment(lib, "Mswsock")

#else

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <string.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>

#define WSAEWOULDBLOCK 		EWOULDBLOCK
#define WSAEINPROGRESS 		EINPROGRESS
#define WSAEALREADY 		EALREADY
#define WSAENOTSOCK 		ENOTSOCK
#define WSAEDESTADDRREQ 	EDESTADDRREQ
#define WSAEMSGSIZE 		EMSGSIZE
#define WSAEPROTOTYPE 		EPROTOTYPE
#define WSAENOPROTOOPT 		ENOPROTOOPT
#define WSAEPROTONOSUPPORT 	EPROTONOSUPPORT
#define WSAESOCKTNOSUPPORT 	ESOCKTNOSUPPORT
#define WSAEOPNOTSUPP 		EOPNOTSUPP
#define WSAEPFNOSUPPORT 	EPFNOSUPPORT
#define WSAEAFNOSUPPORT 	EAFNOSUPPORT
#define WSAEADDRINUSE 		EADDRINUSE
#define WSAEADDRNOTAVAIL 	EADDRNOTAVAIL
#define WSAENETDOWN 		ENETDOWN
#define WSAENETUNREACH 		ENETUNREACH
#define WSAENETRESET 		ENETRESET
#define WSAECONNABORTED 	ECONNABORTED
#define WSAECONNRESET 		ECONNRESET
#define WSAENOBUFS 			ENOBUFS
#define WSAEISCONN 			EISCONN
#define WSAENOTCONN 		ENOTCONN
#define WSAESHUTDOWN 		ESHUTDOWN
#define WSAETOOMANYREFS 	ETOOMANYREFS
#define WSAETIMEDOUT		ETIMEDOUT
#define WSAECONNREFUSED 	ECONNREFUSED
#define WSAELOOP 			ELOOP
#define WSAENAMETOOLONG 	ENAMETOOLONG
#define WSAEHOSTDOWN 		EHOSTDOWN
#define WSAEHOSTUNREACH 	EHOSTUNREACH
#define WSAENOTEMPTY 		ENOTEMPTY
#define WSAEPROCLIM 		EPROCLIM
#define WSAEUSERS 			EUSERS
#define WSAEDQUOT 			EDQUOT
#define WSAESTALE 			ESTALE
#define WSAEREMOTE 			EREMOTE

typedef char				INT8;
typedef unsigned char		UINT8;
typedef short				INT16;
typedef unsigned short		UINT16;
typedef int					INT32;
typedef unsigned int		UINT32;
typedef int					BOOL;
typedef void				VOID;
typedef float				FLOAT;
typedef double				DOUBLE;
typedef long long			INT64;
typedef unsigned long long	UINT64;
typedef char				BYTE;
typedef char				CHAR;
typedef int					SOCKET;
typedef int					HANDLE;

#define INVALID_SOCKET		(-1)
#define TRUE				1
#define FALSE				0
#define INVALID_HANDLE_VALUE 0
#endif

#ifdef OS_PLATFORM_WINDOWS
#ifdef SERVER_ENGINE
#define EngineClass _declspec(dllexport)
#else
#define EngineClass _declspec(dllimport)
#endif
#else
#define EngineClass
#endif

#endif /* __PLATFORM_H__ */