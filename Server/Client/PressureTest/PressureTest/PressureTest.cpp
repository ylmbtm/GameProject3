// PressureTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ClientObject.h"
#include "..\Src\ServerEngine\CommonThreadFunc.h"

#define RUN_TIME 50

std::vector<CClientObject*> g_vtClientList;

//BOOL g_Run = FALSE;
//
//Th_RetName _NetEventThread( void* pParam )
//{
//	std::vector<CClientObject*>* pClientList = (std::vector<CClientObject*>*)pParam;
//
//	while(g_Run)
//	{
//		for(int i = 0; i < pClientList->size(); i++)
//		{
//			CClientObject* pObject = pClientList->at(i);
//			pObject->OnUpdate(0);
//		}
//
//		Sleep(10);
//	}
//
//	CommonThreadFunc::ExitThread();
//
//	return Th_RetValue;
//}
//
//
//for(int i = 0; i < 4; ++i)
//{
//	CommonThreadFunc::CreateThread(_NetEventThread, (void*)this);
//	m_vtWorkThread.push_back(hThread);
//}

int _tmain(int argc, _TCHAR* argv[])
{
	int nRobotNum = 0;
	printf("请输入需要启动的机器人数目: ");
	scanf_s("%d", &nRobotNum);

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

		g_vtClientList.push_back(pClientSpaceObject);
	}

DrivenRobot:
	DWORD dwTickLast = GetTickCount();
	for(std::vector<CClientObject*>::iterator itor = g_vtClientList.begin(); itor != g_vtClientList.end(); itor++)
	{
		CClientObject* pClient = *itor;

		pClient->OnUpdate(0);
	}

	Sleep(1);

	goto DrivenRobot;

	return 0;
}

