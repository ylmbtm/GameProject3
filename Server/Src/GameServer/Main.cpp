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

	UINT32 dwSvrID = cmdLine.GetIntValue("id");
	UINT32 dwPort = cmdLine.GetIntValue("port");

	if((dwSvrID == 0) || (dwPort == 0))
	{
		printf("Error Invalid ServerID:%d or Invalid Port:%d!!!", dwSvrID, dwPort);
		getchar();
		return 0;
	}

	SetCrashReport("GameServer");

	//设置被监视索引，用于和监控程通信
	CWatcherClient::GetInstancePtr()->SetWatchIndex(cmdLine.GetIntValue("windex"));

	if (!CGameService::GetInstancePtr()->Init(dwSvrID, dwPort))
	{
		return 0;
	}

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	UnSetCrashReport();

	return 0;
}

