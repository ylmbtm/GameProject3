// PressureTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ClientObject.h"

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

	std::vector<CClientObject*> m_ClientList;

	char szBuff[256];

    const char* szTreeName = "SmartAI";

    behaviac::Workspace::EFileFormat ff = behaviac::Workspace::EFF_xml;
#if !BEHAVIAC_CCDEFINE_MSVC
    behaviac::Config::SetHotReload(false);
#endif
    behaviac::Workspace::GetInstance()->SetFilePath("../SmartClient/exported");
    behaviac::Workspace::GetInstance()->SetFileFormat(behaviac::Workspace::EFF_xml);

	for(int i = 0; i < nRobotNum; i++)
	{
		//CClientObject* pClientSpaceObject = new CClientObject();
        CClientObject* pClientObject = behaviac::Agent::Create<CClientObject>();
		sprintf(szBuff, "test%d", i);
		pClientObject->m_strAccountName = szBuff;
		sprintf(szBuff, "zhang%d", i);
		pClientObject->m_strRoleName = szBuff;
        pClientObject->m_strPassword = "123456";
		m_ClientList.push_back(pClientObject);

        pClientObject->btload(szTreeName);
        pClientObject->btsetcurrent(szTreeName);
	}

    while (TRUE)
    {
        for(std::vector<CClientObject*>::iterator itor = m_ClientList.begin(); itor != m_ClientList.end(); itor++)
        {
            CClientObject* pClient = *itor;

            behaviac::EBTStatus status = pClient->btexec();
            if(status != behaviac::BT_RUNNING)
            {
                goto EXIT;
            }
        }

        Sleep(1);
    }

    EXIT:
    getchar();
    getchar();

	return 0;
}

