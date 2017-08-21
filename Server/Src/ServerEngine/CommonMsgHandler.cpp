#include "stdafx.h"
#include "CommonMsgHandler.h"
#include "CommonFunc.h"
#include "CommonEvent.h"

CCommonMsgHandler::CCommonMsgHandler()
{
	m_WorkThread.SetCommandHandler(this);
}

CCommonMsgHandler::~CCommonMsgHandler()
{

}

BOOL CCommonMsgHandler::Init(UINT32 dwReserved)
{
	m_WorkThread.Start();

	return TRUE;
}

BOOL CCommonMsgHandler::Uninit()
{
	m_WorkThread.Stop();

	return TRUE;
}

BOOL CCommonMsgHandler::OnMessageHandle( UINT32 dwMsgID, UINT64 u64ConnID, IDataBuffer* pDataBuffer)
{
	ASSERT_FAIELD;
	return TRUE;
}

BOOL CCommonMsgHandler::AddMessage( UINT64 u64ConnID, IDataBuffer* pDataBuffer )
{
	return m_WorkThread.AddMessage(u64ConnID, pDataBuffer);
}

BOOL CCommonMsgHandler::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}

