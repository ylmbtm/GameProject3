#include "stdafx.h"
#include "WatcherClient.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"

#ifdef WIN32
static BOOL        ExitRoutine(INT32 nSignal)
{
    switch (nSignal)
    {
        case CTRL_C_EVENT:
        {
            CWatcherClient::GetInstancePtr()->StopServer();
        }
        break;
        case CTRL_CLOSE_EVENT:
        {
            CWatcherClient::GetInstancePtr()->StopServer();
        }
        break;
        case CTRL_LOGOFF_EVENT:
        {
            CWatcherClient::GetInstancePtr()->StopServer();
        }
        break;
        case CTRL_SHUTDOWN_EVENT:
        {
            CWatcherClient::GetInstancePtr()->StopServer();
        }
        break;
        default:
            break;
    }

    return TRUE;
}
#else

static VOID        ExitRoutine(INT32 nSignal)
{
    switch (nSignal)
    {
        case SIGINT:
        {
            CWatcherClient::GetInstancePtr()->StopServer();
        }
        break;
        case SIGQUIT:
        {
            CWatcherClient::GetInstancePtr()->StopServer();
        }
        break;
        case SIGKILL:
        {
            CWatcherClient::GetInstancePtr()->StopServer();
        }
        break;
        case SIGTERM:
        {
            CWatcherClient::GetInstancePtr()->StopServer();
        }
        break;
        default:
            break;
    }
}
#endif;

CWatcherClient::CWatcherClient(void)
{
    m_bRun              = TRUE;
}

CWatcherClient::~CWatcherClient(void)
{
}

CWatcherClient* CWatcherClient::GetInstancePtr()
{
    static CWatcherClient _WatcherClient;

    return &_WatcherClient;
}

BOOL CWatcherClient::IsRun()
{
    return m_bRun;
}

BOOL CWatcherClient::RegExitSignal()
{
#ifdef _WIN32
    return SetConsoleCtrlHandler((PHANDLER_ROUTINE)ExitRoutine, TRUE);
#else
    signal(SIGINT, ExitRoutine);
    signal(SIGQUIT, ExitRoutine);
    signal(SIGKILL, ExitRoutine);
    signal(SIGTERM, ExitRoutine);
    return TRUE;
#endif
}

BOOL CWatcherClient::StopServer()
{
    m_bRun = FALSE;

    return TRUE;
}

BOOL CWatcherClient::OnNewConnect(INT32 nConnID)
{

    return TRUE;
}

BOOL CWatcherClient::OnCloseConnect(INT32 nConnID)
{
    return TRUE;
}

BOOL CWatcherClient::OnSecondTimer()
{
    return TRUE;
}

BOOL CWatcherClient::DispatchPacket(NetPacket* pNetPacket)
{

    return FALSE;
}



