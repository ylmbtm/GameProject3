#include "stdafx.h"
#include "SpinLock.h"
#include "ServiceBase.h"
#include "NetManager.h"
#include "CommonSocket.h"
#include "CommonEvent.h"
#include "DataBuffer.h"
#include "Connection.h"
#include "TimerManager.h"
#include "PacketHeader.h"

#define NEW_CONNECTION 1
#define CLOSE_CONNECTION 2

ServiceBase::ServiceBase(void)
{
	m_pPacketDispatcher = NULL;
	m_pRecvDataQueue = new std::deque<NetPacket>();
	m_pDispathQueue = new std::deque<NetPacket>();
}

ServiceBase::~ServiceBase(void)
{
	delete m_pRecvDataQueue;
	delete m_pDispathQueue;

	m_pRecvDataQueue = NULL;
	m_pDispathQueue = NULL;
}

ServiceBase* ServiceBase::GetInstancePtr()
{
	static ServiceBase _ServiceBase;

	return &_ServiceBase;
}


BOOL ServiceBase::OnDataHandle(IDataBuffer* pDataBuffer, UINT32 nConnID)
{
	PacketHeader* pHeader = (PacketHeader*)pDataBuffer->GetBuffer();

	m_QueueLock.Lock();
	m_pRecvDataQueue->emplace_back(NetPacket(nConnID, pDataBuffer, pHeader->dwMsgID));
	m_QueueLock.Unlock();
	return TRUE;
}

BOOL ServiceBase::StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher* pDispather, std::string strListenIp)
{
	ERROR_RETURN_FALSE(pDispather != NULL);
	ERROR_RETURN_FALSE(nPortNum > 0);
	ERROR_RETURN_FALSE(nMaxConn > 0);

	m_pPacketDispatcher = pDispather;

	if (!CNetManager::GetInstancePtr()->Start(nPortNum, nMaxConn, this, strListenIp))
	{
		CLog::GetInstancePtr()->LogError("启动网络层失败!");
		return FALSE;
	}

	m_dwLastTick = 0;
	m_dwRecvNum = 0;
	m_dwFps = 0;
	m_dwSendNum = 0;
	m_dwLastMsgID = 0;
	return TRUE;
}

BOOL ServiceBase::StopNetwork()
{
	CNetManager::GetInstancePtr()->Stop();

	return TRUE;
}

template<typename T>
BOOL ServiceBase::SendMsgStruct(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, T& Data)
{
	if (dwConnID <= 0)
	{
		return FALSE;
	}

	m_dwSendNum++;

	return CNetManager::GetInstancePtr()->SendMessageData(dwConnID, dwMsgID, u64TargetID, dwUserData, &Data, sizeof(T));
}

BOOL ServiceBase::SendMsgProtoBuf(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
{
	if (dwConnID <= 0)
	{
		CLog::GetInstancePtr()->LogWarn("SendMsgProtoBuf Error dwConnID is Zero MessageID:%d", dwMsgID);
		return FALSE;
	}

	char szBuff[102400] = {0};

	ERROR_RETURN_FALSE(pdata.ByteSize() < 102400);

	pdata.SerializePartialToArray(szBuff, pdata.GetCachedSize());
	m_dwSendNum++;
	return CNetManager::GetInstancePtr()->SendMessageData(dwConnID, dwMsgID, u64TargetID, dwUserData, szBuff, pdata.GetCachedSize());
}

BOOL ServiceBase::SendMsgRawData(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pdata, UINT32 dwLen)
{
	if (dwConnID <= 0)
	{
		return FALSE;
	}

	m_dwSendNum++;

	return CNetManager::GetInstancePtr()->SendMessageData(dwConnID, dwMsgID, u64TargetID, dwUserData, pdata, dwLen);
}

BOOL ServiceBase::SendMsgBuffer(UINT32 dwConnID, IDataBuffer* pDataBuffer)
{
	if (dwConnID == 0)
	{
		return FALSE;
	}

	m_dwSendNum++;
	return CNetManager::GetInstancePtr()->SendMessageBuff(dwConnID, pDataBuffer);
}

CConnection* ServiceBase::ConnectTo( std::string strIpAddr, UINT16 sPort )
{
	ERROR_RETURN_NULL(!strIpAddr.empty() && sPort > 0);

	return CNetManager::GetInstancePtr()->ConnectTo_Async(strIpAddr, sPort);
}

BOOL ServiceBase::CloseConnect(UINT32 nConnID)
{
	CConnection* pConnection = GetConnectionByID(nConnID);

	ERROR_RETURN_FALSE(pConnection != NULL);

	pConnection->Close();

	return TRUE;
}

BOOL ServiceBase::OnCloseConnect(UINT32 nConnID)
{
	ERROR_RETURN_FALSE(nConnID != 0);
	m_QueueLock.Lock();
	m_pRecvDataQueue->emplace_back(NetPacket(nConnID, NULL, CLOSE_CONNECTION));
	m_QueueLock.Unlock();
	return TRUE;
}

BOOL ServiceBase::OnNewConnect(UINT32 nConnID)
{
	ERROR_RETURN_FALSE(nConnID != 0);
	m_QueueLock.Lock();
	m_pRecvDataQueue->emplace_back(NetPacket(nConnID, NULL, NEW_CONNECTION));
	m_QueueLock.Unlock();
	return TRUE;
}


CConnection* ServiceBase::GetConnectionByID( UINT32 dwConnID )
{
	return CConnectionMgr::GetInstancePtr()->GetConnectionByID(dwConnID);
}

BOOL ServiceBase::Update()
{
	if (m_dwLastTick == 0)
	{
		m_dwLastTick = CommonFunc::GetTickCount();
	}

	m_QueueLock.Lock();
	std::swap(m_pRecvDataQueue, m_pDispathQueue);
	m_QueueLock.Unlock();

	if (m_pDispathQueue->size() > 0)
	{
		for (std::deque<NetPacket>::iterator itor = m_pDispathQueue->begin(); itor != m_pDispathQueue->end(); ++itor)
		{
			NetPacket& item = *itor;
			if (item.m_dwMsgID == NEW_CONNECTION)
			{
				m_pPacketDispatcher->OnNewConnect(item.m_dwConnID);
			}
			else if (item.m_dwMsgID == CLOSE_CONNECTION)
			{
				m_pPacketDispatcher->OnCloseConnect(item.m_dwConnID);
				//发送通知
				CConnectionMgr::GetInstancePtr()->DeleteConnection(item.m_dwConnID);
			}
			else
			{
				m_dwLastMsgID = item.m_dwMsgID;
				m_pPacketDispatcher->DispatchPacket(&item);

				item.m_pDataBuffer->Release();

				m_dwRecvNum += 1;
			}
		}

		m_pDispathQueue->clear();
	}

	m_dwFps += 1;

	if((CommonFunc::GetTickCount() - m_dwLastTick) > 1000)
	{
		m_pPacketDispatcher->OnSecondTimer();

		CLog::GetInstancePtr()->SetTitle("[FPS:%d]-[RecvPack:%d]--[SendPack:%d]", m_dwFps, m_dwRecvNum, m_dwSendNum);
		m_dwFps = 0;
		m_dwRecvNum = 0;
		m_dwSendNum = 0;
		m_dwLastTick = CommonFunc::GetTickCount();
	}

	TimerManager::GetInstancePtr()->UpdateTimer();

	return TRUE;
}
