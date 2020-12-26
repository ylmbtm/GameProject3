#include "stdafx.h"
#include "GameService.h"

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

	if(!CLog::GetInstancePtr()->Start("WatchServer", "log"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->LogInfo("---------服务器开始启动--------");
	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	if (CommonFunc::IsAlreadyRun("WatchServer" + CConfigFile::GetInstancePtr()->GetStringValue("areaid")))
	{
		CLog::GetInstancePtr()->LogError("WatchServer己经在运行!");
		return FALSE;
	}

	CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("watch_log_level"));

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetRealNetPort("watch_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	m_WatchMsgHandler.Init(0);

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

	return TRUE;
}


BOOL CGameService::OnNewConnect(UINT32 nConnID)
{
	m_WatchMsgHandler.OnNewConnect(nConnID);

	return TRUE;
}

BOOL CGameService::OnCloseConnect(UINT32 nConnID)
{
	m_WatchMsgHandler.OnCloseConnect(nConnID);

	return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
	m_WatchMsgHandler.OnSecondTimer();

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
	if (m_WatchMsgHandler.DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return FALSE;
}

// BOOL CGameService::AutoRun()
// {
// #ifdef WIN32
// 	HKEY hKey;
// 	LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
// 	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
// 	ERROR_RETURN_FALSE(lRet == ERROR_SUCCESS);
// 	char pFileName[MAX_PATH] = { 0 };
// 	DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
// 	lRet = RegSetValueEx(hKey, "SetAutoRun", 0, REG_SZ, (BYTE*)pFileName, dwRet);
// 	ERROR_RETURN_FALSE(lRet == ERROR_SUCCESS);
// 	RegCloseKey(hKey);
// #else
//
// #endif
// 	return TRUE;
// }

BOOL CGameService::Uninit()
{
	CLog::GetInstancePtr()->LogError("==========服务器开始关闭=======================");

	ServiceBase::GetInstancePtr()->StopNetwork();

	m_WatchMsgHandler.Uninit();

	google::protobuf::ShutdownProtobufLibrary();

	CLog::GetInstancePtr()->LogError("==========服务器关闭完成=======================");

	return TRUE;
}

BOOL CGameService::Run()
{
	while (TRUE)
	{
		ServiceBase::GetInstancePtr()->Update();

		CommonFunc::Sleep(1);
	}

	return TRUE;
}






