// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameService.h"

#pragma comment(lib, "ServerEngine.lib")

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("not enought param!!!");
		getchar();
		return 0;
	}

	UINT32 dwSvrID = atoi(argv[1]);
	UINT32 dwPort = atoi(argv[2]);

	if((dwSvrID <= 0)||(dwPort <= 0))
	{
		printf("svrid or port is not right!!!");
		getchar();
		return 0;
	}

	CGameService::GetInstancePtr()->Init(dwSvrID, dwPort);

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	return 0;
}

