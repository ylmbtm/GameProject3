#include "stdafx.h"
#include "LogicSvrMgr.h"
#include "CommonSocket.h"


LogicSvrMgr::LogicSvrMgr(void)
{
}


LogicSvrMgr::~LogicSvrMgr(void)
{
}

LogicSvrMgr* LogicSvrMgr::GetInstancePtr()
{
	static LogicSvrMgr _StaticMgr;

	return &_StaticMgr;
}

BOOL LogicSvrMgr::Init()
{

	return TRUE;
}

BOOL LogicSvrMgr::RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID, std::string strSvrName)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		LogicServerNode* pTempNode = new LogicServerNode();
		pTempNode->m_dwServerID = dwServerID;
		pTempNode->m_dwConnID   = dwConnID;
		pTempNode->m_strSvrName = strSvrName;
		m_mapServer.insert(std::make_pair(dwServerID, pTempNode));
		return TRUE;
	}

	pNode->m_dwConnID = dwConnID;
	pNode->m_dwServerID = dwServerID;
	pNode->m_strSvrName = strSvrName;

	return TRUE;
}

BOOL LogicSvrMgr::UnregisterLogicServer(UINT32 dwConnID, UINT32 dwServerID)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		return TRUE;
	}

	pNode->m_dwConnID = 0;

	return TRUE;
}

UINT32 LogicSvrMgr::GetLogicConnID(UINT32 dwServerID)
{
	LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
	if(pNode == NULL)
	{
		return 0;
	}

	return pNode->m_dwConnID;
}

LogicServerNode* LogicSvrMgr::GetLogicServerInfo(UINT32 dwServerID)
{
	auto itor = m_mapServer.find(dwServerID);
	if(itor != m_mapServer.end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL LogicSvrMgr::SendMsgProtoBuf(UINT32 dwServerID, UINT32 dwMsgID, const google::protobuf::Message& pdata)
{
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(GetLogicConnID(dwServerID), dwMsgID, 0, 0, pdata);
}

BOOL LogicSvrMgr::SendMsgRawData(UINT32 dwServerID, UINT32 dwMsgID, const char* pdata, UINT32 dwLen)
{
	return ServiceBase::GetInstancePtr()->SendMsgRawData(GetLogicConnID(dwServerID), dwMsgID, 0, 0, pdata, dwLen);
}