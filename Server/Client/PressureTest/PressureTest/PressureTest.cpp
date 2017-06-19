// PressureTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CommandHandler.h"

#define ROBOT_NUM 2
#define RUN_TIME 50


int _tmain(int argc, _TCHAR* argv[])
{
	int nRobotNum = 0;
	printf("请输入需要启动的机器人数目: ");
	scanf_s("%d", &nRobotNum);

	if(nRobotNum <= 0)
	{
		return 0;
	}

	std::vector<CClientCmdHandler*> m_ClientList;

	char szBuff[256];

	for(int i = 0; i < nRobotNum; i++)
	{
		CClientCmdHandler *pClientSpaceObject = new CClientCmdHandler();

		sprintf(szBuff, "test%d", i);
		pClientSpaceObject->m_strAccountName = szBuff;
		pClientSpaceObject->m_strPassword = "123456";

		sprintf(szBuff, "zhang%d", i);
		pClientSpaceObject->m_strRoleName = szBuff;

		m_ClientList.push_back(pClientSpaceObject);
	}

DrivenRobot:
	DWORD dwTickLast = GetTickCount();
	for(std::vector<CClientCmdHandler*>::iterator itor = m_ClientList.begin(); itor != m_ClientList.end(); itor++)
	{
		CClientCmdHandler *pClient = *itor;

		pClient->OnUpdate(0);
	}
	
	DWORD dwTickCur = GetTickCount();
	if((dwTickCur-dwTickLast) < RUN_TIME)
	{
		Sleep(RUN_TIME - dwTickCur + dwTickLast);
	}

	goto DrivenRobot;

	return 0;
}

