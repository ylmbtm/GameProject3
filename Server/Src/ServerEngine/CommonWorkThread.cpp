#include "stdafx.h"
#include "CommonWorkThread.h"
#include "CommonEvent.h"
#include "DataBuffer.h"
#include "PacketHeader.h"

CCommonWorkThread::CCommonWorkThread()
{
	m_bRun = FALSE;

	m_dwLastTick = CommonFunc::GetTickCount();
}

CCommonWorkThread::~CCommonWorkThread()
{

}

void CCommonWorkThread::Run()
{
	if (!OnThreadBegin())
	{
		return ;
	}

	while (m_bRun)
	{
		//这就把所有的消息都处理完了
		NetPacket* pPacket = NULL;

		while(m_PacketQueue.pop(pPacket))
		{
			ASSERT(pPacket->m_dwConnID != 0);

			m_pThreadHandler->DispatchPacket(pPacket);

			pPacket->m_pDataBuffer->Release();
		}

		CommonFunc::Sleep(1);
	}

	OnThreadEnd();
}

BOOL CCommonWorkThread::Start()
{
	m_bRun = TRUE;

	m_pThread = new std::thread(&CCommonWorkThread::Run, this);

	if(m_pThread != NULL)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CCommonWorkThread::Stop()
{
	m_bRun = FALSE;

	m_pThread->join();

	delete m_pThread;

	return TRUE;
}


BOOL CCommonWorkThread::AddMessage(NetPacket* pNetPacket)
{
	m_PacketQueue.push(pNetPacket);
	return TRUE;
}

BOOL CCommonWorkThread::SetThreadHandler( IThreadHandler* pCommandHandler )
{
	m_pThreadHandler = pCommandHandler;

	return TRUE;
}

BOOL CCommonWorkThread::OnThreadBegin()
{
	if(m_pThreadHandler == NULL)
	{
		return FALSE;
	}

	m_pThreadHandler->OnThreadBegin();

	return TRUE;
}

BOOL CCommonWorkThread::OnThreadEnd()
{
	if(m_pThreadHandler == NULL)
	{
		return FALSE;
	}

	m_pThreadHandler->OnThreadEnd();

	return TRUE;
}

