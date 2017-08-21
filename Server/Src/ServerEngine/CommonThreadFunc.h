#ifndef __COMMON_THREAD_FUNC__
#define __COMMON_THREAD_FUNC__

#ifndef WIN32
#include <pthread.h>
typedef pthread_t THANDLE;
#define Th_RetName void*
#define Th_RetValue NULL
#else
#include <process.h>
typedef HANDLE THANDLE;
#define Th_RetName void
#define Th_RetValue
#endif

typedef Th_RetName *ThreadFunc( void *pParam );

namespace CommonThreadFunc
{
	THANDLE		CreateThread( Th_RetName (*pThreadFunc)(void *),  void *pArg);

	VOID		ExitThread();

	BOOL		WaitThreadExit(THANDLE hThread);

	VOID		Sleep(UINT32 dwMilliseconds);
}


#endif /* __COMMON_THREAD_FUNC__ */