#include "stdafx.h"
#include "CommandDef.h"
#include "WatchMsgHandler.h"
#include "CommonFunc.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "Log.h"
#include "CommonConvert.h"
#include "RapidXml.h"
#include "../Message/Msg_Game.pb.h"
#include "HttpParameter.h"


CWatchMsgHandler::CWatchMsgHandler()
{
	m_uCheckProcessTime = 0;

	m_bStartWatch = true;
}

CWatchMsgHandler::~CWatchMsgHandler()
{

}

BOOL CWatchMsgHandler::Init(UINT32 dwReserved)
{
	LoadProcessList();

	SetStartWatch(CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_statue"));

	return TRUE;
}

BOOL CWatchMsgHandler::Uninit()
{
	return TRUE;
}

BOOL CWatchMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	switch (pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_WATCH_START_SVR_REQ,	OnMsgStartServerReq)
			PROCESS_MESSAGE_ITEM(MSG_WATCH_STOP_SVR_REQ,	OnMsgStopServerReq)
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_ACK,	OnMsgServerHeartAck)
			PROCESS_MESSAGE_ITEM(MSG_WATCH_SVR_PHP_REQ,		OnMsgWatchWebReq)
		default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CWatchMsgHandler::OnUpdate(UINT64 uTick)
{
	CheckProcessStatus(uTick);

	return TRUE;
}


BOOL CWatchMsgHandler::OnNewConnect(CConnection* pConn)
{
	ERROR_RETURN_FALSE(pConn != nullptr);
	INT32 connid = pConn->GetConnectionID();
	for (std::vector<ServerProcessInfo>::iterator iter = m_vtProcessVec.begin(); iter != m_vtProcessVec.end(); ++iter)
	{
		ServerProcessInfo& serverInfo = *iter;
		if (serverInfo.ConnID == connid)
		{
			serverInfo.ProscessStatus = EPS_ConnSucceed;
			serverInfo.RecvTime = CommonFunc::GetTickCount();
		}
	}

	return TRUE;
}

BOOL CWatchMsgHandler::OnCloseConnect(CConnection* pConn)
{
	return TRUE;
}

BOOL CWatchMsgHandler::KillWatchProcess()
{
	for (INT32 i = 0; i < m_vtProcessVec.size(); i++)
	{
		ServerProcessInfo& serverData = m_vtProcessVec[i];

		CommonFunc::KillProcess(serverData.ProcessID);
	}
	return TRUE;
}


BOOL CWatchMsgHandler::OnMsUpdateServerReq(NetPacket* pNetPacket)
{
	/*
	ReadConfigFile();
	MD5校验;
	更新失败

	更新文件
	检查

	//检查完成，更新完成
	*/
	return TRUE;
}

BOOL CWatchMsgHandler::OnMsgStartServerReq(NetPacket* pNetPacket)
{
	if (CanStartServer())
	{
		BootUpProcessList();

		SetStartWatch(TRUE);
	}
	else
	{
		CLog::GetInstancePtr()->LogError("----服务器已经连接 不能重复启动----");
	}

	return TRUE;
}


BOOL CWatchMsgHandler::OnMsgStopServerReq(NetPacket* pNetPacket)
{
	if (CancloseServer())
	{
		SetStartWatch(FALSE);
		for (INT32 i = 0; i < m_vtProcessVec.size(); i++)
		{
			ServerProcessInfo& serverData = m_vtProcessVec[i];
			CommonFunc::KillProcess(serverData.ProcessID);
			serverData.ProscessStatus = EPS_WaitStart;
			serverData.RecvTime = CommonFunc::GetTickCount();
			serverData.ProcessID = 0;
		}
	}
	else
	{
		CLog::GetInstancePtr()->LogError("----有服务等待连接 不能关闭----");
	}

	return TRUE;

}

BOOL CWatchMsgHandler::OnMsgServerHeartAck(NetPacket* pNetPacket)
{
	WatchHeartBeatAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	INT32 index = Ack.data();
	ERROR_RETURN_FALSE(index < m_vtProcessVec.size());
	ServerProcessInfo& serverData = m_vtProcessVec[index];
	serverData.ProcessID = Ack.processid();
	serverData.RecvTime = CommonFunc::GetTickCount();
	return TRUE;
}


BOOL CWatchMsgHandler::OnMsgWatchWebReq(NetPacket* pNetPacket)
{
	CHAR szMsgBuf[1024] = { 0 };
	strncpy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	HttpParameter Params(szMsgBuf);
	std::string strAction = Params.GetStrValue("Action");
	if (strAction == "StartServer")
	{
		if (CanStartServer())
		{
			BootUpProcessList();

			SetStartWatch(TRUE);
		}
	}
	else if (strAction == "StopServer")
	{
		if (CancloseServer())
		{
			SetStartWatch(FALSE);
			for (INT32 i = 0; i < m_vtProcessVec.size(); i++)
			{
				ServerProcessInfo& serverData = m_vtProcessVec[i];
				CommonFunc::KillProcess(serverData.ProcessID);
				serverData.ProscessStatus = EPS_WaitStart;
				serverData.RecvTime = CommonFunc::GetTickCount();
				serverData.ProcessID = 0;
			}
		}
	}
	else if (strAction == "UpdateServer")
	{

	}
	else if (strAction == "UpdateInfo")
	{
		char sz[20] = "123569";
		ServiceBase::GetInstancePtr()->SendMsgRawData(pNetPacket->m_dwConnID, 12345, 0, 0, sz, 20);
	}
	return TRUE;
}

BOOL CWatchMsgHandler::BootUpProcessList()
{
	for (std::vector<ServerProcessInfo>::iterator iter = m_vtProcessVec.begin(); iter != m_vtProcessVec.end(); iter++)
	{
		ServerProcessInfo& processData = *iter;
		processData.ProcessID = 0;
		processData.ProscessStatus = EPS_Start;
		processData.ConnID = 0;
		processData.SendTime = 0;
		processData.RecvTime = 0;

		if (system(processData.BootUpParameter.c_str()) != 0)
		{
			CLog::GetInstancePtr()->LogError("----启动失败 :%s 失败！！！！----", processData.BootUpParameter.c_str());
		}
	}

	return TRUE;
}

BOOL CWatchMsgHandler::CheckProcessStatus(UINT64 uTick)
{
	if (uTick < m_uCheckProcessTime)
	{
		return TRUE;
	}

	m_uCheckProcessTime = uTick + 1000;

	PrintServerStatus();

	if (!m_bStartWatch)
	{
		return TRUE;
	}

	for (INT32 nIndex = 0; nIndex < m_vtProcessVec.size(); nIndex++)
	{
		ServerProcessInfo& serverInfo = m_vtProcessVec[nIndex];
		if (serverInfo.ProscessStatus == EPS_Start || serverInfo.ProscessStatus == EPS_Init)
		{
			CConnection* pConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr("127.0.0.1", serverInfo.Port);
			ERROR_RETURN_FALSE(pConn != NULL);
			serverInfo.ConnID = pConn->GetConnectionID();

			if (serverInfo.RecvTime == 0)
			{
				serverInfo.RecvTime = uTick;
			}

			if (serverInfo.ProscessStatus == EPS_Start)
			{
				if (uTick - serverInfo.RecvTime > 300000)
				{
					KillProcess(serverInfo);
				}
			}
			else
			{
				if (uTick - serverInfo.RecvTime > 1)
				{
					KillProcess(serverInfo);
				}
			}
		}

		if (serverInfo.ProscessStatus == EPS_ConnSucceed)
		{
			if (uTick - serverInfo.RecvTime > 2500)
			{
				KillProcess(serverInfo);
			}
			else
			{
				WatchHeartBeatReq Req;
				Req.set_data(nIndex);
				ServiceBase::GetInstancePtr()->SendMsgProtoBuf(serverInfo.ConnID, MSG_WATCH_HEART_BEAT_REQ, 0, 0, Req);
				serverInfo.SendTime = uTick;
			}
		}

		if (serverInfo.ProscessStatus == EPS_WaitStart && uTick - serverInfo.RecvTime > 1000)
		{
			serverInfo.ProscessStatus = EPS_Start;
			serverInfo.RecvTime = 0;
			serverInfo.SendTime = 0;

			if (system(serverInfo.BootUpParameter.c_str()) != 0)
			{
				CLog::GetInstancePtr()->LogError("----重新启动失败 :%s 失败！！！！----", serverInfo.BootUpParameter.c_str());
			}
		}
	}

	return true;
}

BOOL CWatchMsgHandler::KillProcess(ServerProcessInfo& processData)
{
	if (processData.KillAll)
	{
		for (INT32 i = 0; i < m_vtProcessVec.size(); i++)
		{
			ServerProcessInfo& serverData = m_vtProcessVec[i];
			CommonFunc::KillProcess(serverData.ProcessID);
			serverData.ProscessStatus = EPS_WaitStart;
			serverData.RecvTime = CommonFunc::GetTickCount();
			serverData.ProcessID = 0;
		}
	}
	else
	{
		CommonFunc::KillProcess(processData.ProcessID);
		processData.ProscessStatus = EPS_WaitStart;
		processData.RecvTime = CommonFunc::GetTickCount();
		processData.ProcessID = 0;
	}
	return true;
}


BOOL CWatchMsgHandler::LoadProcessList()
{
	std::string strPath = "ProcessList.xml";
	FILE* pFile = fopen(strPath.c_str(), "rb");
	ERROR_RETURN_FALSE(pFile != NULL);
	char buff[2048] = { 0 };
	fseek(pFile, 0, SEEK_END);
	INT32 nSize = ftell(pFile);
	rewind(pFile);
	fread(buff, nSize, 1, pFile);
	fclose(pFile);
	buff[nSize + 1] = 0;

	rapidxml::xml_document<char>* pXMLDoc = new rapidxml::xml_document<char>();

	pXMLDoc->parse<0>(buff);

	rapidxml::xml_node<char>* pNode = pXMLDoc->first_node("Root");

	for (auto pObjectNode = pNode->first_node("Process"); pObjectNode != NULL; pObjectNode = pObjectNode->next_sibling("Process"))
	{
		ServerProcessInfo ServerData;
		ServerData.serverName = std::string(pObjectNode->first_attribute("Name")->value());
		if (ServerData.serverName == "")
		{
			continue;
		}
		ServerData.Params = std::string(pObjectNode->first_attribute("Params")->value());
		GetProcessBootUpPar(ServerData.BootUpParameter, ServerData.serverName, ServerData.Params);
		ServerData.Port = CommonConvert::StringToInt(pObjectNode->first_attribute("Port")->value());
		ServerData.KillAll = CommonConvert::StringToInt(pObjectNode->first_attribute("KillAll")->value());
		ServerData.ProcessID = 0;
		ServerData.ProscessStatus = EPS_Init;
		ServerData.ConnID = 0;
		ServerData.SendTime = 0;
		ServerData.RecvTime = 0;
		m_vtProcessVec.push_back(ServerData);
	}

	return TRUE;
}

BOOL CWatchMsgHandler::GetProcessBootUpPar(std::string& bootUpPar, std::string& servername, std::string& pars)
{
	if (servername == "" || servername.empty())
	{
		CLog::GetInstancePtr()->LogError(" processList.xml Name  为空");
		return FALSE;
	}

	if (pars != "")
	{
		bootUpPar = "start " + servername + " " + pars;
	}
	else
	{
		bootUpPar = "start " + servername;
	}
	return TRUE;
}

void CWatchMsgHandler::PrintServerStatus()
{
	INT32 connCount = 0;
	INT32 waitStartCount = 0;
	for (auto iter : m_vtProcessVec)
	{
		switch (iter.ProscessStatus)
		{
			case EPS_ConnSucceed:
				connCount++;
				break;
			case EPS_WaitStart:
				waitStartCount++;
				break;
		}
	}

	CLog::GetInstancePtr()->SetTitle(" 进程总数：%d  连接成功数： %d  等待启动数： %d", m_vtProcessVec.size(), connCount, waitStartCount);
}

BOOL CWatchMsgHandler::GetStartWatch()
{
	return m_bStartWatch;
}

void CWatchMsgHandler::SetStartWatch(BOOL bStart)
{
	m_bStartWatch = bStart;
}

bool CWatchMsgHandler::CanStartServer()
{
	for (auto it : m_vtProcessVec)
	{
		if (it.ProscessStatus == EPS_ConnSucceed)
		{
			return false;
		}
	}
	return true;
}

bool CWatchMsgHandler::CancloseServer()
{
	for (auto it : m_vtProcessVec)
	{
		if (it.ProcessID == 0)
		{
			return false;
		}
	}
	return true;
}


