//------------------------------------------------------------------------------
// File: bThdUtil.h
//------------------------------------------------------------------------------
#ifndef __BTHDUTIL__
#define __BTHDUTIL__

#ifdef WIN32
// wrapper for whatever critical section we have
class CCritSec 
{
private:
    // make copy constructor and assignment operator inaccessible

    CCritSec(const CCritSec &refCritSec);
    CCritSec &operator=(const CCritSec &refCritSec);

    CRITICAL_SECTION m_CritSec;

public:
    CCritSec() {
        InitializeCriticalSection(&m_CritSec);
    };

    ~CCritSec() {
        DeleteCriticalSection(&m_CritSec);
    };

    void Lock() {
        EnterCriticalSection(&m_CritSec);
    };

    void Unlock() {
        LeaveCriticalSection(&m_CritSec);
    };

};

#else //LINUX

#include <pthread.h>
class CCritSec 
{
    // make copy constructor and assignment operator inaccessible

    CCritSec(const CCritSec &refCritSec);
    CCritSec &operator=(const CCritSec &refCritSec);

    pthread_mutex_t mutex;

public:
	CCritSec() 
	{
		pthread_mutex_init (&mutex, NULL);
	};

	~CCritSec() 
	{
		pthread_mutex_destroy (&mutex);
	};

	bool Lock() 
	{
		int rc = pthread_mutex_lock (&mutex);
		if (rc)
		{
			return FALSE;
		}
		return TRUE;
	};

	bool TryLock()
	{

		int rc = pthread_mutex_trylock (&mutex);
		if (rc)
		{
			return FALSE;
		}

		return TRUE;			
	}
	bool Unlock() 
	{
		int rc = pthread_mutex_unlock (&mutex);
		if (rc)
		{
			return FALSE;
		}

		return TRUE;
	};
};
#endif  //#ifdef WINDOWS

// locks a critical section, and unlocks it automatically
// when the lock goes out of scope
class CAutoLock {

    // make copy constructor and assignment operator inaccessible

    CAutoLock(const CAutoLock &refAutoLock);
    CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
    CCritSec * m_pLock;
public:
    CAutoLock(CCritSec * plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    };
    ~CAutoLock() 
   {
	if(m_pLock)
       	m_pLock->Unlock();
    };
};

#endif /* __BTHDUTIL__ */

