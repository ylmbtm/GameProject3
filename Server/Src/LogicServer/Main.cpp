// LogicServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameService.h"
#include "CrashReport.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerEngine_d.lib")
#else
#pragma comment(lib, "ServerEngine.lib")
#endif

int main(int argc, char* argv[])
{
	SetCrashReport("LogicServer");

	CGameService::GetInstancePtr()->Init();

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	return 0;
}

