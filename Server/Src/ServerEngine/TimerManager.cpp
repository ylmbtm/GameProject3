#include "stdafx.h"
#include "TimerManager.h"
#include "../../Common/Utility/CommonFunc.h"



TimerManager::TimerManager()
{
	m_pHead = NULL;

	m_pFree = NULL;

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
	if(m_pHead == NULL)
	{
		return TRUE;
	}

	TimeEvent *pDelEvent = m_pHead;
	while(pDelEvent != NULL)
	{
		if((pDelEvent->m_dwSec == dwSec)&&(pDelEvent->m_dwData == dwData))
		{
			break;
		}

		pDelEvent = pDelEvent->m_pNext;
	}


	if(pDelEvent == m_pHead)
	{
		m_pHead = m_pHead->m_pNext;
		if(m_pHead != NULL)
		{
			m_pHead->m_pPrev = NULL;
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

	pDelEvent->m_pNext = m_pFree;
	pDelEvent->m_pPrev = NULL;
	
	if(m_pFree != NULL)
	{
		m_pFree->m_pPrev = pDelEvent;
	}

	m_pFree = pDelEvent;

	pDelEvent->Reset();

	return FALSE;
}

VOID TimerManager::UpdateTimer()
{
	m_dwCurTime = CommonFunc::GetCurrTime();
	TimeEvent *pCurEvent = m_pHead;
	while(pCurEvent != NULL)
	{
		if(m_dwCurTime >= pCurEvent->m_dwFireTime)
		{
			//避免每次启动服务器，之前的定时器都执行一遍
			if(m_dwInitTime <= pCurEvent->m_dwFireTime)
			{
				OnTimerEvent(pCurEvent);
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

		if(pCurEvent->m_dwRepeateTimes <= 0)
		{
			
		}


		pCurEvent = pCurEvent->m_pNext;
	}
}

VOID TimerManager::OnTimerEvent(TimeEvent *pEvent)
{
	if(pEvent == NULL)
	{
		return ;
	}
	
	(*pEvent->m_pTimerFuncSlot)(pEvent->m_dwData);

	return ;
}

BOOL TimerManager::InitTimer()
{
	m_dwInitTime = CommonFunc::GetCurrTime();
	return TRUE;
}

BOOL TimerManager::Clear()
{
	while(m_pHead != NULL)
	{
		TimeEvent *pCurEvent = m_pHead;
		m_pHead = pCurEvent->m_pNext;
		delete pCurEvent;
	}

	return TRUE;
}
