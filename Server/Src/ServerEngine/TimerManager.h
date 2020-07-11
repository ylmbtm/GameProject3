#ifndef _TIMER_MANAGER__
#define _TIMER_MANAGER__
#include "CommonFunc.h"
class CTimerSlotBase
{
public:
	virtual ~CTimerSlotBase() {}
	virtual BOOL operator()(UINT32 pData)
	{
		return TRUE;
	}
	virtual VOID* GetThisAddr()
	{
		return 0;
	}
};

template<typename T>
class CTimerSlot : public CTimerSlotBase
{
	typedef BOOL (T::*FuncType)(UINT32);
public:
	CTimerSlot(BOOL (T::*FuncType)(UINT32), T* pObj)
		: m_pFuncPtr(FuncType), m_pThis(pObj)
	{

	}
	virtual ~CTimerSlot() {}

	virtual BOOL operator() (UINT32 pData)
	{
		if (m_pThis != NULL && m_pFuncPtr != NULL)
		{
			(m_pThis->*m_pFuncPtr)(pData);
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual VOID* GetThisAddr()
	{
		return reinterpret_cast<VOID*>(m_pThis);
	}

private:
	FuncType m_pFuncPtr;
	T*		m_pThis;
};

struct TimeEvent
{
public:
	TimeEvent()
	{
		m_dwFireTime     = 0;
		m_dwSec			 = 0;
		m_dwData         = 0;
		m_pNext          = NULL;
		m_pPrev          = NULL;
		m_dwRepeateTimes = 0x0FFFFFFF;
		m_pTimerFuncSlot = NULL;
	}

	~TimeEvent()
	{
		Reset();
	}

	void Reset()
	{
		m_dwFireTime     = 0;
		m_dwSec			 = 0;
		m_dwData         = 0;
		m_pNext          = NULL;
		m_pPrev          = NULL;
		m_dwRepeateTimes = 0x0FFFFFFF;
		if(m_pTimerFuncSlot != NULL)
		{
			delete m_pTimerFuncSlot;
			m_pTimerFuncSlot = NULL;
		}
	}

	UINT64 m_dwFireTime;  //触发时间
	UINT32 m_dwSec;
	UINT32 m_dwData;
	TimeEvent* m_pPrev; //前一节点
	TimeEvent* m_pNext; //后一节点
	UINT32  m_dwType;   //事件类型,1 绝对时间定时器,2 相对时间定时器
	INT32   m_dwRepeateTimes;
	CTimerSlotBase* m_pTimerFuncSlot;
};

class EngineClass TimerManager
{
	TimerManager();
	~TimerManager();

public:
	static TimerManager* GetInstancePtr();

public:

	template<typename T>
	BOOL AddFixTimer(UINT32 dwSec, UINT32 dwData, BOOL (T::*FuncPtr)(UINT32), T* pObj)
	{
		TimeEvent* pNewEvent = NULL;
		if(m_pFreeHead == NULL)
		{
			pNewEvent = new TimeEvent;
		}
		else
		{
			pNewEvent = m_pFreeHead;
			m_pFreeHead = m_pFreeHead->m_pNext;
			m_pFreeHead->m_pPrev = NULL;
		}

		pNewEvent->m_pNext = NULL;
		pNewEvent->m_pPrev = NULL;

		pNewEvent->m_dwData = dwData;
		pNewEvent->m_dwFireTime = CommonFunc::GetDayBeginTime() + dwSec;

		pNewEvent->m_dwSec = dwSec;
		pNewEvent->m_dwType = 1;
		pNewEvent->m_pTimerFuncSlot = new CTimerSlot<T>(FuncPtr, pObj);

		if(m_pUsedHead == NULL)
		{
			m_pUsedHead = pNewEvent;
		}
		else
		{
			pNewEvent->m_pNext = m_pUsedHead;
			m_pUsedHead->m_pPrev = pNewEvent;
			m_pUsedHead = pNewEvent;
			m_pUsedHead->m_pPrev = NULL;
		}

		return TRUE;
	}

	template<typename T>
	BOOL AddDiffTimer(UINT32 dwSec, UINT32 dwData, BOOL (T::*FuncPtr)(UINT32), T* pObj)
	{
		TimeEvent* pNewEvent = NULL;
		if(m_pFreeHead == NULL)
		{
			pNewEvent = new TimeEvent;
		}
		else
		{
			pNewEvent = m_pFreeHead;
			m_pFreeHead = m_pFreeHead->m_pNext;
			m_pFreeHead->m_pPrev = NULL;
		}

		pNewEvent->m_pNext = NULL;
		pNewEvent->m_pPrev = NULL;

		pNewEvent->m_dwData = dwData;

		pNewEvent->m_dwFireTime = CommonFunc::GetCurrTime() + dwSec;
		pNewEvent->m_dwSec = dwSec;
		pNewEvent->m_dwType = 2;

		pNewEvent->m_pTimerFuncSlot = new CTimerSlot<T>(FuncPtr, pObj);
		if(m_pUsedHead == NULL)
		{
			m_pUsedHead = pNewEvent;
		}
		else
		{
			pNewEvent->m_pNext = m_pUsedHead;
			m_pUsedHead->m_pPrev = pNewEvent;
			m_pUsedHead = pNewEvent;
			m_pUsedHead->m_pPrev = NULL;
		}

		return TRUE;
	}

	BOOL DelTimer( UINT32 dwSec, UINT32 dwData);

	VOID UpdateTimer();

	BOOL OnTimerEvent( TimeEvent* pEvent );

	BOOL InitTimer();

	BOOL Clear();

	TimeEvent* m_pUsedHead;

	TimeEvent* m_pFreeHead;

	UINT64     m_dwCurTime;

	UINT64     m_dwInitTime;  //定时器开始工作时间(不对开始工作时间之前的定时器发生作用)
public:
};



#endif /* _TIMER_MANAGER__ */
