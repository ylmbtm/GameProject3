#include "stdafx.h"
#include "TimerManager.h"


TimerManager::TimerManager()
{
	m_pUsedHead = NULL;

	m_pFreeHead = NULL;

	m_dwInitTime = CommonFunc::GetCurrTime();
}

TimerManager::~TimerManager()
{

}

TimerManager* TimerManager::GetInstancePtr()
{
	static TimerManager _TimerManager;

	return &_TimerManager;
}

BOOL TimerManager::DelTimer(UINT32 dwSec, UINT32 dwData)
{
	if(m_pUsedHead == NULL)
	{
		return TRUE;
	}

	TimeEvent* pDelEvent = m_pUsedHead;
	while(pDelEvent != NULL)
	{
		if((pDelEvent->m_dwSec == dwSec) && (pDelEvent->m_dwData == dwData))
		{
			break;
		}

		pDelEvent = pDelEvent->m_pNext;
	}

	if(pDelEvent == m_pUsedHead)
	{
		m_pUsedHead = m_pUsedHead->m_pNext;
		if(m_pUsedHead != NULL)
		{
			m_pUsedHead->m_pPrev = NULL;
		}
	}
	else
	{
		pDelEvent->m_pPrev->m_pNext = pDelEvent->m_pNext;
		if(pDelEvent->m_pNext != NULL)
		{
			pDelEvent->m_pNext->m_pPrev = pDelEvent->m_pPrev;
		}
	}

	pDelEvent->m_pNext = m_pFreeHead;
	pDelEvent->m_pPrev = NULL;

	if(m_pFreeHead != NULL)
	{
		m_pFreeHead->m_pPrev = pDelEvent;
	}

	m_pFreeHead = pDelEvent;

	pDelEvent->Reset();

	return FALSE;
}

VOID TimerManager::UpdateTimer()
{
	m_dwCurTime = CommonFunc::GetCurrTime();
	TimeEvent* pCurEvent = m_pUsedHead;
	while(pCurEvent != NULL)
	{
        BOOL bRet = TRUE;
		if(m_dwCurTime >= pCurEvent->m_dwFireTime)
		{
			//避免每次启动服务器，之前的定时器都执行一遍
			if(m_dwInitTime <= pCurEvent->m_dwFireTime)
			{
                bRet = OnTimerEvent(pCurEvent);
			}

			pCurEvent->m_dwRepeateTimes -= 1;

			if(pCurEvent->m_dwType == 1)
			{
				pCurEvent->m_dwFireTime = pCurEvent->m_dwFireTime + 86400;
			}
			else
			{
				pCurEvent->m_dwFireTime = pCurEvent->m_dwFireTime + pCurEvent->m_dwSec;
			}
		}

		if(pCurEvent->m_dwRepeateTimes <= 0 || !bRet)
		{
			//首先从己用中删除
			if (pCurEvent == m_pUsedHead)
			{
				//自己是首结点
				m_pUsedHead = pCurEvent->m_pNext;
				if (m_pUsedHead != NULL)
				{
					m_pUsedHead->m_pPrev = NULL;
				}
			}
			else
			{
				ERROR_RETURN_NONE(pCurEvent->m_pPrev != NULL);
				pCurEvent->m_pPrev->m_pNext = pCurEvent->m_pNext;
				if (pCurEvent->m_pNext != NULL)
				{
					pCurEvent->m_pNext->m_pPrev = pCurEvent->m_pPrev;
				}
			}

			TimeEvent* pDelEvent = pCurEvent;

			pCurEvent = pCurEvent->m_pNext;

			if (m_pFreeHead != NULL)
			{
				m_pFreeHead->m_pPrev = pDelEvent;
			}

			m_pFreeHead = pDelEvent;

			pDelEvent->Reset();
		}
		else
		{
			pCurEvent = pCurEvent->m_pNext;
		}
	}
}

BOOL TimerManager::OnTimerEvent(TimeEvent* pEvent)
{
	if(pEvent == NULL)
	{
		return FALSE;
	}

	BOOL bRet = (*pEvent->m_pTimerFuncSlot)(pEvent->m_dwData);

	//如要定时器返回FALSE, 表示需要被删除

	return bRet;
}

BOOL TimerManager::InitTimer()
{
	m_dwInitTime = CommonFunc::GetCurrTime();
	return TRUE;
}

BOOL TimerManager::Clear()
{
	while(m_pUsedHead != NULL)
	{
		TimeEvent* pCurEvent = m_pUsedHead;
		m_pUsedHead = pCurEvent->m_pNext;
		delete pCurEvent;
	}

	return TRUE;
}
