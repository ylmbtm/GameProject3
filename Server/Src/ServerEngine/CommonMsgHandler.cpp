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

BOOL CCommonMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	ASSERT_FAIELD;
	return TRUE;
}

BOOL CCommonMsgHandler::AddMessage(NetPacket* pNetPacket)
{
	return m_WorkThread.AddMessage(pNetPacket);
}

BOOL CCommonMsgHandler::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}

