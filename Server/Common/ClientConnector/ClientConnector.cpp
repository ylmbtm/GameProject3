#include "StdAfx.h"
#include "ClientConnector.h"
#include "CommandDef.h"
#include "DataBuffer\BufferHelper.h"
#include "PacketDef\ClientPacket.h"
#include "Error.h"
#include "ObjectID.h"
#include "Utility\CommonSocket.h"

CClientConnector::CClientConnector(void)
{
	m_hSocket		= INVALID_SOCKET;

	m_nDataLen		= 0;

	m_ConnectState	= Not_Connect;

	m_u64ClientID = 0;

	CommonSocket::InitNetwork();
}

CClientConnector::~CClientConnector(void)
{
	m_u64ClientID = 0;

	m_ConnectState	= Not_Connect;

	m_hSocket		= INVALID_SOCKET;

	m_nDataLen		= 0;

	CommonSocket::UninitNetwork();
}


BOOL CClientConnector::InitConnector()
{
	

	return TRUE;
}

BOOL CClientConnector::CloseConnector()
{

	return TRUE;
}

BOOL CClientConnector::SetClientID( UINT64 u64ClientID )
{
	m_u64ClientID = u64ClientID;

	return TRUE;
}

template <typename T>
BOOL CClientConnector::SendData(UINT16 dwMsgID, T &msgData, UINT32 dwSceneID, UINT64 u64CharID)
{
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(dwMsgID, dwSceneID, u64CharID);
	WriteHelper.Write(msgData);
	WriteHelper.EndWrite();

	SendData(m_WriteBuffer.GetBuffer(), m_WriteBuffer.GetTotalLenth());
	return TRUE;
}

BOOL CClientConnector::SendData( char *pData, INT32 dwLen )
{
	if((pData == NULL)||(dwLen == 0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if((m_ConnectState != Succ_Connect)&&(m_ConnectState != Raw_Connect))
	{
		return FALSE;
	}

	int nWriteLen = send(m_hSocket,  (char*)pData, dwLen, 0);
	if(nWriteLen < 0)
	{
		DWORD nError = CommonSocket::GetSocketLastError();

		printf("发送数据发生错误:%s!\n", CommonSocket::GetLastErrorStr(nError).c_str());

		return FALSE;
	}
	else
	{
		if(nWriteLen < dwLen)
		{
			SetConnectState(Not_Connect);
			CommonSocket::CloseSocket(m_hSocket);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CClientConnector::Login(const char *pszAccountName, const char *pszPassword, BOOL bConnect)
{
	if((pszPassword == NULL)||(pszAccountName == NULL))
	{
		return FALSE;
	}

	if(bConnect)
	{
		if(m_strLoginIp.empty())
		{
			ASSERT_FAIELD;
			return FALSE;
		}

		DisConnect();

		if(!ConnectToServer(m_strLoginIp, m_sLoginPort))
		{
			ASSERT_FAIELD;
			return FALSE;
		}
	}

	StCharLoginReq CharLoginReq;
	strncpy(CharLoginReq.szAccountName, pszAccountName, 32);
	strncpy(CharLoginReq.szPassword, pszPassword, 32);
	SendData(CMD_CHAR_LOGIN_REQ, CharLoginReq, 0, 0);

	return TRUE;
}

BOOL CClientConnector::RegisterMsgHandler(IMessageHandler *pMsgHandler)
{
	for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
	{
		if(pMsgHandler == *itor)
		{
			return FALSE;
		}
	}
		
	m_vtMsgHandler.push_back(pMsgHandler);

	return TRUE;
}

BOOL CClientConnector::UnregisterMsgHandler( IMessageHandler *pMsgHandler )
{
	for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
	{
		if(pMsgHandler == *itor)
		{
			m_vtMsgHandler.erase(itor);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CClientConnector::Render()
{
	if(m_ConnectState == Not_Connect)
	{
		return FALSE;
	}

	if(m_ConnectState == Start_Connect)
	{
		return FALSE;
	}

	if(m_ConnectState == Raw_Connect)
	{

	}

	ReceiveData();
	
	while(ProcessData());

	return TRUE;

}

BOOL CClientConnector::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_PICK_CHAR_ACK,	OnCmdPickCharAck);

		PROCESS_COMMAND_ITEM_T(CMD_CHAR_HEART_BEAT_ACK,	OnCmdHearBeatAck);

	default:
		{
			for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
			{
				IMessageHandler *pHandler = *itor;

				if(pHandler->OnCommandHandle(wCommandID, u64ConnID, pBufferHelper))
				{
					break;
				}
			}
		}
		break;
	}

	return TRUE;
}


BOOL CClientConnector::OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
		StCharEnterGameReq CharEnterGameReq;

		CharEnterGameReq.u64CharID = m_u64ClientID;

		CharEnterGameReq.dwIdentifyCode = m_dwIdentifyCode;

		CHECK_PAYER_ID(m_u64ClientID);

		CBufferHelper WriteHelper(TRUE, 1024);

		WriteHelper.BeginWrite(CMD_CHAR_ENTER_GAME_REQ,0,  CharEnterGameReq.u64CharID);

		WriteHelper.Write(CharEnterGameReq);

		WriteHelper.EndWrite();

		SendData(m_WriteBuffer.GetBuffer(),m_WriteBuffer.GetTotalLenth());
	
	return 0;
}

IDataBuffer* CClientConnector::GetWriteBuffer()
{
	return &m_WriteBuffer;
}

BOOL CClientConnector::OnCmdPickCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharPickCharAck CharPickCharAck;
	pBufferHelper->Read(CharPickCharAck);

	CHECK_PAYER_ID(CharPickCharAck.u64CharID);

	if(CharPickCharAck.nRetCode == E_SUCCESSED)
	{
		DisConnect();
		m_u64ClientID = CharPickCharAck.u64CharID;
		m_dwIdentifyCode = CharPickCharAck.dwIdentifyCode;
		ConnectToServer(CharPickCharAck.szProxyIpAddr, CharPickCharAck.nProxyPort);
	}

	return TRUE;
}

BOOL CClientConnector::ConnectToServer( std::string strIpAddr, UINT16 sPort )
{
	if(m_ConnectState != Not_Connect)
	{
		printf("连接服务器失败， 服务器己连接!\n");

		return FALSE;
	}

	SetConnectState(Start_Connect);

	m_hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if((m_hSocket == INVALID_SOCKET)||(m_hSocket == NULL))
	{
		printf("创建套接字失败!\n");

		SetConnectState(Not_Connect);

		return FALSE;
	}


	if(!CommonSocket::ConnectSocket(m_hSocket, strIpAddr.c_str(), sPort))
	{
		printf("连接服务器失败!\n");

		SetConnectState(Not_Connect);

		CommonSocket::CloseSocket(m_hSocket);

		return FALSE;
	}

	CommonSocket::SetSocketUnblock(m_hSocket);

	SetConnectState(Raw_Connect);

	return TRUE;
}

BOOL CClientConnector::DisConnect()
{
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

	m_ConnectState = Not_Connect;

	return TRUE;
}


void CClientConnector::SetConnectState( ConnectState val )
{
	if(val == Not_Connect)
	{
		printf("设置未连接!\n");
	}
	else if(val == Start_Connect)
	{
		printf("设置开始连接!\n");
	}
	else if(val == Raw_Connect)
	{
		printf("设置己完成原始连接!\n");
	}
	else if(val == Succ_Connect)
	{
		printf("设置连接成功!\n");
	}

	m_ConnectState = val;
}

ConnectState CClientConnector::GetConnectState( VOID )
{
	return m_ConnectState;
}


BOOL CClientConnector::ReceiveData()
{
	int nReadLen = recv(m_hSocket, m_DataBuffer + m_nDataLen, CONST_BUFF_SIZE - m_nDataLen, 0);
	if(nReadLen < 0)
	{
		DWORD nError = CommonSocket::GetSocketLastError();
		if(nError == WSAEWOULDBLOCK)
		{

		}
		else 
		{
			printf("接收数据发生错误:%s!\n", CommonSocket::GetLastErrorStr(nError).c_str());
		}

		return FALSE;
	}
	else if(nReadLen == 0)
	{
		printf("对方关闭了连接!\n");

		SetConnectState(Not_Connect); 

		CommonSocket::CloseSocket(m_hSocket);

		return FALSE;
	}
	else
	{
		m_nDataLen += nReadLen;
	}

	return TRUE;
}


BOOL CClientConnector::ProcessData()
{
	if(m_nDataLen < sizeof(PacketHeader))
	{
		return FALSE;
	}

	PacketHeader *pHeader = (PacketHeader *)m_DataBuffer;
	if(pHeader->CheckCode != 0xff)
	{
		ASSERT_FAIELD;
		return FALSE;
	}
	
	if(pHeader->dwSize == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(pHeader->dwSize > m_nDataLen)
	{
		return FALSE;
	}

	UINT32 dwCheckCode = *(UINT32*)(m_DataBuffer+pHeader->dwSize-4);
	if(dwCheckCode != 0x11111111)
	{
		ASSERT_FAIELD;
	}

	memcpy(m_ReadBuffer.GetBuffer(), m_DataBuffer, pHeader->dwSize);

	m_ReadBuffer.SetTotalLenth(pHeader->dwSize);

	m_nDataLen -= pHeader->dwSize;

	if(m_nDataLen > 0)
	{
		memmove(m_DataBuffer, m_DataBuffer+pHeader->dwSize, m_nDataLen);
	}
	else if(m_nDataLen == 0)
	{
		printf("直接处理完了一缓冲数据!\n");
	}
	else
	{
		ASSERT_FAIELD;
	}

	CBufferHelper BufferReader(FALSE, &m_ReadBuffer);
	if(!BufferReader.BeginRead())
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if((pHeader->wCommandID > CMD_BEGIN_TAG)&&(pHeader->wCommandID < CMD_END_TAG))
	{
		OnCommandHandle(pHeader->wCommandID, 0, &BufferReader);
	}
	else
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	

	
	
	return TRUE;
}

UINT32 CClientConnector::GetServerTime()
{
	UINT32 dwTick = ::GetTickCount();

	return m_dwServerTime + (m_dwServerTick - dwTick)/1000;
}

BOOL CClientConnector::OnCmdHearBeatAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharHeartBeatAck CharHeartBeatAck;
	pBufferHelper->Read(CharHeartBeatAck);

	UINT32 dwTick = ::GetTickCount();

	m_dwServerTime = CharHeartBeatAck.dwServerTime + (dwTick - CharHeartBeatAck.dwReqTimestamp) /1000;
	m_dwServerTick = dwTick;

	return TRUE;
}

BOOL CClientConnector::SetLoginServerAddr( std::string strIpAddr, UINT16 sPort )
{
	m_strLoginIp = strIpAddr;
	m_sLoginPort = sPort;

	return TRUE;
}








