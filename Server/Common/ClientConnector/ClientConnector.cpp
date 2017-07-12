#include "StdAfx.h"
#include "ClientConnector.h"
#include "CommandDef.h"
#include "Utility\CommonSocket.h"
#include "..\Src\Message\Msg_Login.pb.h"
#include "PacketHeader.h"

CClientConnector::CClientConnector(void)
{
	m_hSocket		= INVALID_SOCKET;

	m_nDataLen = 0;
	m_PacketLen = 0;
	memset(m_PackBuffer, 0, CONST_BUFF_SIZE);
	memset(m_DataBuffer, 0, CONST_BUFF_SIZE);

	m_ConnectState	= Not_Connect;

	m_u64ClientID = 0;

	CommonSocket::InitNetwork();
}

CClientConnector::~CClientConnector(void)
{
	m_u64ClientID = 0;

	m_ConnectState	= Not_Connect;

	m_hSocket		= INVALID_SOCKET;

	m_nDataLen = 0;
	m_PacketLen = 0;
	memset(m_PackBuffer, 0, CONST_BUFF_SIZE);
	memset(m_DataBuffer, 0, CONST_BUFF_SIZE);

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


BOOL CClientConnector::SendData(UINT32 dwMsgID, const google::protobuf::Message& pdata, UINT64 u64TargetID, UINT32 dwUserData)
{
	char szBuff[10240] = {0};

	PacketHeader* pHeader = (PacketHeader*)szBuff;

	pHeader->CheckCode = 0x88;
	pHeader->dwMsgID = dwMsgID;
	pHeader->u64TargetID = u64TargetID;
	pHeader->dwUserData = dwUserData;
	pHeader->dwSize = 28 + pdata.ByteSize();

	pdata.SerializePartialToArray(szBuff+28, pdata.ByteSize());

	return SendData( szBuff, pdata.ByteSize() + 28);
}

BOOL CClientConnector::SendData( char *pData, INT32 dwLen )
{
	if((pData == NULL)||(dwLen == 0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(m_ConnectState != Succ_Connect)
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

BOOL CClientConnector::ConnectLoginSvr(BOOL bConnect)
{
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

	ReceiveData();
	
	while(ProcessData());

	return TRUE;

}

BOOL CClientConnector::DispatchPacket(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	switch(dwMsgID)
	{
	default:
		{
			for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
			{
				IMessageHandler *pHandler = *itor;

				if(pHandler->DispatchPacket(dwMsgID, PacketBuf, BufLen))
				{
					break;
				}
			}
		}
		break;
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

	SetConnectState(Succ_Connect);

	return TRUE;
}

BOOL CClientConnector::DisConnect()
{
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

	m_ConnectState = Not_Connect;

	m_nDataLen = 0;
	m_PacketLen = 0;
	memset(m_PackBuffer, 0, CONST_BUFF_SIZE);
	memset(m_DataBuffer, 0, CONST_BUFF_SIZE);
	return TRUE;
}


void CClientConnector::SetConnectState( ConnectState val )
{
	if(val == Succ_Connect)
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

		printf("================recv data : %d!\n", nReadLen);
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
		memmove(m_DataBuffer, m_DataBuffer+pHeader->dwSize, m_nDataLen);
	}
	else if(m_nDataLen < 0)
	{
		ASSERT_FAIELD;
	}

	PacketHeader *pMsgHeader = (PacketHeader *)m_PackBuffer;

	DispatchPacket(pMsgHeader->dwMsgID, m_PackBuffer + 28, m_PacketLen - 28);

	return TRUE;
}

UINT32 CClientConnector::GetServerTime()
{
	UINT32 dwTick = ::GetTickCount();

	return m_dwServerTime + (m_dwServerTick - dwTick)/1000;
}

BOOL CClientConnector::SetLoginServerAddr( std::string strIpAddr, UINT16 sPort )
{
	m_strLoginIp = strIpAddr;
	m_sLoginPort = sPort;

	return TRUE;
}








