#include "stdafx.h"
#include "LogMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "CommonSocket.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "../LogData/LogStruct.h"


CLogMsgHandler::CLogMsgHandler()
{

}

CLogMsgHandler::~CLogMsgHandler()
{

}

BOOL CLogMsgHandler::Init(INT32 nReserved)
{
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_log_svr_ip");
    INT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_log_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_log_svr_user");
    std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_log_svr_pwd");
    std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_log_svr_db_name");
    BOOL bRet = m_DBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort);
    if(!bRet)
    {
        CLog::GetInstancePtr()->LogError("CLogMsgHandler::Init Error: Can not open mysql database! Reason:%s", m_DBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError("CLogMsgHandler::Init Error: Host:[%s]-User:[%s]-Pwd:[%s]-DBName:[%s]", strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str());
        return FALSE;
    }

    m_nLastWriteTime = 0;

    return TRUE;
}

BOOL CLogMsgHandler::Uninit()
{
    m_DBConnection.close();

    return TRUE;
}

BOOL CLogMsgHandler::OnUpdate(UINT64 uTick)
{
    if (m_nLastWriteTime == 0)
    {
        m_nLastWriteTime = uTick;
    }

    if (m_nWriteCount == 0)
    {
        return TRUE;
    }

    if (uTick - m_nLastWriteTime > 1000)
    {
        if (!m_DBConnection.commit())
        {
            CLog::GetInstancePtr()->LogError("CLogMsgHandler::commit Error :%s", m_DBConnection.GetErrorMsg());
        }

        m_DBConnection.ping();

        if (!m_DBConnection.startTransaction())
        {
            CLog::GetInstancePtr()->LogError("CLogMsgHandler::commit Error :%s", m_DBConnection.GetErrorMsg());
        }

        m_nLastWriteTime = uTick;
        m_nWriteCount = 0;
    }

    return TRUE;
}

BOOL CLogMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
    switch(pNetPacket->m_nMsgID)
    {
            PROCESS_MESSAGE_ITEM(MSG_LOG_DATA_NTF,          OnMsgLogDataNtf)
    }

    return FALSE;
}

BOOL CLogMsgHandler::OnMsgLogDataNtf(NetPacket* pNetPacket)
{
    Log_BaseData* pData = (Log_BaseData*)pNetPacket->m_pDataBuffer->GetData();
    CHAR szSql[4096] = {0};

    switch (pData->m_LogType)
    {
        case ELT_ACCOUNT_CREATE:
        {
            Log_AccountCreate* p = (Log_AccountCreate*)pData;
            p->GetLogSql(szSql);
        }
        break;
        case ELT_ACCOUNT_LOGIN:
        {
            Log_AccountLogin* p = (Log_AccountLogin*)pData;
            p->GetLogSql(szSql);
        }
        break;
        case ELT_ROLE_CREATE:
        {
            Log_RoleCreate* p = (Log_RoleCreate*)pData;
            p->GetLogSql(szSql);
        }
        break;
        case ELT_ROLE_LOGIN:
        {
            Log_RoleLogin* p = (Log_RoleLogin*)pData;
            p->GetLogSql(szSql);
        }
        break;
        case ELT_ROLE_LOGOUT:
        {
            Log_RoleLogout* p = (Log_RoleLogout*)pData;
            p->GetLogSql(szSql);
        }
        break;
        case ELT_ROLE_CHAT:
        {
            Log_RoleChat* p = (Log_RoleChat*)pData;
            p->GetLogSql(szSql);
        }
        break;


        default:
            break;
    }

    if (m_DBConnection.execSQL(szSql) <= 0)
    {
        CLog::GetInstancePtr()->LogError("CLogMsgHandler::OnLogDataNtf m_LogType :%d, Error :%s", pData->m_LogType,  m_DBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError(szSql);
    }

    m_nWriteCount += 1;

    if (m_nWriteCount > 1000)
    {
        if (!m_DBConnection.commit())
        {
            CLog::GetInstancePtr()->LogError("CLogMsgHandler::commit Error :%s", m_DBConnection.GetErrorMsg());
        }

        m_DBConnection.ping();

        if (!m_DBConnection.startTransaction())
        {
            CLog::GetInstancePtr()->LogError("CLogMsgHandler::commit Error :%s", m_DBConnection.GetErrorMsg());
        }

        m_nWriteCount = 0;
    }

    return TRUE;
}

