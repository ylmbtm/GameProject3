#ifndef __COMMON_MESSAGE_QUEUE_
#define __COMMON_MESSAGE_QUEUE_
#include "CritSecNotify.h"

namespace CommonQueue
{
	template <typename ELEM_T, INT32 dwSize=512>
	class CMessageQueue
	{
	public:
		CMessageQueue()
		{
			m_nReadPos = 0;

			m_nWritePos = 0;
		}

		~CMessageQueue()
		{
		}

		BOOL Pop(ELEM_T &_Value)
		{
			m_CritSec.Lock();
			if(m_nReadPos == m_nWritePos)
			{
				m_CritSec.Wait();
				return FALSE;
			}

			_Value = m_vtData[m_nReadPos];

			m_nReadPos = (m_nReadPos+1)%dwSize;
			
			m_CritSec.Unlock();
			
			return TRUE;
		}

		BOOL Push(ELEM_T &_Value)
		{
			m_CritSec.Lock();
			if(((m_nWritePos + 1)%dwSize) == m_nReadPos)
			{
				m_CritSec.Unlock();
				ASSERT_FAIELD;
				return FALSE;
			}

			m_vtData[m_nWritePos] = _Value;

			m_nWritePos = (m_nWritePos+1)%dwSize;

			m_CritSec.Unlock();

			m_CritSec.Notify();

			return TRUE; 
		}

		ELEM_T m_vtData[dwSize];

		INT32 m_nWritePos;
		
		INT32 m_nReadPos;

		CCritSecNotify m_CritSec;
	};

}

#endif /* __COMMON_MESSAGE_QUEUE_ */
