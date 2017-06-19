#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "TypeDef.h"
#include "GameDefine.h"
#include <errno.h>

#ifdef WIN32
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
#endif

#ifdef WIN32
#ifdef SERVER_ENGINE
#define EngineClass _declspec(dllexport)
#else
#define EngineClass _declspec(dllimport)
#endif
#else
#define EngineClass 
#endif

#endif /* __PLATFORM_H__ */