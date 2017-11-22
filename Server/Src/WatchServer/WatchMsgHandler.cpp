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


CWatchMsgHandler::CWatchMsgHandler()
{
	m_uCheckProcessTime = 0;
}

CWatchMsgHandler::~CWatchMsgHandler()
{

}

BOOL CWatchMsgHandler::Init(UINT32 dwReserved)
{
	ReadProcessList();
	if (CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_statue"))
	{
		BootUpProcessList();
	}

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
			PROCESS_MESSAGE_ITEM(MSG_WATCH_START_SVR_REQ, OnMsgStartServerReq)
			PROCESS_MESSAGE_ITEM(MSG_WATCH_STOP_SVR_REQ, OnMsgStopServerReq)
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_ACK, OnMsgServerHeartAck)
			PROCESS_MESSAGE_ITEM(MSG_WATCH_SVR_PHP_REQ, OnMsgWatchWebReq)
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
			serverInfo.ProscessStatus = connSucceed;
			serverInfo.RecvTime = CommonFunc::GetTickCount();
		}
	}

	return TRUE;
}

BOOL CWatchMsgHandler::OnCloseConnect(CConnection* pConn)
{
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
	BootUpProcessList();

	return TRUE;

}


BOOL CWatchMsgHandler::OnMsgStopServerReq(NetPacket* pNetPacket)
{
	for (INT32 i = 0; i < m_vtProcessVec.size(); i++)
	{
		ServerProcessInfo& serverData = m_vtProcessVec[i];
		CommonFunc::KillProcess(serverData.ProcessID);
		serverData.ProscessStatus = waitStart;
		serverData.RecvTime = CommonFunc::GetTickCount();
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
	CLog::GetInstancePtr()->LogError("耗时 %d ", serverData.RecvTime - serverData.SendTime);
	return TRUE;
}


BOOL CWatchMsgHandler::OnMsgWatchWebReq(NetPacket* pNetPacket)
{
	CHAR szMsgBuf[1024] = { 0 };
	strncpy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());


	return TRUE;
}

BOOL CWatchMsgHandler::BootUpProcessList()
{
	for (std::vector<ServerProcessInfo>::iterator iter = m_vtProcessVec.begin(); iter != m_vtProcessVec.end(); iter++)
	{
		ServerProcessInfo& processData = *iter;
		processData.ProcessID = 0;
		processData.ProscessStatus = start;
		processData.ConnID = 0;
		processData.SendTime = 0;
		processData.RecvTime = 0;

		if (system(processData.BootUpParameter.c_str()) != 0)
		{
			CLog::GetInstancePtr()->LogError("----启动失败 :%s 失败！！！！----", processData.BootUpParameter.c_str());
		}
	}

	return true;
}

BOOL CWatchMsgHandler::CheckProcessStatus(INT64 uTick)
{
	if (uTick < m_uCheckProcessTime)
	{
		return true;
	}
	else
	{
		m_uCheckProcessTime = uTick + 500;
	}
	PrintServerStatus();
	for (INT32 index = 0; index < m_vtProcessVec.size(); index++)
	{
		ServerProcessInfo& serverInfo = m_vtProcessVec[index];
		if (serverInfo.ProscessStatus == start)
		{
			CConnection* pConn = ServiceBase::GetInstancePtr()->ConnectToOtherSvr("127.0.0.1", serverInfo.Port);
			ERROR_RETURN_FALSE(pConn != NULL);
			INT32 connid = pConn->GetConnectionID();
			serverInfo.ConnID = connid;

			if (serverInfo.RecvTime == 0)
			{
				serverInfo.RecvTime = uTick + 6000;
			}

			if (uTick - serverInfo.RecvTime > 30000)
			{
				KillProcess(serverInfo);
			}

		}

		if (serverInfo.ProscessStatus == connSucceed)
		{
			if (uTick - serverInfo.RecvTime > 2500)
			{
				KillProcess(serverInfo);
			}
			else
			{
				WatchHeartBeatReq Req;
				Req.set_data(index);
				ServiceBase::GetInstancePtr()->SendMsgProtoBuf(serverInfo.ConnID, MSG_WATCH_HEART_BEAT_REQ, 0, 0, Req);
				serverInfo.SendTime = uTick;
			}
		}

		if (serverInfo.ProscessStatus == waitStart && uTick - serverInfo.RecvTime > 1000)
		{
			serverInfo.ProscessStatus = start;
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
			serverData.ProscessStatus = waitStart;
			serverData.RecvTime = CommonFunc::GetTickCount();
		}
	}
	else
	{
		CommonFunc::KillProcess(processData.ProcessID);
		processData.ProscessStatus = waitStart;
		processData.RecvTime = CommonFunc::GetTickCount();

	}
	return true;
}


BOOL CWatchMsgHandler::ReadProcessList()
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
		m_vtProcessVec.push_back(ServerData);
	}

	return TRUE;
}

BOOL CWatchMsgHandler::GetProcessBootUpPar(std::string& bootUpPar, std::string& servername, std::string& pars)
{
	if (servername == "")
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
			case connSucceed:
				connCount++;
				break;
			case waitStart:
				waitStartCount++;
				break;
		}
	}

	CLog::GetInstancePtr()->SetTitle(" 进程总数：%d  连接成功数： %d  等待启动数： %d", m_vtProcessVec.size(), connCount, waitStartCount);
}
