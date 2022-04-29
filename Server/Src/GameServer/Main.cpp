// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameService.h"
#include "CrashReport.h"
#include "CommandLine.h"
#include "WatcherClient.h"

int main(int argc, char* argv[])
{
    CCommandLine cmdLine(argc, argv);

    INT32 nSvrID = cmdLine.GetIntValue("id");
    INT32 nPort = cmdLine.GetIntValue("port");

    if((nSvrID == 0) || (nPort == 0))
    {
        printf("Error Invalid ServerID:%d or Invalid Port:%d!!!", nSvrID, nPort);
        getchar();
        return 0;
    }

    SetCrashReport("GameServer");

    if (!CGameService::GetInstancePtr()->Init(nSvrID, nPort))
    {
        return 0;
    }

    CGameService::GetInstancePtr()->Run();

    CGameService::GetInstancePtr()->Uninit();

    UnSetCrashReport();

    return 0;
}

