// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameService.h"
#include "CrashReport.h"

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Error : Not Enough Parameter!!! num:%d", argc);
		getchar();
		return 0;
	}

	UINT32 dwSvrID = atoi(argv[1]);
	UINT32 dwPort = atoi(argv[2]);

	if((dwSvrID <= 0) || (dwPort <= 0))
	{
		printf("Error Invalid ServerID:%d or Invalid Port:%d!!!", dwSvrID, dwPort);
		getchar();
		return 0;
	}

	SetCrashReport("GameServer");

	CGameService::GetInstancePtr()->Init(dwSvrID, dwPort);

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	return 0;
}

