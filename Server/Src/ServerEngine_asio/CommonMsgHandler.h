#ifndef _COMMON_MSG_HANDLER_H_
#define _COMMON_MSG_HANDLER_H_
#include "CommonWorkThread.h"
#include "DataBuffer.h"

class CCommonMsgHandler : public IThreadCommandHandler
{
public:
	CCommonMsgHandler();

	~CCommonMsgHandler();

	BOOL OnMessageHandle(UINT32 dwMsgID, UINT64 u64ConnID, IDataBuffer *pDataBuffer);

	BOOL OnUpdate(UINT32 dwTick);

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	BOOL AddMessage(UINT64 u64ConnID, IDataBuffer *pDataBuffer);

	BOOL OnThreadBegin(){return TRUE;}

	BOOL OnThreadEnd(){return TRUE;}
	

protected:
	CCommonWorkThread				m_WorkThread;

	CDataBuffer<CONST_BUFF_SIZE>	m_WriteBuffer;

	//*********************消息处理定义开始******************************
public:

	//*********************消息处理定义结束******************************
};

#endif //_COMMON_MSG_HANDLER_H_
