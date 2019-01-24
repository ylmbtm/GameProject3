#ifndef __SPIN_LOCK_h__
#define __SPIN_LOCK_h__
#include <atomic>

class CSpinLock
{
	std::atomic_flag m_flag;
public:
	CSpinLock()
	{
		
	};

	~CSpinLock()
	{
	};

	void Lock()
	{
		for (unsigned k = 0; !TryLock(); ++k)
		{
			if (k % 64 == 0)
			{
#ifdef WIN32
				::Sleep(0);
#else
				struct timespec req;
				req.tv_sec = 0;
				req.tv_nsec = 0;
				if (-1 == nanosleep(&req, NULL))
				{
					return;
				}
#endif
			}
		}

		return ;
	}

	bool TryLockTimes(unsigned nTimes)
	{
		for (unsigned k = 0; !TryLock(); ++k)
		{
			if (k >= nTimes)
			{
				return false;
			}
		}

		return true;
	}

	bool TryLock()
	{
		if (m_flag.test_and_set(std::memory_order_acquire))
		{
			return true;
		}

		return false;
	}

	void Unlock()
	{
		m_flag.clear(std::memory_order_release);
	
		return ;
	}

};

#endif /* __SPIN_LOCK_h__ */

