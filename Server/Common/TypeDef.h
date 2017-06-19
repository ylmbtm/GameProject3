#pragma once
#ifndef __COMMON_DATA_TYPE__
#define __COMMON_DATA_TYPE__

#define TRUE			1
#define FALSE			0

#ifdef WIN32


#else

typedef char			INT8;
typedef unsigned char	UINT8;
typedef short			INT16;
typedef unsigned short  UINT16;
typedef int				INT32;
typedef unsigned int	UINT32;
typedef int				BOOL;
typedef void			VOID;
typedef float			FLOAT;
typedef double			DOUBLE;
typedef long long       INT64;
typedef unsigned long long UINT64;
typedef char			BYTE;
typedef char			CHAR;

typedef int				SOCKET;
typedef int				HANDLE;

#define INVALID_SOCKET	(-1)
#define MAXEPOLLSIZE	10000


#endif

#endif /* __COMMON_DATA_TYPE__*/