#ifndef _COMMON_WORK_THREAD_H_
#define _COMMON_WORK_THREAD_H_
#include "Utility/CommonThreadFunc.h"
#include "Utility/CommonMsgQueue.h"
#include "IBufferHandler.h"
#include "Utility/LockFreeQueue.h"

Th_RetName _CommonWorkThread( void *pParam );

struct MsgItem
{
	MsgItem(UINT64 _u64ConnID = 0, IDataBuffer *pBuffer = NULL)
	{
		u64ConnID = _u64ConnID;

		pDataBuffer = pBuffer;
	}

	UINT64		u64ConnID;

	IDataBuffer *pDataBuffer;
};

class CCommonWorkThread
{
public:
	CCommonWorkThread();

	~CCommonWorkThread();

	BOOL			Start();

	BOOL			Stop();

	void			Run();

	BOOL			ProcessTimeEvent();

	BOOL			ProcessMessage();

	BOOL			AddMessage(UINT64 u64ConnID, IDataBuffer *pDataBuffer);

	BOOL			SetCommandHandler(IThreadCommandHandler *pCommandHandler);

	BOOL			OnThreadBegin();

	BOOL			OnThreadEnd();

protected:
	THANDLE					m_hThread;

	BOOL					m_bRun;

	IThreadCommandHandler	*m_pCommandHandler;

	ArrayLockFreeQueue<MsgItem>   m_MessageQueue;

	UINT32					m_dwLastTick;

	
};

#endif //_COMMON_WORK_THREAD_H_
