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
}

BOOL CCommonWorkThread::Start()
{
	m_bRun = TRUE;

	m_hThread = CommonThreadFunc::CreateThread(_CommonWorkThread, this);

	if(m_hThread != NULL)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CCommonWorkThread::Stop()
{
	m_bRun = FALSE;

	CommonThreadFunc::WaitThreadExit(m_hThread);

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


Th_RetName _CommonWorkThread( void* pParam )
{
	CCommonWorkThread* pThread = (CCommonWorkThread*)pParam;

	if(!pThread->OnThreadBegin())
	{
		ASSERT_FAIELD;

		return Th_RetValue;
	}

	pThread->Run();

	pThread->OnThreadEnd();

	return Th_RetValue;
}
