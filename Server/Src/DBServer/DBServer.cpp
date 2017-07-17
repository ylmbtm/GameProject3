// DBServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameService.h"
#include "CrashReport.h"

#pragma comment(lib, "ServerEngine.lib")

int main(int argc, char* argv[])
{
	SetCrashReport("DBServer");

	CGameService::GetInstancePtr()->Init();

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	return 0;
}

