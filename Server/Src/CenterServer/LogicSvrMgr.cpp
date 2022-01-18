#include "stdafx.h"
#include "LogicSvrMgr.h"
#include "CommonSocket.h"


CLogicSvrMgr::CLogicSvrMgr(void)
{
}


CLogicSvrMgr::~CLogicSvrMgr(void)
{
}

CLogicSvrMgr* CLogicSvrMgr::GetInstancePtr()
{
    static CLogicSvrMgr _StaticMgr;

    return &_StaticMgr;
}

BOOL CLogicSvrMgr::Init()
{
    return TRUE;
}

BOOL CLogicSvrMgr::Uninit()
{
    for (auto itor = m_mapServer.begin(); itor != m_mapServer.end(); itor++)
    {
        LogicServerNode* pTempNode = itor->second;

        delete pTempNode;
    }

    m_mapServer.clear();

    return TRUE;
}

BOOL CLogicSvrMgr::RegisterLogicServer(INT32 nConnID, INT32 nServerID, std::string strSvrName)
{
    LogicServerNode* pNode = GetLogicServerInfo(nServerID);
    if(pNode == NULL)
    {
        LogicServerNode* pTempNode = new LogicServerNode();
        pTempNode->m_nServerID = nServerID;
        pTempNode->m_nConnID   = nConnID;
        pTempNode->m_strSvrName = strSvrName;
        m_mapServer.insert(std::make_pair(nServerID, pTempNode));
        return TRUE;
    }

    pNode->m_nConnID = nConnID;
    pNode->m_nServerID = nServerID;
    pNode->m_strSvrName = strSvrName;

    return TRUE;
}

BOOL CLogicSvrMgr::UnregisterLogicServer(INT32 nServerID)
{
    LogicServerNode* pNode = GetLogicServerInfo(nServerID);
    if(pNode == NULL)
    {
        return TRUE;
    }

    pNode->m_nConnID = 0;

    return TRUE;
}

BOOL CLogicSvrMgr::OnCloseConnect(INT32 nConnID)
{
    for (auto itor = m_mapServer.begin(); itor != m_mapServer.end(); ++itor)
    {
        LogicServerNode* pNode = itor->second;
        ERROR_CONTINUE_EX(pNode != NULL);

        if (pNode->m_nConnID == nConnID)
        {
            pNode->m_nConnID = 0;

            CLog::GetInstancePtr()->LogError("Lost Connect To Server:[%d] !", pNode->m_nServerID);
        }

    }

    return TRUE;
}

INT32 CLogicSvrMgr::GetLogicConnID(INT32 nServerID)
{
    LogicServerNode* pNode = GetLogicServerInfo(nServerID);
    if(pNode == NULL)
    {
        return 0;
    }

    return pNode->m_nConnID;
}

LogicServerNode* CLogicSvrMgr::GetLogicServerInfo(INT32 nServerID)
{
    auto itor = m_mapServer.find(nServerID);
    if(itor != m_mapServer.end())
    {
        return itor->second;
    }

    return NULL;
}

BOOL CLogicSvrMgr::SendMsgProtoBuf(INT32 nServerID, INT32 nMsgID, const google::protobuf::Message& pdata, UINT64 uTargetID, UINT32 userData)
{
    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(GetLogicConnID(nServerID), nMsgID, uTargetID, userData, pdata);
}

BOOL CLogicSvrMgr::BroadMsgToAll(INT32 nMsgID, const google::protobuf::Message& pdata, INT32 nNoConnID)
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

BOOL CLogicSvrMgr::SendMsgRawData(INT32 nServerID, INT32 nMsgID, const char* pdata, INT32 nLen)
{
    return ServiceBase::GetInstancePtr()->SendMsgRawData(GetLogicConnID(nServerID), nMsgID, 0, 0, pdata, nLen);
}