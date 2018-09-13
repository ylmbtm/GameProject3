#ifndef __SPIN_LOCK_h__
#define __SPIN_LOCK_h__
#include "atomic_ops.h"

class CSpinLock
{
public:
	CSpinLock()
	{
		m_Value = 0;
	};

	~CSpinLock()
	{
		m_Value = 0;
	};


public:
	UINT64 m_Value;

	void Lock()
	{
		for (unsigned k = 0; !TryLock(); ++k)
		{
			if (k % 32 == 0)
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
		UINT64 nOne = 1, nZero = 0;
		if (CAS(&m_Value, nZero, nOne))
		{
			return true;
		}

		return false;
	}

	void Unlock()
	{
		UINT64 nOne = 1, nZero = 0;
		for (unsigned k = 0; k < 32; ++k)
		{
			if (CAS(&m_Value, nOne, nZero))
			{
				return;
			}
		}

		return ;
	}

};

#endif /* __SPIN_LOCK_h__ */

