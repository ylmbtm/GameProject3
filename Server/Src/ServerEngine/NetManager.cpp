#include "stdafx.h"
#include "NetManager.h"
#include "Connection.h"
#include "CommonSocket.h"
#include "DataBuffer.h"
#include "PacketHeader.h"

CNetManager::CNetManager(void)
{
	m_hListenSocket		= NULL;
	m_hCompletePort		= NULL;
	m_bCloseEvent		= FALSE;
	m_pBufferHandler	= NULL;
}

CNetManager::~CNetManager(void)
{
	m_pBufferHandler = NULL;
}

BOOL CNetManager::CreateEventThread(UINT32 nNum )
{
	if(nNum == 0)
	{
		nNum = CommonFunc::GetProcessorNum();
	}

	ERROR_RETURN_FALSE(nNum > 0);

	m_bCloseEvent = FALSE;

	//目前linux使用单线程
#ifndef WIN32
	nNum = 1;
#endif

	for(UINT32 i = 0; i < nNum; ++i)
	{
		std::thread* pThread = new std::thread(&CNetManager::WorkThread_ProcessEvent, this, nNum);
		m_vtEventThread.push_back(pThread);
	}

	return TRUE;
}

BOOL CNetManager::WorkThread_Listen()
{
	sockaddr_in Con_Addr;
	socklen_t nLen = sizeof(Con_Addr);
	while(TRUE)
	{
		memset(&Con_Addr, 0, sizeof(Con_Addr));
		SOCKET hClientSocket = accept(m_hListenSocket, (sockaddr*)&Con_Addr, &nLen);
		if(hClientSocket == INVALID_SOCKET)
		{
			CLog::GetInstancePtr()->LogError("accept 错误 原因:%s!", CommonFunc::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
			return FALSE;
		}
		CommonSocket::SetSocketBlock(hClientSocket, FALSE);
		//CommonSocket::SetSocketNoDelay(hClientSocket);
		CConnection* pConnection = AssociateCompletePort(hClientSocket, FALSE);
		if(pConnection != NULL)
		{
			pConnection->m_dwIpAddr = Con_Addr.sin_addr.s_addr;

			pConnection->SetConnectionOK(TRUE);

			m_pBufferHandler->OnNewConnect(pConnection->GetConnectionID());

			//在Windows的IOCP模式，一个新的连接必须首先调一次接收， 而EPOLL模型下，只要关注了读事件就可以等事件到了之后发读的操作。
#ifdef WIN32
			if(!pConnection->DoReceive())
			{
				pConnection->Close();
			}
#endif
		}
		else
		{
			CLog::GetInstancePtr()->LogError("接受新连接失败 原因:己达到最大连接数或者绑定失败!");
		}
	}

	return TRUE;
}

BOOL CNetManager::StartNetListen(UINT16 nPortNum, std::string strIpAddr)
{
	sockaddr_in SvrAddr;
	SvrAddr.sin_family		= AF_INET;
	SvrAddr.sin_port		= htons(nPortNum);

	if (strIpAddr.size() <= 1)
	{
		SvrAddr.sin_addr.s_addr = htonl(INADDR_ANY);		//支持多IP地址监听
	}
	else
	{
		SvrAddr.sin_addr.s_addr = CommonSocket::IpAddrStrToInt(strIpAddr.c_str());
	}

	m_hListenSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(m_hListenSocket == INVALID_SOCKET)
	{
		CLog::GetInstancePtr()->LogError("创建监听套接字失败原因:%s!", CommonFunc::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	CommonSocket::SetSocketBlock(m_hListenSocket, FALSE);

	CommonSocket::SetSocketReuseable(m_hListenSocket);

	if(!CommonSocket::BindSocket(m_hListenSocket, (sockaddr*)&SvrAddr, sizeof(SvrAddr)))
	{
		CLog::GetInstancePtr()->LogError("邦定套接字失败原因:%s!", CommonFunc::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	if(!CommonSocket::ListenSocket(m_hListenSocket, 20))
	{
		CLog::GetInstancePtr()->LogError("监听线程套接字失败:%s!", CommonFunc::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	if (!WaitConnect())
	{
		CLog::GetInstancePtr()->LogError("等待接受连接失败:%s!", CommonFunc::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	return TRUE;
}


BOOL CNetManager::CreateCompletePort()
{
#ifdef WIN32
	m_hCompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, -1);
	ERROR_RETURN_FALSE(m_hCompletePort != NULL);
#else
	m_hCompletePort = epoll_create(10000);
	ERROR_RETURN_FALSE(m_hCompletePort != -1);
#endif

	return TRUE;
}

BOOL CNetManager::DestroyCompletePort()
{
#ifdef WIN32
	CloseHandle(m_hCompletePort);
#else
	close(m_hCompletePort);
#endif

	return TRUE;
}


#ifdef WIN32
BOOL CNetManager::WorkThread_ProcessEvent(UINT32 nParam)
{
	ERROR_RETURN_FALSE(m_hCompletePort != INVALID_HANDLE_VALUE);

	DWORD dwNumOfByte = 0;
	ULONG_PTR CompleteKey = 0;
	LPOVERLAPPED lpOverlapped = NULL;
	DWORD dwWaitTime = 1000;
	BOOL  bRetValue = FALSE;

	while(!m_bCloseEvent)
	{
		bRetValue = GetQueuedCompletionStatus(m_hCompletePort, &dwNumOfByte, &CompleteKey, &lpOverlapped, dwWaitTime);
		if(!bRetValue)
		{
			if (lpOverlapped == NULL)
			{
				if (ERROR_ABANDONED_WAIT_0 == CommonSocket::GetSocketLastError())
				{
					CLog::GetInstancePtr()->LogError("完成端口被外部关闭!");
					return FALSE;
				}

				if (CommonSocket::GetSocketLastError() == WAIT_TIMEOUT)
				{
					continue;
				}

				if (CommonSocket::GetSocketLastError() == ERROR_OPERATION_ABORTED)
				{
					return FALSE;
				}

				//if (CommonSocket::GetSocketLastError() == ERROR_NETNAME_DELETED)
				//{
				//  //客户端关闭的一种情况
				//}
			}
			//else
			//{
			//	NetIoOperatorData* pIoPeratorData = (NetIoOperatorData*)lpOverlapped;
			//	CLog::GetInstancePtr()->LogError(" GetQueuedCmpletionStatus Error: %d-%d-%s", pIoPeratorData->dwOpType, CommonFunc::GetLastError(), CommonFunc::GetLastErrorStr(CommonFunc::GetLastError()).c_str());
			//}
		}

		NetIoOperatorData* pIoPeratorData = (NetIoOperatorData*)lpOverlapped;
		switch( pIoPeratorData->dwOpType)
		{
			case NET_OP_RECV:
			{
				CConnection* pConnection = (CConnection*)CompleteKey;
				if(pConnection == NULL)
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_RECV1, pConnection == NULL");
					break;
				}

				if(dwNumOfByte == 0)
				{
					//说明对方己经关闭
					if(pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
					{
						CLog::GetInstancePtr()->LogError("触发了NET_MSG_RECV2, 对方己经关闭连接，但可能我们这边更快, 连接己经被重用了。nowid:%d, oldid:%d", pConnection->GetConnectionID(), pIoPeratorData->dwConnID);
						break;
					}
					pConnection->Close();
				}
				else
				{
					if(pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
					{
						CLog::GetInstancePtr()->LogError("触发了NET_MSG_RECV3，确实有数据, 但连接己经被重用了。nowid:%d, oldid:%d", pConnection->GetConnectionID(), pIoPeratorData->dwConnID);
						break;
					}

					if(pConnection->IsConnectionOK())
					{
						if(!pConnection->HandleRecvEvent(dwNumOfByte))
						{
							//收数据失败，基本就是连接己断开
							if(pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
							{
								CLog::GetInstancePtr()->LogError("触发了NET_MSG_RECV4, 但连接己经被关闭重用了。nowid:%d, oldid:%d", pConnection->GetConnectionID(), pIoPeratorData->dwConnID);
								break;
							}
							pConnection->Close();
						}
					}
					else
					{
						CLog::GetInstancePtr()->LogError("严重的错误, 没有连接上，却收到的数据!", pConnection);
					}
				}
			}
			break;
			case NET_OP_SEND:
			{
				pIoPeratorData->pDataBuffer->Release();
				CConnection* pConnection = (CConnection*)CompleteKey;
				if (pConnection == NULL)
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_SEND, pConnection == NULL。");
					break;
				}

				if(pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_SEND, 但连接己经被关闭重用了。");
					break;
				}

				pConnection->DoSend();
			}
			break;
			case NET_OP_POST:
			{
				CConnection* pConnection = (CConnection*)CompleteKey;
				if (pConnection == NULL)
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_POST1, pConnection == NULL。");
					break;
				}

				if (pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_POST2, 但连接己经被关闭重用了。");
					break;
				}

				pConnection->DoSend();
			}
			break;
			case NET_OP_CONNECT:
			{
				CConnection* pConnection = (CConnection*)CompleteKey;
				if (pConnection == NULL)
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_CONNECT, pConnection == NULL。");
					break;
				}

				if (pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_CONNECT, 事件ID和连接ID不一致。");
					break;
				}

				if(bRetValue)
				{
					pConnection->SetConnectionOK(TRUE);
					m_pBufferHandler->OnNewConnect(pConnection->GetConnectionID());

					if(!pConnection->DoReceive())
					{
						pConnection->Close();
					}
				}
				else
				{
					pConnection->SetConnectionOK(FALSE);
					pConnection->Close();
				}
			}
			break;
			case NET_OP_ACCEPT:
			{
				if (m_hCurAcceptSocket == INVALID_SOCKET)
				{
					break;
				}

				sockaddr_in* addrClient = NULL, *addrLocal = NULL;
				if (!CommonSocket::GetSocketAddress(m_hListenSocket, m_AddressBuf, addrClient, addrLocal))
				{
					CLog::GetInstancePtr()->LogError("接受新连接失败 原因:GetSocketAddress FALSE Reason:%s!", CommonFunc::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
					break;
				}

				CommonSocket::SetSocketBlock(m_hCurAcceptSocket, FALSE);
				//CommonSocket::SetSocketNoDelay(m_hCurAcceptSocket);
				CConnection* pConnection = AssociateCompletePort(m_hCurAcceptSocket, FALSE);
				if (pConnection != NULL)
				{
					pConnection->m_dwIpAddr = addrClient->sin_addr.s_addr;

					pConnection->SetConnectionOK(TRUE);

					m_pBufferHandler->OnNewConnect(pConnection->GetConnectionID());

					if (!pConnection->DoReceive())
					{
						pConnection->Close();
					}
				}
				else
				{
					CLog::GetInstancePtr()->LogError("接受新连接失败 原因:AssociateCompletePort己达到最大连接数或者绑定失败!");
				}

				m_IoOverlapAccept.Reset();
				m_IoOverlapAccept.dwOpType = NET_OP_ACCEPT;
				m_hCurAcceptSocket = CommonSocket::CreateSocket();
				CommonSocket::AcceptSocketEx(m_hListenSocket, m_hCurAcceptSocket, (CHAR*)m_AddressBuf, (LPOVERLAPPED)&m_IoOverlapAccept);
			}
			break;
		}
	}

	return TRUE;
}

CConnection* CNetManager::AssociateCompletePort( SOCKET hSocket, BOOL bConnect)
{
	ERROR_RETURN_NULL(hSocket != INVALID_SOCKET && hSocket != 0);
	CConnection* pConnection = CConnectionMgr::GetInstancePtr()->CreateConnection();
	if (pConnection == NULL)
	{
		CommonSocket::CloseSocket(hSocket);
		return FALSE;
	}
	pConnection->SetSocket(hSocket);
	pConnection->SetDataHandler(m_pBufferHandler);
	if(NULL == CreateIoCompletionPort((HANDLE)hSocket, m_hCompletePort, (ULONG_PTR)pConnection, 0))
	{
		CLog::GetInstancePtr()->LogError("AssociateCompletePort失败!");
		pConnection->Close();
	}

	return pConnection;
}

BOOL CNetManager::EventDelete(CConnection* pConnection)
{
	return TRUE;
}

#else

CConnection* CNetManager::AssociateCompletePort( SOCKET hSocket, BOOL bConnect)
{
	ERROR_RETURN_NULL(hSocket != INVALID_SOCKET && hSocket != 0);

	CConnection* pConnection = CConnectionMgr::GetInstancePtr()->CreateConnection();
	if (pConnection == NULL)
	{
		CommonSocket::CloseSocket(hSocket);
		return NULL;
	}

	pConnection->SetSocket(hSocket);

	pConnection->SetDataHandler(m_pBufferHandler);

	struct epoll_event EpollEvent;
	EpollEvent.data.ptr = pConnection;
	if (bConnect)
	{
		EpollEvent.events = EPOLLIN | EPOLLOUT | EPOLLET;
	}
	else
	{
		EpollEvent.events = EPOLLIN | EPOLLET;
	}

	if(-1 == epoll_ctl(m_hCompletePort, EPOLL_CTL_ADD, hSocket, &EpollEvent))
	{
		CLog::GetInstancePtr()->LogError("AssociateCompletePort失败!");
		pConnection->Close();
	}

	return pConnection;
}

BOOL CNetManager::WorkThread_ProcessEvent(UINT32 nParam)
{
	struct epoll_event vtEvents[512];
	int nFd = 0;
	while (!m_bCloseEvent)
	{
		nFd = epoll_wait(m_hCompletePort, vtEvents, 512, 500);
		if (nFd == -1)
		{
			if (errno != EINTR)
			{
				CLog::GetInstancePtr()->LogError("epoll_wait失败 原因:%s", CommonFunc::GetLastErrorStr(errno).c_str());
				return -1;
			}

			continue;
		}

		for (int i = 0; i < nFd; ++i)
		{
			if (vtEvents[i].data.fd == m_hListenSocket)
			{
				sockaddr_in Con_Addr;
				socklen_t nLen = sizeof(Con_Addr);
				while (TRUE)
				{
					memset(&Con_Addr, 0, sizeof(Con_Addr));
					SOCKET hClientSocket = accept(m_hListenSocket, (sockaddr*)&Con_Addr, &nLen);
					if (hClientSocket == INVALID_SOCKET)
					{
						if (errno != EAGAIN && errno != EINTR)
						{
							CLog::GetInstancePtr()->LogError("接受新连接失败 原因:%s", CommonFunc::GetLastErrorStr(errno).c_str());
						}

						break;
					}

					CommonSocket::SetSocketBlock(hClientSocket, FALSE);
					//CommonSocket::SetSocketNoDelay(hClientSocket);
					CConnection* pConnection = AssociateCompletePort(hClientSocket, FALSE);
					if (pConnection != NULL)
					{
						pConnection->m_dwIpAddr = Con_Addr.sin_addr.s_addr;

						pConnection->SetConnectionOK(TRUE);

						m_pBufferHandler->OnNewConnect(pConnection->GetConnectionID());
					}
					else
					{
						CLog::GetInstancePtr()->LogError("接受新连接失败 原因:己达到最大连接数或者绑定失败!");
						break;
					}
				}

				continue;
			}
			CConnection* pConnection = (CConnection*)vtEvents[i].data.ptr;
			if (pConnection == NULL)
			{
				CLog::GetInstancePtr()->LogError("---Invalid pConnection Ptr------------!");
				continue;
			}

			if ((vtEvents[i].events & EPOLLERR) || (vtEvents[i].events & EPOLLHUP))
			{
				EventDelete(pConnection);
				pConnection->Close();
				continue;
			}

			int nError;
			socklen_t len;
			if (getsockopt(pConnection->GetSocket(), SOL_SOCKET, SO_ERROR, (char*)&nError, &len) < 0)
			{
				CLog::GetInstancePtr()->LogError("-------getsockopt Error:%d--------失败----!", nError);
				continue;
			}

			if (nError != 0)
			{
				if (vtEvents[i].events & EPOLLIN)
				{
					CLog::GetInstancePtr()->LogError("-------EPOLLIN---------失败---!");
				}

				if (vtEvents[i].events & EPOLLOUT)
				{
					CLog::GetInstancePtr()->LogError("-------EPOLLOUT----失败-------!");
				}

				continue;
			}

			if (vtEvents[i].events & EPOLLIN)
			{
				if (!pConnection->IsConnectionOK())
				{
					pConnection->SetConnectionOK(TRUE);
					m_pBufferHandler->OnNewConnect(pConnection->GetConnectionID());
				}

				if (!pConnection->HandleRecvEvent(0))
				{
					//基本表明连接己断开，可以关闭连接了。
					EventDelete(pConnection);
					pConnection->Close();
					continue;
				}
				else
				{
					struct epoll_event EpollEvent;
					EpollEvent.data.ptr = pConnection;
					EpollEvent.events = EPOLLIN | EPOLLET;
					if(0 != epoll_ctl(m_hCompletePort, EPOLL_CTL_MOD, pConnection->GetSocket(), &EpollEvent))
					{
						CLog::GetInstancePtr()->LogError("socket 设置事件失败 原因:%s", CommonFunc::GetLastErrorStr(errno).c_str());
					}
				}
			}

			if (vtEvents[i].events & EPOLLOUT)
			{
				if (!pConnection->IsConnectionOK())
				{
					pConnection->SetConnectionOK(TRUE);
					m_pBufferHandler->OnNewConnect(pConnection->GetConnectionID());
				}

				UINT32 nRet = pConnection->DoSend();
				if (nRet == E_SEND_ERROR)
				{
					EventDelete(pConnection);
					pConnection->Close();
				}
				else if (nRet == E_SEND_UNDONE)
				{
					PostSendOperation(pConnection);
				}
				else if (nRet == E_SEND_SUCCESS)
				{
					struct epoll_event EpollEvent;
					EpollEvent.data.ptr = pConnection;
					EpollEvent.events = EPOLLIN | EPOLLET;
					if (0 != epoll_ctl(m_hCompletePort, EPOLL_CTL_MOD, pConnection->GetSocket(), &EpollEvent))
					{
						CLog::GetInstancePtr()->LogError("socket 设置事件失败 原因:%s", CommonFunc::GetLastErrorStr(errno).c_str());
					}
				}
			}
		}
	}

	return TRUE;
}


BOOL CNetManager::EventDelete(CConnection* pConnection)
{
	struct epoll_event delEpv = { 0, { 0 } };
	delEpv.data.ptr = pConnection;
	if (-1 == epoll_ctl(m_hCompletePort, EPOLL_CTL_DEL, pConnection->GetSocket(), &delEpv))
	{
		CLog::GetInstancePtr()->LogError("---epoll_ctl----epoll_ctl------失败------!");
		return FALSE;
	}

	return TRUE;
}

#endif


BOOL CNetManager::Start(UINT16 nPortNum, UINT32 nMaxConn, IDataHandler* pBufferHandler, std::string strIpAddr)
{
	ERROR_RETURN_FALSE(pBufferHandler != NULL);

	m_pBufferHandler = pBufferHandler;

	CConnectionMgr::GetInstancePtr()->InitConnectionList(nMaxConn);

	if(!InitNetwork())
	{
		CLog::GetInstancePtr()->LogError("初始化网络失败！！");
		return FALSE;
	}

	if(!CreateCompletePort())
	{
		CLog::GetInstancePtr()->LogError("创建完成端口或Epoll失败！！");
		return FALSE;
	}

	if(!CreateEventThread(0))
	{
		CLog::GetInstancePtr()->LogError("创建网络事件处理线程失败！！");
		return FALSE;
	}

	if(!StartNetListen(nPortNum, strIpAddr))
	{
		CLog::GetInstancePtr()->LogError("开启监听失败！！");
		return FALSE;
	}

	return TRUE;
}

BOOL CNetManager::InitNetwork()
{
	return CommonSocket::InitNetwork();
}

BOOL CNetManager::UninitNetwork()
{
	return CommonSocket::UninitNetwork();
}

BOOL CNetManager::Stop()
{
	StopListen();

	CloseEventThread();

	CConnectionMgr::GetInstancePtr()->CloseAllConnection();

	DestroyCompletePort();

	UninitNetwork();

	CConnectionMgr::GetInstancePtr()->DestroyAllConnection();

	return TRUE;
}

BOOL CNetManager::StopListen()
{
	CommonSocket::CloseSocket(m_hListenSocket);

	CommonSocket::CloseSocket(m_hCurAcceptSocket);

	return TRUE;
}

CConnection* CNetManager::ConnectTo_Sync( std::string strIpAddr, UINT16 sPort )
{
	SOCKET hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
	{
		CommonSocket::CloseSocket(hSocket);
		CLog::GetInstancePtr()->LogError("创建套接字失败!!");
		return NULL;
	}

	CommonSocket::SetSocketBlock(hSocket, TRUE);

	//CommonSocket::SetSocketNoDelay(hSocket);

	if(!CommonSocket::ConnectSocket(hSocket, strIpAddr.c_str(), sPort))
	{
		CommonSocket::CloseSocket(hSocket);
		return NULL;
	}

	CConnection* pConnection = AssociateCompletePort(hSocket, TRUE);
	if(pConnection == NULL)
	{
		CLog::GetInstancePtr()->LogError("邦定套接字到完成端口失败!!");
		return NULL;
	}

	CommonSocket::SetSocketBlock(hSocket, FALSE);

	pConnection->SetConnectionOK(TRUE);

	m_pBufferHandler->OnNewConnect(pConnection->GetConnectionID());

	if(!pConnection->DoReceive())
	{
		pConnection->Close();
	}

	return pConnection;
}

CConnection* CNetManager::ConnectTo_Async( std::string strIpAddr, UINT16 sPort )
{
	SOCKET hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET || hSocket == 0)
	{
		CommonSocket::CloseSocket(hSocket);
		CLog::GetInstancePtr()->LogError("创建套接字失败!!");
		return NULL;
	}

	CommonSocket::SetSocketBlock(hSocket, FALSE);

	//CommonSocket::SetSocketNoDelay(hSocket);

#ifdef WIN32
	CConnection* pConnection = AssociateCompletePort(hSocket, TRUE);
	if (pConnection == NULL)
	{
		CLog::GetInstancePtr()->LogError("邦定套接字到完成端口失败!!");

		return NULL;
	}

	pConnection->m_IoOverlapRecv.Reset();

	pConnection->m_IoOverlapRecv.dwOpType = NET_OP_CONNECT;

	pConnection->m_IoOverlapRecv.dwConnID = pConnection->GetConnectionID();

	pConnection->m_dwIpAddr = CommonSocket::IpAddrStrToInt((CHAR*)strIpAddr.c_str());

	BOOL bRet = CommonSocket::ConnectSocketEx(hSocket, strIpAddr.c_str(), sPort, (LPOVERLAPPED)&pConnection->m_IoOverlapRecv);

	if(!bRet)
	{
		CLog::GetInstancePtr()->LogError("ConnectTo_Async 连接目标服务器失败,IP:%s--Port:%d!!", strIpAddr.c_str(), sPort);
		pConnection->Close();
	}

	return pConnection;
#else
	BOOL bRet = CommonSocket::ConnectSocket(hSocket, strIpAddr.c_str(), sPort);
	if (!bRet)
	{
		CLog::GetInstancePtr()->LogError("ConnectTo_Async 连接失败,IP:%s,Port:%d!", strIpAddr.c_str(), sPort);
		CommonSocket::CloseSocket(hSocket);
		return NULL;
	}

	CConnection* pConnection = AssociateCompletePort(hSocket, TRUE);
	if (pConnection == NULL)
	{
		CLog::GetInstancePtr()->LogError("邦定套接字到完成端口失败!!");

		return NULL;
	}

	return pConnection;
#endif
}

BOOL CNetManager::WaitConnect()
{
#ifdef WIN32
	if (NULL == CreateIoCompletionPort((HANDLE)m_hListenSocket, m_hCompletePort, (ULONG_PTR)NULL, 0))
	{
		CLog::GetInstancePtr()->LogError("WaitConnect邦定Listen套接字失败:%s!", CommonFunc::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}
	m_IoOverlapAccept.Reset();
	m_IoOverlapAccept.dwOpType = NET_OP_ACCEPT;
	m_hCurAcceptSocket = CommonSocket::CreateSocket();
	return CommonSocket::AcceptSocketEx(m_hListenSocket, m_hCurAcceptSocket, (CHAR*)m_AddressBuf, (LPOVERLAPPED)&m_IoOverlapAccept);
#else
	struct epoll_event epollev;
	epollev.data.fd = m_hListenSocket;
	epollev.events = EPOLLIN | EPOLLET;
	return (-1 < epoll_ctl(m_hCompletePort, EPOLL_CTL_ADD, m_hListenSocket, &epollev));
#endif
}

BOOL CNetManager::SendMessageData(UINT32 dwConnID,  UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData,  const char* pData, UINT32 dwLen)
{
	if (dwConnID <= 0)
	{
		return FALSE;
	}

	CConnection* pConn = CConnectionMgr::GetInstancePtr()->GetConnectionByID(dwConnID);
	if(pConn == NULL)
	{
		//表示连接己经失败断开了，这个连接ID不可用了。
		return FALSE;
	}

	if(!pConn->IsConnectionOK())
	{
		CLog::GetInstancePtr()->LogError("CNetManager::SendMessageByConnID FAILED, 连接己断开");
		return FALSE;
	}

	IDataBuffer* pDataBuffer = CBufferAllocator::GetInstancePtr()->AllocDataBuff(dwLen + sizeof(PacketHeader));
	ERROR_RETURN_FALSE(pDataBuffer != NULL);

	PacketHeader* pHeader = (PacketHeader*)pDataBuffer->GetBuffer();
	pHeader->CheckCode = CODE_VALUE;
	pHeader->dwUserData = dwUserData;
	pHeader->u64TargetID = u64TargetID;
	pHeader->dwSize = dwLen + sizeof(PacketHeader);
	pHeader->dwMsgID = dwMsgID;
	pHeader->dwPacketNo = 1;

	memcpy(pDataBuffer->GetBuffer() + sizeof(PacketHeader), pData, dwLen);

	pDataBuffer->SetTotalLenth(pHeader->dwSize);

	if (pConn->SendBuffer(pDataBuffer))
	{
		PostSendOperation(pConn);
		return TRUE;
	}

	return FALSE;
}

BOOL CNetManager::SendMessageBuff(UINT32 dwConnID, IDataBuffer* pBuffer)
{
	ERROR_RETURN_FALSE(dwConnID != 0);
	CConnection* pConn = CConnectionMgr::GetInstancePtr()->GetConnectionByID(dwConnID);
	if(pConn == NULL)
	{
		//表示连接己经失败断开了，这个连接ID不可用了。
		return FALSE;
	}

	if(!pConn->IsConnectionOK())
	{
		CLog::GetInstancePtr()->LogError("CNetManager::SendMsgBufByConnID FAILED, 连接己断开");
		return FALSE;
	}

	pBuffer->AddRef();
	if (pConn->SendBuffer(pBuffer))
	{
		PostSendOperation(pConn);
		return TRUE;
	}

	return FALSE;
}

BOOL CNetManager::CloseEventThread()
{
	m_bCloseEvent = TRUE;

	for(std::vector<std::thread*>::iterator itor = m_vtEventThread.begin(); itor != m_vtEventThread.end(); ++itor)
	{
		(*itor)->join();

		delete (*itor);
	}

	m_vtEventThread.clear();

	return TRUE;
}

BOOL CNetManager::PostSendOperation(CConnection* pConnection, BOOL bCheck)
{
	ERROR_RETURN_FALSE(pConnection != NULL);

	if (!pConnection->m_IsSending || !bCheck)
	{
#ifdef WIN32
		pConnection->m_IsSending = TRUE;
		pConnection->m_IoOverLapPost.Reset();
		pConnection->m_IoOverLapPost.dwOpType = NET_OP_POST;
		pConnection->m_IoOverLapPost.dwConnID = pConnection->GetConnectionID();
		PostQueuedCompletionStatus(m_hCompletePort, pConnection->GetConnectionID(), (ULONG_PTR)pConnection, (LPOVERLAPPED)&pConnection->m_IoOverLapPost);
#else
		struct epoll_event EpollEvent;
		EpollEvent.data.ptr = pConnection;
		EpollEvent.events = EPOLLOUT | EPOLLET;
		epoll_ctl(m_hCompletePort, EPOLL_CTL_MOD, pConnection->GetSocket(), &EpollEvent);
#endif
	}
	return TRUE;
}