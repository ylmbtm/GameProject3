#include "stdafx.h"
#include "GameService.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "WatcherClient.h"

CGameService::CGameService(void)
{
    m_nLogSvrConnID    = 0;
}

CGameService::~CGameService(void)
{
}

CGameService* CGameService::GetInstancePtr()
{
    static CGameService _GameService;

    return &_GameService;
}

INT32 CGameService::GetLogSvrConnID()
{
    return m_nLogSvrConnID;
}

BOOL CGameService::Init()
{
    CommonFunc::SetCurrentWorkDir("");

    if(!CLog::GetInstancePtr()->Start("AccountServer", "log"))
    {
        return FALSE;
    }

    CLog::GetInstancePtr()->LogInfo("---------服务器开始启动-----------");

    if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
    {
        CLog::GetInstancePtr()->LogError("配制文件加载失败!");
        return FALSE;
    }

    if (CommonFunc::IsAlreadyRun("AccountServer"))
    {
        CLog::GetInstancePtr()->LogError("AccountServer己经在运行!");
        return FALSE;
    }

    CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("account_log_level"));

    UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("account_svr_port");
    if (nPort <= 0)
    {
        CLog::GetInstancePtr()->LogError("配制文件account_svr_port配制错误!");
        return FALSE;
    }

    INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("account_svr_max_con");
    if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this, "127.0.0.1"))
    {
        CLog::GetInstancePtr()->LogError("启动服务失败!");
        return FALSE;
    }

    ERROR_RETURN_FALSE(m_AccountMsgHandler.Init(0));

    CLog::GetInstancePtr()->LogHiInfo("---------服务器启动成功!--------");

    return TRUE;
}

BOOL CGameService::OnNewConnect(INT32 nConnID)
{
    CWatcherClient::GetInstancePtr()->OnNewConnect(nConnID);

    return TRUE;
}

BOOL CGameService::OnCloseConnect(INT32 nConnID)
{
    if (nConnID == m_nLogSvrConnID)
    {
        m_nLogSvrConnID = 0;
    }

    CWatcherClient::GetInstancePtr()->OnCloseConnect(nConnID);

    return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
    ConnectToLogServer();

    return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
    if (CWatcherClient::GetInstancePtr()->DispatchPacket(pNetPacket))
    {
        return TRUE;
    }

    if (m_AccountMsgHandler.DispatchPacket(pNetPacket))
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CGameService::Uninit()
{
    CLog::GetInstancePtr()->LogError("==========服务器开始关闭=======================");

    ServiceBase::GetInstancePtr()->StopNetwork();

    m_AccountMsgHandler.Uninit();

    google::protobuf::ShutdownProtobufLibrary();

    CLog::GetInstancePtr()->LogError("==========服务器关闭完成=======================");

    return TRUE;
}

BOOL CGameService::Run()
{
    while (CWatcherClient::GetInstancePtr()->IsRun())
    {
        ServiceBase::GetInstancePtr()->Update();

        ServiceBase::GetInstancePtr()->FixFrameNum(100);
    }

    return TRUE;
}

BOOL CGameService::ConnectToLogServer()
{
    if (m_nLogSvrConnID != 0)
    {
        return TRUE;
    }
    INT32 nLogPort = CConfigFile::GetInstancePtr()->GetRealNetPort("log_svr_port");
    ERROR_RETURN_FALSE(nLogPort > 0);
    std::string strStatIp = CConfigFile::GetInstancePtr()->GetStringValue("log_svr_ip");
    CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strStatIp, nLogPort);
    ERROR_RETURN_FALSE(pConnection != NULL);
    m_nLogSvrConnID = pConnection->GetConnectionID();
    return TRUE;
}
