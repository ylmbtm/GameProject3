// PressureTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ClientObject.h"
#include "..\Src\ServerEngine\CommonFunc.h"
#include "..\Src\ServerEngine\CommonSocket.h"

#define RUN_TIME 50

std::vector<CClientObject*> g_vtClientList;

int _tmain(int argc, _TCHAR* argv[])
{
	int nRobotNum = 0;
	printf("请输入需要启动的机器人数目: ");
	scanf_s("%d", &nRobotNum);

	CommonSocket::InitNetwork();

	if(nRobotNum <= 0)
	{
		return 0;
	}

	char szBuff[256];
	for(int i = 0; i < nRobotNum; i++)
	{
		CClientObject* pClientSpaceObject = new CClientObject();

		sprintf(szBuff, "test%d", i);
		pClientSpaceObject->m_strAccountName = szBuff;
		pClientSpaceObject->m_strPassword = "123456";

		sprintf(szBuff, "zhang%d", i);
		pClientSpaceObject->m_strRoleName = szBuff;

		pClientSpaceObject->m_dwCarrerID = rand() % 4 + 1;

		g_vtClientList.push_back(pClientSpaceObject);
	}


	//十个以下机器人，不需要开线程　

	while (true)
	{
		DWORD dwTickLast = GetTickCount();
		for (std::vector<CClientObject*>::iterator itor = g_vtClientList.begin(); itor != g_vtClientList.end(); itor++)
		{
			CClientObject* pClient = *itor;

			pClient->OnUpdate(0);
		}

		CommonFunc::Sleep(RUN_TIME);
	}

	return 0;
}

