
#ifndef __CCRITSECT_NOTIFY__
#define __CCRITSECT_NOTIFY__

#ifdef WIN32
// wrapper for whatever critical section we have
class CCritSecNotify 
{
private:
    // make copy constructor and assignment operator inaccessible

    CCritSecNotify(const CCritSecNotify &refCritSec);
    CCritSecNotify &operator=(const CCritSecNotify &refCritSec);

    CRITICAL_SECTION m_CritSec;

	HANDLE			 m_hEvent;

public:
    CCritSecNotify() 
	{
        InitializeCriticalSection(&m_CritSec);

		m_hEvent = CreateEvent(NULL, FALSE, FALSE, "");
    };

    ~CCritSecNotify() 
	{
        DeleteCriticalSection(&m_CritSec);

		CloseHandle(m_hEvent);

    };

    void Lock() 
	{
        EnterCriticalSection(&m_CritSec);
    };

    void Unlock() 
	{
        LeaveCriticalSection(&m_CritSec);
    };


	void Wait(UINT32 dwTime = INFINITE)
	{
		Unlock();
		WaitForSingleObject(m_hEvent, dwTime);
	}


	BOOL Notify()
	{
		return SetEvent(m_hEvent);
	}

};

#else //LINUX
#define INFINITE (0xffffffff)
#include <pthread.h>
class CCritSecNotify 
{
    // make copy constructor and assignment operator inaccessible

    CCritSecNotify(const CCritSecNotify &refCritSec);
    CCritSecNotify &operator=(const CCritSecNotify &refCritSec);

    pthread_mutex_t mutex;

	pthread_cond_t  cond;

public:
	CCritSecNotify() 
	{
		pthread_mutex_init (&mutex, NULL);
	
		pthread_cond_init(&cond, NULL); 
	};

	~CCritSecNotify() 
	{
		pthread_mutex_destroy (&mutex);
		pthread_cond_destroy(&cond);
	};

	bool Lock() 
	{
		if (0 != pthread_mutex_lock (&mutex))
		{
			return FALSE;
		}

		return TRUE;
	};

	bool TryLock()
	{
		if (0 != pthread_mutex_trylock (&mutex))
		{
			return FALSE;
		}

		return TRUE;			
	}
	bool Unlock() 
	{
		if (0 != pthread_mutex_unlock (&mutex))
		{
			return FALSE;
		}

		return TRUE;
	};

	void Wait(UINT32 dwTime = INFINITE)
	{
		if(dwTime == INFINITE)
		{
			pthread_cond_wait(&cond, &mutex);
			pthread_mutex_unlock (&mutex);
		}
		else
		{
			struct timespec abstime;   
			struct timeval tv;   
			gettimeofday(&tv, NULL);   
			abstime.tv_sec  = tv.tv_sec + dwTime / 1000;   
			abstime.tv_nsec = tv.tv_usec*1000 + (dwTime % 1000)*1000000;   
			if (abstime.tv_nsec >= 1000000000)   
			{   
				abstime.tv_nsec -= 1000000000;   
				abstime.tv_sec++;   
			}   
		}
	} 

	BOOL Notify()
	{
		pthread_cond_broadcast(&cond);

		return TRUE;
	}


};
#endif  //#ifdef WINDOWS

#endif /* __CCRITSECT_NOTIFY__ */

