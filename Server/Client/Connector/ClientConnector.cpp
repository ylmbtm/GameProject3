#include "stdafx.h"
#include "ClientConnector.h"
#include "google\protobuf\message.h"
#include "..\Src\ServerEngine\CommonSocket.h"
#include "..\Src\ServerEngine\PacketHeader.h"
#include "..\Src\ServerEngine\CommonFunc.h"

CClientConnector::CClientConnector(void)
{
	m_hSocket		= INVALID_SOCKET;

	m_nDataLen = 0;
	m_PacketLen = 0;
	memset(m_PackBuffer, 0, CONST_BUFF_SIZE);
	memset(m_DataBuffer, 0, CONST_BUFF_SIZE);

	m_ConnectState	= ECS_NO_CONNECT;
}

CClientConnector::~CClientConnector(void)
{
	m_ConnectState	= ECS_NO_CONNECT;

	m_hSocket		= INVALID_SOCKET;

	m_nDataLen = 0;
	m_PacketLen = 0;
	memset(m_PackBuffer, 0, CONST_BUFF_SIZE);
	memset(m_DataBuffer, 0, CONST_BUFF_SIZE);
}

BOOL CClientConnector::SendData(UINT32 dwMsgID, const google::protobuf::Message& pdata, UINT64 u64TargetID, UINT32 dwUserData)
{
	char szBuff[10240] = {0};

	PacketHeader* pHeader = (PacketHeader*)szBuff;

	pHeader->CheckCode = 0x88;
	pHeader->dwMsgID = dwMsgID;
	pHeader->u64TargetID = u64TargetID;
	pHeader->dwUserData = dwUserData;
	pHeader->dwSize = 28 + pdata.ByteSize();

	pdata.SerializePartialToArray(szBuff + 28, pdata.ByteSize());

	return SendData( szBuff, pdata.ByteSize() + 28);
}

BOOL CClientConnector::SendData( char* pData, INT32 dwLen )
{
	if((pData == NULL) || (dwLen == 0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(m_ConnectState != ECS_CONNECTED)
	{
		return FALSE;
	}

	int nWriteLen = send(m_hSocket,  (char*)pData, dwLen, 0);
	if(nWriteLen < 0)
	{
		DWORD nError = CommonSocket::GetSocketLastError();

		printf("发送数据发生错误:%s!\n", CommonFunc::GetLastErrorStr(nError).c_str());

		return FALSE;
	}
	else
	{
		if(nWriteLen < dwLen)
		{
			SetConnectState(ECS_NO_CONNECT);
			CommonSocket::CloseSocket(m_hSocket);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CClientConnector::RegisterMsgHandler(IMessageHandler* pMsgHandler)
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

BOOL CClientConnector::UnregisterMsgHandler( IMessageHandler* pMsgHandler )
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
	if(m_ConnectState == ECS_NO_CONNECT)
	{
		return FALSE;
	}

	if(m_ConnectState == ECS_CONNECTING)
	{
		return FALSE;
	}

	ReceiveData();

	while(ProcessData());

	return TRUE;

}

BOOL CClientConnector::DispatchPacket(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	//switch(dwMsgID)
	//{
	//	default:
	//	{
	for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
	{
		IMessageHandler* pHandler = *itor;

		if(pHandler->DispatchPacket(dwMsgID, PacketBuf, BufLen))
		{
			break;
		}
	}
	//	}
	//	break;
	//}

	return TRUE;
}

BOOL CClientConnector::ConnectTo( std::string strIpAddr, UINT16 sPort )
{
	if(m_ConnectState != ECS_NO_CONNECT)
	{
		printf("连接服务器失败， 服务器己连接!\n");

		return FALSE;
	}

	SetConnectState(ECS_CONNECTING);

	m_hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if((m_hSocket == INVALID_SOCKET) || (m_hSocket == NULL))
	{
		printf("创建套接字失败!\n");

		SetConnectState(ECS_NO_CONNECT);

		return FALSE;
	}


	if(!CommonSocket::ConnectSocket(m_hSocket, strIpAddr.c_str(), sPort))
	{
		printf("连接服务器失败!\n");

		SetConnectState(ECS_NO_CONNECT);

		CommonSocket::CloseSocket(m_hSocket);

		return FALSE;
	}

	CommonSocket::SetSocketBlock(m_hSocket, FALSE);

	CommonSocket::SetSocketBuffSize(m_hSocket, 1024 * 1024, 1024 * 1024);

	SetConnectState(ECS_CONNECTED);

	return TRUE;
}

BOOL CClientConnector::DisConnect()
{
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

	m_ConnectState = ECS_NO_CONNECT;

	m_nDataLen = 0;

	m_PacketLen = 0;

	memset(m_PackBuffer, 0, CONST_BUFF_SIZE);

	memset(m_DataBuffer, 0, CONST_BUFF_SIZE);

	return TRUE;
}


void CClientConnector::SetConnectState( ConnectState val )
{
	if(val == ECS_CONNECTED)
	{
		printf("连接服务器成功!\n");
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
		if(nError != WSAEWOULDBLOCK)
		{
			printf("接收数据发生错误:%s!\n", CommonFunc::GetLastErrorStr(nError).c_str());

			SetConnectState(ECS_NO_CONNECT);

			CommonSocket::CloseSocket(m_hSocket);
		}

		return FALSE;
	}
	else if(nReadLen == 0)
	{
		printf("对方关闭了连接!\n");

		SetConnectState(ECS_NO_CONNECT);

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

	PacketHeader* pHeader = (PacketHeader*)m_DataBuffer;
	if(pHeader->CheckCode != 0x88)
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

	memcpy(m_PackBuffer, m_DataBuffer, pHeader->dwSize);
	m_PacketLen = pHeader->dwSize;
	m_nDataLen -= pHeader->dwSize;

	if(m_nDataLen > 0)
	{
		memmove(m_DataBuffer, m_DataBuffer + pHeader->dwSize, m_nDataLen);
	}
	else if(m_nDataLen < 0)
	{
		ASSERT_FAIELD;
	}

	PacketHeader* pMsgHeader = (PacketHeader*)m_PackBuffer;

	DispatchPacket(pMsgHeader->dwMsgID, m_PackBuffer + 28, m_PacketLen - 28);

	return TRUE;
}

UINT32 CClientConnector::GetServerTime()
{
	UINT32 dwTick = ::GetTickCount();

	return m_dwServerTime + (m_dwServerTick - dwTick) / 1000;
}

BOOL CClientConnector::IsConnected()
{
	return m_ConnectState == ECS_CONNECTED;
}








