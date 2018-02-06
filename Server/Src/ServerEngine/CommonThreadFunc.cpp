#include "stdafx.h"
#include "CommonThreadFunc.h"

THANDLE CommonThreadFunc::CreateThread( Th_RetName (*pThreadFunc) (void*),  void* pArg)
{
	THANDLE hThread = (THANDLE)0;
#ifdef WIN32
	hThread = (THANDLE)_beginthread(pThreadFunc, 0, pArg);
#else
	pthread_create(&hThread, NULL, pThreadFunc, pArg);
#endif
	return hThread;
}

void CommonThreadFunc::ExitThread()
{
#ifdef WIN32
	_endthread();
#else
	pthread_exit(NULL);
#endif
}

BOOL CommonThreadFunc::WaitThreadExit( THANDLE hThread )
{
#ifdef WIN32
	WaitForSingleObject(hThread, INFINITE);
#else
	void* pStatue = NULL;
	pthread_join(hThread, &pStatue);
#endif
	return TRUE;
}



