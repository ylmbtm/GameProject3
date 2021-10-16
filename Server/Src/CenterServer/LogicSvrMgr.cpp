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

BOOL LogicSvrMgr::RegisterLogicServer(INT32 nConnID, UINT32 dwServerID, std::string strSvrName)
{
    LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
    if(pNode == NULL)
    {
        LogicServerNode* pTempNode = new LogicServerNode();
        pTempNode->m_dwServerID = dwServerID;
        pTempNode->m_nConnID   = nConnID;
        pTempNode->m_strSvrName = strSvrName;
        m_mapServer.insert(std::make_pair(dwServerID, pTempNode));
        return TRUE;
    }

    pNode->m_nConnID = nConnID;
    pNode->m_dwServerID = dwServerID;
    pNode->m_strSvrName = strSvrName;

    return TRUE;
}

BOOL LogicSvrMgr::UnregisterLogicServer(UINT32 dwServerID)
{
    LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
    if(pNode == NULL)
    {
        return TRUE;
    }

    pNode->m_nConnID = 0;

    return TRUE;
}

BOOL LogicSvrMgr::OnCloseConnect(INT32 nConnID)
{
    for (auto itor = m_mapServer.begin(); itor != m_mapServer.end(); ++itor)
    {
        LogicServerNode* pNode = itor->second;
        ERROR_CONTINUE_EX(pNode != NULL);

        if (pNode->m_nConnID == nConnID)
        {
            pNode->m_nConnID = 0;
        }

    }

    return TRUE;
}

UINT32 LogicSvrMgr::GetLogicConnID(UINT32 dwServerID)
{
    LogicServerNode* pNode = GetLogicServerInfo(dwServerID);
    if(pNode == NULL)
    {
        return 0;
    }

    return pNode->m_nConnID;
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

BOOL LogicSvrMgr::SendMsgProtoBuf(UINT32 dwServerID, INT32 nMsgID, const google::protobuf::Message& pdata)
{
    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(GetLogicConnID(dwServerID), nMsgID, 0, 0, pdata);
}

BOOL LogicSvrMgr::BroadMsgToAll(INT32 nMsgID, const google::protobuf::Message& pdata, INT32 nNoConnID)
{
    for (auto itor = m_mapServer.begin(); itor != m_mapServer.end(); ++itor)
    {
        LogicServerNode* pServerNode = itor->second;
        ERROR_CONTINUE_EX(pServerNode != NULL);

        if (pServerNode->m_nConnID == nNoConnID)
        {
            continue;
        }

        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pServerNode->m_nConnID, nMsgID, 0, 0, pdata);
    }

    return TRUE;
}

BOOL LogicSvrMgr::SendMsgRawData(UINT32 dwServerID, INT32 nMsgID, const char* pdata, UINT32 dwLen)
{
    return ServiceBase::GetInstancePtr()->SendMsgRawData(GetLogicConnID(dwServerID), nMsgID, 0, 0, pdata, dwLen);
}