// AccountServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameService.h"
#include "CrashReport.h"
#include "CommandLine.h"

int main(int argc, char* argv[])
{
	SetCrashReport("AccountServer");

	CCommandLine cmdLine(argc, argv);

	//设置被监视索引，用于和监控程通信
	CGameService::GetInstancePtr()->SetWatchIndex(cmdLine.GetIntValue("windex"));

	if (!CGameService::GetInstancePtr()->Init())
	{
		return 0;
	}

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	UnSetCrashReport();

	return 0;
}

