#include "stdafx.h"
#include "NetManager.h"
#include "Connection.h"
#include "CommonSocket.h"
#include "CommonFunc.h"
#include "CommandDef.h"
#include "DataBuffer.h"
#include "Log.h"


CNetManager::CNetManager(void)
{
	m_hListenThread		= (THANDLE)NULL;
	m_hListenSocket		= NULL;
	m_hCompletePort		= NULL;
	m_bCloseSend		= TRUE;
	m_bCloseEvent		= TRUE;
	m_pBufferHandler	= NULL;
}

CNetManager::~CNetManager(void)
{
}

BOOL CNetManager::CreateEventThread( int nNum )
{
	if(nNum == 0)
	{
		nNum = CommonFunc::GetProcessorNum();
	}

	ERROR_RETURN_FALSE(nNum > 0);

	m_bCloseEvent = FALSE;

	for(int i = 0; i < nNum; ++i)
	{
		THANDLE hThread = CommonThreadFunc::CreateThread(_NetEventThread, (void*)this);
		m_vtEventThread.push_back(hThread);
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
			CLog::GetInstancePtr()->LogError("accept 错误 原因:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());

			break;
		}
		CommonSocket::SetSocketUnblock(hClientSocket);
		CConnection* pConnection = AssociateCompletePort(hClientSocket);
		if(pConnection != NULL)
		{
			pConnection->m_dwIpAddr = Con_Addr.sin_addr.S_un.S_addr;

			pConnection->SetConnectionOK(TRUE);

			m_pBufferHandler->OnNewConnect(pConnection);
#ifdef WIN32
			if(!pConnection->DoReceive())
			{
				pConnection->Close();
			}
#endif
		}
		else
		{
			CLog::GetInstancePtr()->LogError("accept 错误 原因:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		}
	}

	return TRUE;
}


BOOL CNetManager::StartListen(UINT16 nPortNum)
{
	sockaddr_in SvrAddr;
	SvrAddr.sin_family		= AF_INET;
	SvrAddr.sin_port		= htons(nPortNum);
	SvrAddr.sin_addr.s_addr	= htonl(INADDR_ANY);		//支持多IP地址监听
	//inet_pton(AF_INET, CGlobalConfig::GetInstancePtr()->m_strIpAddr.c_str(), &SvrAddr.sin_addr);

	m_hListenSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(m_hListenSocket == INVALID_SOCKET)
	{
		CLog::GetInstancePtr()->LogError("创建监听套接字失败原因:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	CommonSocket::SetSocketReuseable(m_hListenSocket);

	if(!CommonSocket::BindSocket(m_hListenSocket, (sockaddr*)&SvrAddr, sizeof(SvrAddr)))
	{
		CLog::GetInstancePtr()->LogError("邦定套接字失败原因:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	if(!CommonSocket::ListenSocket(m_hListenSocket, 20))
	{
		CLog::GetInstancePtr()->LogError("监听线程套接字失败:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	if((m_hListenThread = CommonThreadFunc::CreateThread(_NetListenThread,  (void*)NULL)) == NULL)
	{
		CLog::GetInstancePtr()->LogError("创建监听线程失败:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	return TRUE;
}


#ifdef WIN32

BOOL CNetManager::WorkThread_ProcessEvent()
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
			if(lpOverlapped == NULL)
			{
				if(ERROR_ABANDONED_WAIT_0 == CommonSocket::GetSocketLastError())
				{
					CLog::GetInstancePtr()->LogError("完成端口被外部关闭!");
					return FALSE;
				}

				if(CommonSocket::GetSocketLastError() == WAIT_TIMEOUT)
				{
					continue;
				}
			}
		}

		NetIoOperatorData* pIoPeratorData = (NetIoOperatorData*)lpOverlapped;
		switch( pIoPeratorData->dwCmdType )
		{
			case NET_MSG_RECV:
			{
				CConnection* pConnection = (CConnection*)CompleteKey;
				if(pConnection == NULL)
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_RECV4, 但连接指针己经为空了");
					break;
				}

				if(dwNumOfByte == 0)
				{
					//说明对方己经关闭
					if(pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
					{
						CLog::GetInstancePtr()->LogError("触发了NET_MSG_RECV, 对方己经关闭连接，但可能我们这边更快, 连接己经被重用了。nowid:%d, oldid:%d", pConnection->GetConnectionID(), pIoPeratorData->dwConnID);
						break;
					}
					pConnection->Close();
				}
				else
				{
					if(pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
					{
						CLog::GetInstancePtr()->LogError("触发了NET_MSG_RECV，确实有数据, 但连接己经被重用了。nowid:%d, oldid:%d", pConnection->GetConnectionID(), pIoPeratorData->dwConnID);
						break;
					}

					if(pConnection->IsConnectionOK())
					{
						if(!pConnection->HandleRecvEvent(dwNumOfByte))
						{
							//收数据失败，基本就是连接己断开
							if(pConnection->GetConnectionID() != pIoPeratorData->dwConnID)
							{
								CLog::GetInstancePtr()->LogError("触发了NET_MSG_RECV3, 但连接己经被关闭重用了。nowid:%d, oldid:%d", pConnection->GetConnectionID(), pIoPeratorData->dwConnID);
								break;
							}
							pConnection->Close();
							CLog::GetInstancePtr()->LogError("收到的数据格式错误%x!", pConnection);
						}
					}
					else
					{
						ASSERT_FAIELD;
						CLog::GetInstancePtr()->LogError("严重的错误, 没有连接上，却收到的数据!", pConnection);
					}
				}
			}
			break;
			case NET_MSG_SEND:
			{
				pIoPeratorData->pDataBuffer->Release();
				CConnection* pConnection = (CConnection*)CompleteKey;
				if((pConnection != NULL) && (pConnection->GetConnectionID() != pIoPeratorData->dwConnID))
				{
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_SEND, 但连接己经被关闭重用了。");
					break;
				}

				if(pConnection != NULL)
				{
					pConnection->DoSend(NULL);
				}
				else
				{
					ASSERT_FAIELD;
					CLog::GetInstancePtr()->LogError("触发了NET_MSG_SEND,连接指针为空。");
				}
			}
			break;
			case NET_MSG_CONNECT:
			{
				CConnection* pConnection = (CConnection*)CompleteKey;
				if(pConnection != NULL)
				{
					if(bRetValue)
					{
						pConnection->SetConnectionOK(TRUE);
						m_pBufferHandler->OnNewConnect(pConnection);

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
			}
			break;
			default:
			{

			}
			break;
		}
	}

	return TRUE;
}


BOOL CNetManager::CreateCompletePort()
{
	m_hCompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, -1);
	ERROR_RETURN_FALSE(m_hCompletePort != NULL);

	return TRUE;
}

CConnection* CNetManager::AssociateCompletePort( SOCKET hSocket )
{
	CConnection* pConnection = CConnectionMgr::GetInstancePtr()->CreateConnection();
	ERROR_RETURN_NULL(pConnection != NULL);
	pConnection->SetSocket(hSocket);
	pConnection->SetDataHandler(m_pBufferHandler);
	if(NULL == CreateIoCompletionPort((HANDLE)hSocket, m_hCompletePort, (ULONG_PTR)pConnection, 0))
	{
		pConnection->Close();
		return NULL;
	}
	return pConnection;
}

BOOL CNetManager::DestroyCompletePort()
{
	CloseHandle(m_hCompletePort);

	return TRUE;
}

BOOL CNetManager::CreateDispatchThread()
{
	return TRUE;
}

BOOL CNetManager::WorkThread_DispathEvent()
{
	return TRUE;
}

#else

BOOL CNetManager::CreateCompletePort()
{
	m_hCompletePort = epoll_create(10000);
	ERROR_RETURN_FALSE(m_hCompletePort != -1);

	return TRUE;
}

CConnection* CNetManager::AssociateCompletePort( SOCKET hSocket )
{
	CConnection* pConnection = CConnectionMgr::GetInstancePtr()->CreateConnection();

	pConnection->SetSocket(hSocket);

	pConnection->SetDataHandler(m_pBufferHandler);

	struct epoll_event EpollEvent;
	EpollEvent.data.ptr = pConnection;
	EpollEvent.events  = EPOLLIN | EPOLLOUT | EPOLLET;

	if(-1 == epoll_ctl(m_hCompletePort, EPOLL_CTL_ADD, hSocket, &EpollEvent))
	{
		pConnection->Close();

		return NULL;
	}

	return pConnection;
}

BOOL CNetManager::DestroyCompletePort()
{
	close(m_hCompletePort);

	return TRUE;
}

BOOL CNetManager::CreateDispatchThread()
{
	m_bCloseDispath = FALSE;

	m_hDispathThread = CommonThreadFunc::CreateThread(_NetEventDispatchThread,  (void*)NULL);
	ERROR_RETURN_FALSE(m_hDispathThread != (THANDLE)NULL);

	return TRUE;
}

BOOL CNetManager::WorkThread_DispathEvent()
{
	struct epoll_event EpollEvent[20];
	int nFd = 0;
	EventNode _EventNode;
	while(!m_bCloseDispath)
	{
		nFd = epoll_wait(m_hCompletePort, EpollEvent, 20, 1000);
		if(nFd == -1)
		{
			continue;
		}

		for(int i = 0; i < nFd; ++i)
		{
			CConnection* pConnection = (CConnection*)EpollEvent[i].data.ptr;
			if(pConnection == NULL)
			{
				continue ;
			}

			if((EpollEvent[i].events & EPOLLERR) || (EpollEvent[i].events & EPOLLHUP))
			{
				if(!pConnection->IsConnectionOK())
				{
					CLog::GetInstancePtr()->LogError("---未连接socket收到这个消息----EPOLLERR------------!");
					continue;
				}

				CLog::GetInstancePtr()->LogError("---己连接socket收到这个消息----EPOLLERR------------!");

				continue;
			}

			int nError;
			socklen_t len;

			if(getsockopt(pConnection->GetSocket(), SOL_SOCKET, SO_ERROR, (char*)&nError, &len) < 0)
			{
				CLog::GetInstancePtr()->LogError("-------getsockopt Error:%d--------成功----!", nError);
				continue;
			}

			if(EpollEvent[i].events & EPOLLIN)
			{
				if(nError == 0)
				{
					_EventNode.dwEvent = EVENT_READ;

					_EventNode.pPtr = EpollEvent[i].data.ptr;

					m_DispatchEventList.Push(_EventNode);

					CLog::GetInstancePtr()->LogError("-------EPOLLIN--------成功----!");
				}
				else
				{
					CLog::GetInstancePtr()->LogError("-------EPOLLIN---------失败---!");
				}
			}

			if(EpollEvent[i].events & EPOLLOUT)
			{
				if(nError == 0)
				{
					if(!pConnection->IsConnectionOK())
					{
						pConnection->SetConnectionOK(TRUE);

						_EventNode.dwEvent = EVENT_WRITE;

						_EventNode.pPtr = EpollEvent[i].data.ptr;

						m_DispatchEventList.Push(_EventNode);
					}

					CLog::GetInstancePtr()->LogError("-------EPOLLOUT-----成功-------!");
				}
				else
				{
					CLog::GetInstancePtr()->LogError("-------EPOLLOUT----失败-------!");
				}
			}
		}
	}

	return TRUE;
}

BOOL CNetManager::WorkThread_ProcessEvent()
{
	EventNode _EventNode;
	while(TRUE)
	{
		if(!m_DispatchEventList.Pop(_EventNode))
		{
			continue;
		}

		CConnection* pConnection = (CConnection*)_EventNode.pPtr;
		if(pConnection == NULL)
		{
			if((_EventNode.dwEvent != EVENT_READ) && (_EventNode.dwEvent != EVENT_WRITE))
			{
				CLog::GetInstancePtr()->LogError("错误:取出一个空事件!");
			}

			continue;
		}

		if(_EventNode.dwEvent == EVENT_READ)
		{
			if(!pConnection->HandleRecvEvent(0))
			{
				//基本表明连接己断开，可以关闭连接了。
				pConnection->Close();
			}
			else
			{
				struct epoll_event EpollEvent;
				EpollEvent.data.ptr = pConnection;
				EpollEvent.events  = EPOLLIN | EPOLLET;

				epoll_ctl(m_hCompletePort, EPOLL_CTL_MOD, pConnection->GetSocket(),  &EpollEvent);
			}
		}
		else if(_EventNode.dwEvent == EVENT_WRITE)
		{
			pConnection->m_CritSecSendList.Lock();
			pConnection->m_IsSending = FALSE;
			pConnection->DoSend();
			struct epoll_event EpollEvent;
			EpollEvent.data.ptr = pConnection;
			EpollEvent.events  = EPOLLOUT | EPOLLET;
			epoll_ctl(m_hCompletePort, EPOLL_CTL_MOD, pConnection->GetSocket(),  &EpollEvent);
			pConnection->m_CritSecSendList.Unlock();
		}
	}

	return TRUE;
}


#endif


BOOL CNetManager::Start(UINT16 nPortNum, UINT32 nMaxConn, IDataHandler* pBufferHandler )
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

	if(!CreateDispatchThread())
	{
		CLog::GetInstancePtr()->LogError("创建事件分发线程失败！！");
		return FALSE;
	}

	if(!StartListen(nPortNum))
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

BOOL CNetManager::Close()
{
	StopListen();

	CloseDispatchThread();

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

	CommonThreadFunc::WaitThreadExit(m_hListenThread);

	return TRUE;
}

CConnection* CNetManager::ConnectToOtherSvr( std::string strIpAddr, UINT16 sPort )
{
	SOCKET hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
	{
		CommonSocket::CloseSocket(hSocket);
		CLog::GetInstancePtr()->LogError("创建套接字失败!!");
		return NULL;
	}

	CommonSocket::SetSocketBlock(hSocket);

	CommonSocket::SetSocketNoDelay(hSocket);

	if(!CommonSocket::ConnectSocket(hSocket, strIpAddr.c_str(), sPort))
	{
		CommonSocket::CloseSocket(hSocket);
		return NULL;
	}

	CConnection* pConnection = AssociateCompletePort(hSocket);
	if(pConnection == NULL)
	{
		CLog::GetInstancePtr()->LogError("邦定套接字到完成端口失败!!");
		return NULL;
	}

	pConnection->SetConnectionOK(TRUE);
	m_pBufferHandler->OnNewConnect(pConnection);

	if(!pConnection->DoReceive())
	{
		pConnection->Close();
		return NULL;
	}

	return pConnection;
}

CConnection* CNetManager::ConnectToOtherSvrEx( std::string strIpAddr, UINT16 sPort )
{
	SOCKET hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
	{
		CommonSocket::CloseSocket(hSocket);
		CLog::GetInstancePtr()->LogError("创建套接字失败!!");
		return NULL;
	}

	CommonSocket::SetSocketUnblock(hSocket);

	CommonSocket::SetSocketNoDelay(hSocket);

	CConnection* pConnection = AssociateCompletePort(hSocket);
	if(pConnection == NULL)
	{
		CLog::GetInstancePtr()->LogError("邦定套接字到完成端口失败!!");

		return NULL;
	}

#ifdef WIN32

	pConnection->m_IoOverlapRecv.Clear();

	pConnection->m_IoOverlapRecv.dwCmdType = NET_MSG_CONNECT;

	BOOL bRet = CommonSocket::ConnectSocketEx(hSocket, strIpAddr.c_str(), sPort, (LPOVERLAPPED)&pConnection->m_IoOverlapRecv);

#else

	BOOL bRet = CommonSocket::ConnectSocket(hSocket, strIpAddr.c_str(), sPort);

#endif
	if(!bRet)
	{
		pConnection->Close();

		CLog::GetInstancePtr()->LogError("连接服务器%s : %d失败!!", strIpAddr.c_str(), sPort);

		return NULL;
	}

	pConnection->m_dwIpAddr = CommonSocket::IpAddrStrToInt((CHAR*)strIpAddr.c_str());

	return pConnection;
}

BOOL CNetManager::SendMessageByConnID(UINT32 dwConnID,  UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData,  const char* pData, UINT32 dwLen)
{
	ERROR_RETURN_FALSE(dwConnID != 0);
	CConnection* pConn = CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(dwConnID);
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
	return pConn->SendMessage(dwMsgID, u64TargetID, dwUserData, pData, dwLen);
}

BOOL CNetManager::SendMsgBufByConnID(UINT32 dwConnID, IDataBuffer* pBuffer)
{
	ERROR_RETURN_FALSE(dwConnID != 0);
	CConnection* pConn = CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(dwConnID);
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
	return pConn->SendBuffer(pBuffer);
}

BOOL CNetManager::CloseEventThread()
{
	m_bCloseEvent = TRUE;

	for(std::vector<THANDLE>::iterator itor = m_vtEventThread.begin(); itor != m_vtEventThread.end(); ++itor)
	{
		CommonThreadFunc::WaitThreadExit(*itor);
	}

	return TRUE;
}

BOOL CNetManager::CloseDispatchThread()
{
	CommonThreadFunc::WaitThreadExit(m_hDispathThread);

	return TRUE;
}

Th_RetName _NetEventThread( void* pParam )
{
	CNetManager* pNetManager = CNetManager::GetInstancePtr();

	pNetManager->WorkThread_ProcessEvent();

	CLog::GetInstancePtr()->LogError("网络事件处理线程退出!");

	CommonThreadFunc::ExitThread();

	return Th_RetValue;
}

Th_RetName _NetListenThread( void* pParam )
{
	CNetManager* pNetManager = CNetManager::GetInstancePtr();

	pNetManager->WorkThread_Listen();

	CLog::GetInstancePtr()->LogInfo("监听线程退出!");

	CommonThreadFunc::ExitThread();

	return Th_RetValue;
}

Th_RetName _NetEventDispatchThread(void* pParam )
{
	CNetManager* pNetManager = CNetManager::GetInstancePtr();

	pNetManager->WorkThread_DispathEvent();

	CLog::GetInstancePtr()->LogInfo("事件分发线程退出!");

	CommonThreadFunc::ExitThread();

	return Th_RetValue;
}
