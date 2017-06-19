// ProxyServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameService.h"

#pragma comment(lib, "ServerEngine.lib")

int main(int argc, char* argv[])
{
	CGameService::GetInstancePtr()->Init();

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	return 0;
}

