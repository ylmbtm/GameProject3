#include "stdafx.h"
#include "LoginClientMgr.h"
#include "../Message/Msg_ID.pb.h"

CLoginClient::CLoginClient()
{
	m_dwConnID = 0;
}

CLoginClient::~CLoginClient()
{

}

UINT32 CLoginClient::GetConnID()
{
	return m_dwConnID;
}

EClientStatue CLoginClient::GetClientStatue()
{
	return m_ClientStatue;
}

VOID CLoginClient::SetConnID(UINT32 dwConnID)
{
	m_dwConnID = dwConnID;
}

CLoginClientMgr::CLoginClientMgr(void)
{
}

CLoginClientMgr::~CLoginClientMgr(void)
{
}

CLoginClientMgr* CLoginClientMgr::GetInstancePtr()
{
	static CLoginClientMgr _StaicClientMgr;

	return &_StaicClientMgr;
}

CLoginClient* CLoginClientMgr::GetByConnID(UINT32 dwConnID)
{
	return GetByKey(dwConnID);
}

BOOL CLoginClientMgr::RemoveByConnID(UINT32 dwConnID)
{
	return Delete(dwConnID);
}

CLoginClient* CLoginClientMgr::CreateLoginClient(UINT32 dwConnID)
{
	CLoginClient* pLoginClient = InsertAlloc(dwConnID);
	ERROR_RETURN_NULL(pLoginClient != NULL);

	pLoginClient->m_dwConnID = dwConnID;
	pLoginClient->m_ClientStatue = ECS_NONE;

	return pLoginClient;
}

BOOL CLoginClientMgr::CheckClientMessage(UINT32 dwConnID, UINT32 dwMsgID)
{
	return TRUE;
	CLoginClient* pLoginClient = GetByConnID(dwConnID);

	if (pLoginClient == NULL)
	{
		if (dwMsgID != MSG_CHECK_VERSION_REQ)
		{
			return FALSE;
		}

		pLoginClient = CreateLoginClient(dwConnID);

		return TRUE;
	}

	//如果是初始状态，则只能接收MSG_CHECK_VERSION_REQ版本验证消息，否则非法
	if (pLoginClient->m_ClientStatue == ECS_NONE)
	{
		if (dwMsgID == MSG_CHECK_VERSION_REQ)
		{
			pLoginClient->m_ClientStatue = ECS_VER_CHECKED;
			return TRUE;
		}

		return FALSE;
	}

	//如果版本己验证，则下一个消息必须是账号注册或登录，否则非法
	if(pLoginClient->m_ClientStatue == ECS_VER_CHECKED)
	{
		if (dwMsgID == MSG_ACCOUNT_LOGIN_REQ || dwMsgID == MSG_ACCOUNT_REG_REQ)
		{
			pLoginClient->m_ClientStatue = ECS_PSD_CHECKED;
			return TRUE;
		}

		return FALSE;
	}

	//如果账号密码己验证，则下一个消息必须是选服消息，否则非法
	if (pLoginClient->m_ClientStatue == ECS_PSD_CHECKED)
	{
		if (dwMsgID == MSG_SERVER_LIST_REQ)
		{
			return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

BOOL CLoginClientMgr::OnCloseConnect(UINT32 dwConnID)
{
	RemoveByConnID(dwConnID);

	return TRUE;
}

