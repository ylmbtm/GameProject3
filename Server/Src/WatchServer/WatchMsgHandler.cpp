#include "stdafx.h"

#include "WatchMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "RapidXml.h"
#include "../Message/Msg_Game.pb.h"
#include "HttpParameter.h"
#include "WebActionDef.h"


CWatchMsgHandler::CWatchMsgHandler()
{
	m_uLaskTick = 0;

	m_bStartWatch = true;
}

CWatchMsgHandler::~CWatchMsgHandler()
{

}

BOOL CWatchMsgHandler::Init(UINT32 dwReserved)
{
	if (!LoadProcessList())
	{
		CLog::GetInstancePtr()->LogError("Cannot find the processlist.xml!");
	}

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
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_REQ,	OnMsgServerHeartReq)
			PROCESS_MESSAGE_ITEM(MSG_PHP_GM_COMMAND_REQ,	OnMsgWebCommandReq)
	}

	return FALSE;
}

BOOL CWatchMsgHandler::OnUpdate(UINT64 uTick)
{
	if ((m_uLaskTick != 0) && (uTick - m_uLaskTick < 1000))
	{
		return TRUE;
	}

	m_uLaskTick = uTick;

	if (!m_bStartWatch)
	{
		return TRUE;
	}

	for (INT32 nIndex = 0; nIndex < m_vtProcess.size(); nIndex++)
	{
		CheckProcessStatus(uTick, nIndex);
	}

	return TRUE;
}


BOOL CWatchMsgHandler::OnNewConnect(UINT32 nConnID)
{
	return TRUE;
}

BOOL CWatchMsgHandler::OnCloseConnect(UINT32 nConnID)
{
	return TRUE;
}

BOOL CWatchMsgHandler::OnSecondTimer()
{
	return TRUE;
}

BOOL CWatchMsgHandler::KillAllProcess()
{
	for (INT32 i = 0; i < m_vtProcess.size(); i++)
	{
		ServerProcessInfo& serverData = m_vtProcess[i];
		if (CommonFunc::KillProcess(serverData.ProcessID))
		{
			serverData.ProscessStatus = EPS_Stop;
			serverData.LastHeartTick = CommonFunc::GetTickCount();
			serverData.ProcessID = 0;
			serverData.ConnID = 0;
		}

	}
	return TRUE;
}

BOOL CWatchMsgHandler::OnMsgServerHeartReq(NetPacket* pNetPacket)
{
	WatchHeartBeatReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	INT32 index = Req.data();
	ERROR_RETURN_FALSE(index > 0);
	ERROR_RETURN_FALSE(index <= m_vtProcess.size());

	ServerProcessInfo& serverData = m_vtProcess[index - 1];
	serverData.ProcessID = Req.processid();
	serverData.LastHeartTick = CommonFunc::GetTickCount();
	if (serverData.ProscessStatus == EPS_Starting)
	{
		serverData.ProscessStatus = EPS_Connected;
	}

	serverData.ConnID = pNetPacket->m_dwConnID;

	INT32 nCount = 0;
	for (int i = 0; i < m_vtProcess.size(); i++)
	{
		if (m_vtProcess[i].ProcessID != 0)
		{
			nCount += 1;
		}
	}

	CLog::GetInstancePtr()->LogInfo("--收到心跳， 总进程数:%d, 当前有效心跳数:%d， 监视:%s", m_vtProcess.size(), nCount, m_bStartWatch ? "[开]" : "[关]");

	WatchHeartBeatAck Ack;
	Ack.set_data(index);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_WATCH_HEART_BEAT_REQ, 0, 0, Ack);
	return TRUE;
}


BOOL CWatchMsgHandler::OnMsgWebCommandReq(NetPacket* pNetPacket)
{
	CHAR szMsgBuf[1024] = { 0 };
	strncpy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

	HttpParameter Params;
	Params.ParseStringToMap(szMsgBuf);
	std::string strAction = Params.GetStrValue("Action");
	CLog::GetInstancePtr()->LogInfo("Web Action :%s", strAction.c_str());

	EWebAction eWebAction = (EWebAction)CommonConvert::StringToInt(strAction.c_str());
	switch (eWebAction)
	{
		case EWA_SERVER_START:
			OnGmServerStart(Params, pNetPacket->m_dwConnID);
			break;
		case EWA_SERVER_STOP:
			OnGmServerStop(Params, pNetPacket->m_dwConnID);
			break;
		case EWA_SERVER_UPDATE:
			OnGmServerUpdate(Params, pNetPacket->m_dwConnID);
			break;
		case EWA_SERVER_INFO:
			OnGmServerInfo(Params, pNetPacket->m_dwConnID);
			break;
		default:
			SendWebResult(pNetPacket->m_dwConnID, EWR_INVALID_ACT);
			break;
	}

	return TRUE;
}

void CWatchMsgHandler::OnGmServerStart(HttpParameter& hParams, UINT32 nConnID)
{
	if (CanStartServer())
	{
		StartAllProcess();

		SetStartWatch(TRUE);
	}

	SendWebResult(nConnID, EWR_SUCCESSED);
}

void CWatchMsgHandler::OnGmServerStop(HttpParameter& hParams, UINT32 nConnID)
{
	if (CanStopServer())
	{
		SetStartWatch(FALSE);
		for (INT32 i = 0; i < m_vtProcess.size(); i++)
		{
			ServerProcessInfo& serverData = m_vtProcess[i];
			if (CommonFunc::KillProcess(serverData.ProcessID))
			{
				serverData.ProscessStatus = EPS_Stop;
				serverData.LastHeartTick = CommonFunc::GetTickCount();
				serverData.ProcessID = 0;
				serverData.ConnID = 0;
			}
		}
	}

	SendWebResult(nConnID, EWR_SUCCESSED);
}

void CWatchMsgHandler::OnGmServerUpdate(HttpParameter& hParams, UINT32 nConnID)
{
	system("start updatesvr.bat");

	SendWebResult(nConnID, EWR_SUCCESSED);
}

void CWatchMsgHandler::OnGmServerInfo(HttpParameter& hParams, UINT32 nConnID)
{
	SendWebResult(nConnID, EWR_SUCCESSED);
}

BOOL CWatchMsgHandler::StartAllProcess()
{
	for (INT32 i = 0; i < m_vtProcess.size(); i++)
	{
		StartProcess(m_vtProcess.at(i), i);
	}

	return TRUE;
}

BOOL CWatchMsgHandler::CheckProcessStatus(UINT64 uTick, UINT32 nIndex)
{
	ServerProcessInfo& serverInfo = m_vtProcess[nIndex];

	if (serverInfo.ProscessStatus == EPS_Stop)
	{
		StartProcess(serverInfo, nIndex);
	}

	if (serverInfo.ProscessStatus == EPS_Starting)
	{
		//如果响应超时，表示服务器己经停止响应或不存在
		if (((uTick > serverInfo.LastHeartTick)) && (uTick - serverInfo.LastHeartTick > 3000))
		{
			//先杀死
			KillProcess(serverInfo);
		}
	}

	if (serverInfo.ProscessStatus == EPS_Connected)
	{
		//如果响应超时，表示服务器己经停止响应或不存在
		if (((uTick > serverInfo.LastHeartTick)) && (uTick - serverInfo.LastHeartTick > 3000))
		{
			//先杀死
			KillProcess(serverInfo);
		}
	}


	return TRUE;
}

BOOL CWatchMsgHandler::StartProcess(ServerProcessInfo& processData, INT32 nIndex)
{
	std::string strRunString = processData.BootUpParameter;

	strRunString += " windex=" + CommonConvert::IntToString(nIndex + 1);

	if (system(strRunString.c_str()) != 0)
	{
		CLog::GetInstancePtr()->LogError("----重新启动失败 :%s 失败！！！！----", processData.BootUpParameter.c_str());
	}

	processData.ProscessStatus = EPS_Starting;

	processData.LastHeartTick = CommonFunc::GetTickCount();

	return TRUE;
}

BOOL CWatchMsgHandler::KillProcess(ServerProcessInfo& processData)
{
	if (processData.KillAll)
	{
		for (INT32 i = 0; i < m_vtProcess.size(); i++)
		{
			ServerProcessInfo& serverData = m_vtProcess[i];
			if (CommonFunc::KillProcess(serverData.ProcessID) || serverData.ProcessID == 0)
			{
				serverData.ProscessStatus = EPS_Stop;
				serverData.LastHeartTick = CommonFunc::GetTickCount();
				serverData.ProcessID = 0;
				serverData.ConnID = 0;
			}

		}
	}
	else
	{
		if (CommonFunc::KillProcess(processData.ProcessID) || processData.ProcessID == 0)
		{
			processData.ProscessStatus = EPS_Stop;
			processData.LastHeartTick = CommonFunc::GetTickCount();
			processData.ProcessID = 0;
			processData.ConnID = 0;
		}
	}
	return TRUE;
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
		ServerData.BootUpParameter = "start " + ServerData.serverName + " " + ServerData.Params;
		ServerData.KillAll = CommonConvert::StringToInt(pObjectNode->first_attribute("KillAll")->value());
		ServerData.ProcessID = 0;
		ServerData.ProscessStatus = EPS_Starting;
		ServerData.ConnID = 0;
		ServerData.LastHeartTick = CommonFunc::GetTickCount();
		m_vtProcess.push_back(ServerData);
	}

	return TRUE;
}


void CWatchMsgHandler::PrintServerStatus()
{
	INT32 connCount = 0;
	INT32 waitStartCount = 0;
	for (auto iter : m_vtProcess)
	{
		switch (iter.ProscessStatus)
		{
			case EPS_Connected:
				connCount++;
				break;
		}
	}

	CLog::GetInstancePtr()->SetTitle(" 进程总数：%d  连接成功数：%d  等待启动数：%d", m_vtProcess.size(), connCount, waitStartCount);
}

BOOL CWatchMsgHandler::GetStartWatch()
{
	return m_bStartWatch;
}

void CWatchMsgHandler::SetStartWatch(BOOL bStart)
{
	m_bStartWatch = bStart;
}

BOOL CWatchMsgHandler::CanStartServer()
{
	for (auto it : m_vtProcess)
	{
		if (it.ProscessStatus == EPS_Connected)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CWatchMsgHandler::CanStopServer()
{
	for (auto it : m_vtProcess)
	{
		if (it.ProscessStatus != EPS_Connected || it.ProcessID == 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CWatchMsgHandler::SendWebResult(UINT32 nConnID, EWebResult eResult)
{
	std::string strResult = CommonConvert::IntToString((INT64)eResult);

	ServiceBase::GetInstancePtr()->SendMsgRawData(nConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, strResult.c_str(), (UINT32)strResult.size());

	return TRUE;
}
