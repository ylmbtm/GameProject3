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
		return FALSE;
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
			PROCESS_MESSAGE_ITEMEX(MSG_WATCH_HEART_BEAT_REQ,	OnMsgServerHeartReq)
			PROCESS_MESSAGE_ITEM(MSG_PHP_GM_COMMAND_REQ,		OnMsgWebCommandReq)
	}

	return FALSE;
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
	if (!m_bStartWatch)
	{
		INT32 nCount = 0;
		for (INT32 nIndex = 0; nIndex < m_vtProcess.size(); nIndex++)
		{
			ServerProcessInfo& serverInfo = m_vtProcess[nIndex];
			if (serverInfo.ProscessStatus == EPS_Connected)
			{
				nCount++;
			}
		}

		CLog::GetInstancePtr()->LogInfo("----监视:%s， 总进程数:%d, 活跃进程数:%d---", m_bStartWatch ? "[开]" : "[关]", m_vtProcess.size(), nCount);

		return TRUE;
	}

	for (INT32 nIndex = 0; nIndex < m_vtProcess.size(); nIndex++)
	{
		CheckProcessStatus(nIndex);
	}

	INT32 nCount = 0;
	for (INT32 nIndex = 0; nIndex < m_vtProcess.size(); nIndex++)
	{
		ServerProcessInfo& serverInfo = m_vtProcess[nIndex];
		if (serverInfo.ProscessStatus == EPS_Connected)
		{
			nCount++;
		}
	}

	CLog::GetInstancePtr()->LogInfo("----监视:%s， 总进程数:%d, 活跃进程数:%d---", m_bStartWatch ? "[开]" : "[关]", m_vtProcess.size(), nCount);

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

	if (serverData.ProscessStatus == EPS_Starting || serverData.ProscessStatus == EPS_Checking)
	{
		serverData.ProscessStatus = EPS_Connected;
	}

	serverData.ConnID = pNetPacket->m_dwConnID;

	WatchHeartBeatAck Ack;
	Ack.set_data(index);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_WATCH_HEART_BEAT_ACK, 0, 0, Ack);
	return TRUE;
}


BOOL CWatchMsgHandler::OnMsgWebCommandReq(NetPacket* pNetPacket)
{
	CHAR szMsgBuf[1024] = { 0 };
	CommonConvert::StrCopy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth() + 1);

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

			KillProcessByMsg(serverData);
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

BOOL CWatchMsgHandler::CheckProcessStatus(UINT32 nIndex)
{
	UINT64 uCurrentTick = CommonFunc::GetTickCount();

	ServerProcessInfo& serverInfo = m_vtProcess[nIndex];

	switch (serverInfo.ProscessStatus)
	{
		case EPS_Stop:
		{
			StartProcess(serverInfo, nIndex);
		}
		break;
		case EPS_Starting:
		{
			//如果响应超时，表示服务器己经停止响应或不存在
			if (((uCurrentTick > serverInfo.LastHeartTick)) && (uCurrentTick - serverInfo.LastHeartTick > 3000))
			{
				//先杀死
				KillProcess(serverInfo);
			}
		}
		break;
		case EPS_Connected:
		{
			//如果响应超时，表示服务器己经停止响应或不存在
			if ((((uCurrentTick > serverInfo.LastHeartTick)) && (uCurrentTick - serverInfo.LastHeartTick > 6000)) || (!CommonFunc::IsProcessExist(serverInfo.ProcessID)))
			{
				//先杀死
				KillProcess(serverInfo);
			}
		}
		break;
		case EPS_Checking:
		{
			if (serverInfo.LastHeartTick == 0)
			{
				serverInfo.LastHeartTick = uCurrentTick;
			}

			//如果3秒还没有连上我，就是表示不存在
			if (uCurrentTick - serverInfo.LastHeartTick > 2000)
			{
				serverInfo.ProscessStatus = EPS_Stop;
				serverInfo.LastHeartTick = uCurrentTick;
			}
		}
		break;
		case EPS_Stoping:
		{
			if ((uCurrentTick - serverInfo.LastHeartTick > 2000) || (!CommonFunc::IsProcessExist(serverInfo.ProcessID)))
			{
				serverInfo.ProscessStatus = EPS_Stop;
				serverInfo.LastHeartTick = uCurrentTick;
			}
		}
		break;
		default:
			break;
	}

	return TRUE;
}

BOOL CWatchMsgHandler::StartProcess(ServerProcessInfo& processData, INT32 nIndex)
{
	processData.ProscessStatus = EPS_Starting;

	processData.LastHeartTick = CommonFunc::GetTickCount();

	std::string strRunString = processData.Params;

	strRunString += " windex=" + CommonConvert::IntToString(nIndex + 1);

	if (!CommonFunc::CreateProcess(processData.serverName.c_str(), strRunString))
	{
		CLog::GetInstancePtr()->LogError("----重新启动失败 :%s 失败！！！！----", processData.BootUpParameter.c_str());
	}

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
				serverData.ProscessStatus = EPS_Stoping;
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
			processData.ProscessStatus = EPS_Stoping;
			processData.LastHeartTick = CommonFunc::GetTickCount();
			processData.ProcessID = 0;
			processData.ConnID = 0;
		}
	}
	return TRUE;
}

BOOL CWatchMsgHandler::KillProcessByMsg(ServerProcessInfo& processData)
{
	if (processData.ProscessStatus != EPS_Connected)
	{
		return FALSE;
	}

	GmStopServerReq Req;
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(processData.ConnID, MSG_GM_SHUTDOWN_SVR_REQ, 0, 0, Req);
	processData.ProscessStatus = EPS_Stoping;
	processData.LastHeartTick = CommonFunc::GetTickCount();
	processData.ProcessID = 0;
	processData.ConnID = 0;

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
		ServerData.ProscessStatus = EPS_Checking;
		ServerData.ConnID = 0;
		ServerData.LastHeartTick = 0;
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
