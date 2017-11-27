#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "CommonFunc.h"
#include "CommonThreadFunc.h"
#include "Log.h"


CGameService::CGameService(void)
{

}

CGameService::~CGameService(void)
{

}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
}

BOOL CGameService::Init()
{
	CommonFunc::SetCurrentWorkDir("");

	if(!CLog::GetInstancePtr()->StartLog("WatchServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogError("---------服务器开始启动--------");
	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	m_WatchMsgHandler.Init(0);

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

	AutoRun();

	return TRUE;
}


BOOL CGameService::OnNewConnect(CConnection* pConn)
{
	m_WatchMsgHandler.OnNewConnect(pConn);

	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection* pConn)
{
	m_WatchMsgHandler.OnCloseConnect(pConn);

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
		default:
		{
			m_WatchMsgHandler.DispatchPacket(pNetPacket);
		}
		break;
	}

	return TRUE;
}

BOOL CGameService::AutoRun()
{
#ifdef WIN32
	HKEY hKey;
	LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	ERROR_RETURN_FALSE(lRet == ERROR_SUCCESS);
	char pFileName[MAX_PATH] = { 0 };
	DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
	lRet = RegSetValueEx(hKey, "SetAutoRun", 0, REG_SZ, (BYTE*)pFileName, dwRet);
	ERROR_RETURN_FALSE(lRet == ERROR_SUCCESS);
	RegCloseKey(hKey);
#else

#endif
	return TRUE;
}

BOOL CGameService::Uninit()
{
	ServiceBase::GetInstancePtr()->StopNetwork();
	google::protobuf::ShutdownProtobufLibrary();
	return TRUE;
}

BOOL CGameService::Run()
{
	UINT64 uTickCount = 0;
	while(TRUE)
	{
		ServiceBase::GetInstancePtr()->Update();

		uTickCount = CommonFunc::GetTickCount();

		m_WatchMsgHandler.OnUpdate(uTickCount);

		CommonThreadFunc::Sleep(1);
	}

	return TRUE;
}

void CGameService::KillWatchProcess(void)
{
	m_WatchMsgHandler.KillWatchProcess();
}
