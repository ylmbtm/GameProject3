#include "stdafx.h"
#include "GameService.h"

CGameService::CGameService(void)
{
    m_bIsRun = TRUE;
}

CGameService::~CGameService(void)
{
    m_bIsRun = FALSE;
}

CGameService* CGameService::GetInstancePtr()
{
    static CGameService _GameService;

    return &_GameService;
}

BOOL CGameService::Init()
{
    CommonFunc::SetCurrentWorkDir("");

    if(!CLog::GetInstancePtr()->Start("WatchServer", "log"))
    {
        return FALSE;
    }

    CLog::GetInstancePtr()->LogInfo("---------服务器开始启动--------");
    if(!CConfigFile::GetInstancePtr()->Load("watchcfg.ini"))
    {
        CLog::GetInstancePtr()->LogError("配制文件加载失败!");
        return FALSE;
    }

    if (CommonFunc::IsAlreadyRun("WatchServer"))
    {
        CLog::GetInstancePtr()->LogError("WatchServer己经在运行!");
        return FALSE;
    }

    CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("watch_log_level"));

    UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_port");
    INT32 nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_max_con");
    std::string strListenIp = CConfigFile::GetInstancePtr()->GetStringValue("watch_svr_ip");
    if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this, strListenIp))
    {
        CLog::GetInstancePtr()->LogError("启动服务失败!");
        return FALSE;
    }

    m_WatchMsgHandler.Init(0);

    CLog::GetInstancePtr()->LogHiInfo("---------服务器启动成功!--------");

    return TRUE;
}


BOOL CGameService::OnNewConnect(INT32 nConnID)
{
    m_WatchMsgHandler.OnNewConnect(nConnID);

    return TRUE;
}

BOOL CGameService::OnCloseConnect(INT32 nConnID)
{
    m_WatchMsgHandler.OnCloseConnect(nConnID);

    return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
    m_WatchMsgHandler.OnSecondTimer();

    return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
    if (m_WatchMsgHandler.DispatchPacket(pNetPacket))
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CGameService::Uninit()
{
    CLog::GetInstancePtr()->LogError("==========服务器开始关闭=======================");

    ServiceBase::GetInstancePtr()->StopNetwork();

    m_WatchMsgHandler.Uninit();

    google::protobuf::ShutdownProtobufLibrary();

    CLog::GetInstancePtr()->LogError("==========服务器关闭完成=======================");

    return TRUE;
}

BOOL CGameService::Run()
{
    while (m_bIsRun)
    {
        ServiceBase::GetInstancePtr()->Update();

        ServiceBase::GetInstancePtr()->FixFrameNum(30);
    }

    return TRUE;
}






