#include "stdafx.h"
#include "CommandDef.h"
#include "LogMsgHandler.h"
#include "CommonFunc.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "Log.h"

CLogMsgHandler::CLogMsgHandler()
{

}

CLogMsgHandler::~CLogMsgHandler()
{

}

BOOL CLogMsgHandler::Init(UINT32 dwReserved)
{
	std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_ip");
	UINT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
	std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_user");
	std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_pwd");
	std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_db_name");
	BOOL bRet = m_DBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort);
	ERROR_RETURN_FALSE(bRet);

	m_nLastWriteTime = 0;

	return TRUE;
}

BOOL CLogMsgHandler::Uninit()
{
	m_DBConnection.close();

	return TRUE;
}

BOOL CLogMsgHandler::OnUpdate(UINT64 uTick)
{
	if (m_nLastWriteTime == 0)
	{
		m_nLastWriteTime = uTick;
	}

	if (m_nWriteCount <= 0)
	{
		return TRUE;
	}

	if (uTick - m_nLastWriteTime > 1000)
	{
		m_DBConnection.commit();
		m_DBConnection.ping();
		m_DBConnection.startTransaction();
		m_nLastWriteTime = uTick;
		m_nWriteCount = 0;
	}

	return TRUE;
}

BOOL CLogMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_LOG_DATA_NTF, OnLogDataNtf)
		default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CLogMsgHandler::OnLogDataNtf(NetPacket* pNetPacket)
{
	char* pSql = pNetPacket->m_pDataBuffer->GetData();
	*(pSql + pNetPacket->m_pDataBuffer->GetBodyLenth()) = 0;

	if (m_DBConnection.execSQL(pSql) <= 0)
	{
		CLog::GetInstancePtr()->LogError(pSql);
		if (!m_DBConnection.ping())
		{
			m_DBConnection.close();
			m_DBConnection.reconnect();
		}
	}

	m_nWriteCount += 1;

	if (m_nWriteCount > 1000)
	{
		m_DBConnection.commit();
		m_DBConnection.ping();
		m_DBConnection.startTransaction();
		m_nWriteCount = 0;
	}

	return TRUE;
}

