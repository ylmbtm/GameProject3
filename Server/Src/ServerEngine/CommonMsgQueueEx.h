#ifndef __COMMON_MESSAGE_QUEUEEX_
#define __COMMON_MESSAGE_QUEUEEX_
#include "CritSecNotify.h"

//这是用双数组来实现


template <typename T>
class CMessageListEx
{
public:
	CMessageListEx()
	{
		m_List_1.clear();
		m_List_2.clear();

		m_pListIn = &m_List_1;
		m_pListOut= &m_List_2;
	}

public:
	T Pop()
	{
		T _Value;

		if(m_pListOut->size() <= m_nOutPos)
		{
			m_CritSec.Lock();

			std::vector<T> *pTemp = m_pListOut;

			m_pListOut = m_pListIn;

			m_pListIn = pTemp;

			m_CritSec.Unlock();

			m_nOutPos = 0;
			
			m_nInPos = 0;

			return _Value;
		}

		_Value = m_pListOut->[m_nOutPos];

		m_nOutPos++;

		return _Value;
	}

	void Push(T &_Value)
	{
		if(m_nInPos >= m_pListIn->size())
		{
			m_pListIn->push_back(_Value);
		}
		else
		{
			m_pListIn->[m_nInPos] = _Value;
			m_nInPos++;
		}
	}

	CCritSecNotify m_CritSec;

	std::vector<T> *m_pListIn;
	std::vector<T> *m_pListOut;

	int m_nInPos, m_nOutPos;

private:
	std::vector<T> m_List_1;
	std::vector<T> m_List_2;
};



#endif /* __COMMON_MESSAGE_QUEUEEX_ */
