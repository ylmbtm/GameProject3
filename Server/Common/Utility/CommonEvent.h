//------------------------------------------------------------------------------
// File: bThdUtil.h
//------------------------------------------------------------------------------
#ifndef __COMMONEVENT__
#define __COMMONEVENT__

#ifdef WIN32
// wrapper for whatever critical section we have
class CCommonEvent 
{
    // make copy constructor and assignment operator inaccessible

    CCommonEvent(const CCommonEvent &refEvent);
    CCommonEvent &operator=(const CCommonEvent &refEvent);

    HANDLE m_hEvent;

public:
    CCommonEvent() 
	{

    };

    ~CCommonEvent() 
	{
		CloseHandle(m_hEvent);
    };

	BOOL InitEvent(BOOL bManualReset, BOOL bInitialState)
	{
		if((m_hEvent = CreateEvent(NULL, bManualReset, bInitialState, NULL)) == NULL)
		{
			return FALSE;
		}

		return TRUE;
	}

    void SetEvent() 
	{
		::SetEvent(m_hEvent);
    }

    INT32 Wait() 
	{
        DWORD dwRet = WaitForSingleObject(m_hEvent, INFINITE);
		if (dwRet == WAIT_OBJECT_0)
		{
			return 0;
		}

		if (dwRet == WAIT_TIMEOUT)
		{
			return 1;
		}

		return -1;
    };

	INT32 Wait(UINT32 dwMS)
	{
		DWORD dwRet = WaitForSingleObject(m_hEvent, dwMS);
		if (dwRet == WAIT_OBJECT_0)
		{
			return 0;
		}

		if (dwRet == WAIT_TIMEOUT)
		{
			return 1;
		}

		return -1;
	}

};

#else //LINUX

#include <pthread.h>
class CCommonEvent 
{
    // make copy constructor and assignment operator inaccessible

	CCommonEvent(const CCommonEvent &refEvent);
	CCommonEvent &operator=(const CCommonEvent &refEvent);

    pthread_mutex_t mutex;

	pthread_cond_t  cond;

	BOOL bManualReset, bInitialState;


public:
	CCommonEvent() 
	{
		
	};

	~CCommonEvent() 
	{
		pthread_mutex_destroy (&mutex);

		pthread_cond_destroy(&cond);
	};

	BOOL InitEvent(BOOL bManualReset, BOOL bInitialState)
	{
		pthread_mutex_init (&mutex, NULL);

		pthread_cond_init(&cond, NULL); 

		bManualReset = bManualReset;

		bInitialState = bInitialState;

		return TRUE;
	}

	void SetEvent() 
	{
		pthread_cond_broadcast(&cond);
	};

	void Wait() 
	{
		pthread_mutex_lock (&mutex);
		pthread_cond_wait(&cond, &mutex);
		pthread_mutex_unlock (&mutex);
	};

	void Wait(UINT32 dwMS)
	{

	}

};
#endif  //#ifdef WINDOWS

#endif /* __COMMONEVENT__ */

