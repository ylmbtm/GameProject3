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
	CLoginClient* pLoginClient = GetByConnID(dwConnID);
        return TRUE;
	//MSG_CHECK_VERSION_REQ是第一个消息，用于创建登录状态
	if (dwMsgID == MSG_CHECK_VERSION_REQ)
	{
		if (pLoginClient == NULL)
		{
			pLoginClient = CreateLoginClient(dwConnID);
		}
	}

	//如果是初始状态，则只能接收MSG_CHECK_VERSION_REQ版本验证消息，否则非法
	if (pLoginClient->m_ClientStatue == ECS_NONE)
	{
		return dwMsgID == MSG_CHECK_VERSION_REQ;
	}

	//如果版本己验证，则下一个消息必须是账号注册或登录，否则非法
	if(pLoginClient->m_ClientStatue == ECS_VER_CHECKED)
	{
		return dwMsgID == MSG_ACCOUNT_LOGIN_REQ || dwMsgID == MSG_ACCOUNT_REG_REQ;
	}

	//如果账号密码己验证，则下一个消息必须是选服消息，否则非法
	if (pLoginClient->m_ClientStatue == ECS_PSD_CHECKED)
	{
		return dwMsgID == MSG_SERVER_LIST_REQ || dwMsgID == MSG_SERVER_LIST_REQ;
	}

	return TRUE;
}

BOOL CLoginClientMgr::SetClientStatue(UINT32 dwConnID, EClientStatue eStatue)
{
	CLoginClient* pLoginClient = GetByConnID(dwConnID);
	ERROR_RETURN_FALSE(pLoginClient != NULL);

	pLoginClient->m_ClientStatue = eStatue;

	return TRUE;
}

BOOL CLoginClientMgr::OnCloseConnect(UINT32 dwConnID)
{
	RemoveByConnID(dwConnID);

	return TRUE;
}

