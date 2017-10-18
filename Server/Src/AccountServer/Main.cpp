// AccountServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameService.h"
#include "CrashReport.h"

int main(int argc, char* argv[])
{
	SetCrashReport("AccountServer");

	CGameService::GetInstancePtr()->Init();

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	return 0;
}

