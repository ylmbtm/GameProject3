#include "stdafx.h"
#include "SpinLock.h"
#include "ServiceBase.h"
#include "NetManager.h"
#include "CommandDef.h"
#include "Connection.h"
#include "PacketHeader.h"
#include "../ServerEngine/Log.h"
#include "../ServerEngine/TimerManager.h"
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


BOOL ServiceBase::OnDataHandle(IDataBuffer* pDataBuffer, CConnection* pConnection)
{
	PacketHeader* pHeader = (PacketHeader*)pDataBuffer->GetBuffer();

	m_SpinLock.Lock();
	m_pRecvDataQueue->emplace_back(NetPacket(pConnection->GetConnectionID(), pDataBuffer, pHeader->dwMsgID));
	m_SpinLock.Unlock();
	return TRUE;
}

BOOL ServiceBase::StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher* pDispather, std::string strListenIp)
{
	ERROR_RETURN_FALSE (pDispather != NULL)

	ERROR_RETURN_FALSE((nPortNum != 0) && (nMaxConn != 0))

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
	return TRUE;
}

BOOL ServiceBase::StopNetwork()
{
	CLog::GetInstancePtr()->LogError("==========服务器开始关闭=======================");

	CNetManager::GetInstancePtr()->Close();

	CLog::GetInstancePtr()->CloseLog();

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

	return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, &Data, sizeof(T));
}

BOOL ServiceBase::SendMsgProtoBuf(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
{
	if (dwConnID <= 0)
	{
		return FALSE;
	}

	char szBuff[102400] = {0};

	ERROR_RETURN_FALSE(pdata.ByteSize() < 102400);

	pdata.SerializePartialToArray(szBuff, pdata.GetCachedSize());

	m_dwSendNum++;

	return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, szBuff, pdata.GetCachedSize());
}

BOOL ServiceBase::SendMsgRawData(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pdata, UINT32 dwLen)
{
	if (dwConnID <= 0)
	{
		return FALSE;
	}

	m_dwSendNum++;

	return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, pdata, dwLen);
}

BOOL ServiceBase::SendMsgBuffer(UINT32 dwConnID, IDataBuffer* pDataBuffer)
{
	m_dwSendNum++;
	return CNetManager::GetInstancePtr()->SendMsgBufByConnID(dwConnID, pDataBuffer);
}

CConnection* ServiceBase::ConnectTo( std::string strIpAddr, UINT16 sPort )
{
	ERROR_RETURN_NULL(!strIpAddr.empty() && sPort > 0)

	return CNetManager::GetInstancePtr()->ConnectTo_Async(strIpAddr, sPort);
}

BOOL ServiceBase::OnCloseConnect( CConnection* pConnection )
{
	ERROR_RETURN_FALSE(pConnection->GetConnectionID() != 0);

	m_SpinLock.Lock();
	m_pRecvDataQueue->emplace_back(NetPacket(pConnection->GetConnectionID(), (IDataBuffer*)pConnection, CLOSE_CONNECTION));
	m_SpinLock.Unlock();
	return TRUE;
}

BOOL ServiceBase::OnNewConnect( CConnection* pConnection )
{
	ERROR_RETURN_FALSE(pConnection->GetConnectionID() != 0);
	m_SpinLock.Lock();
	m_pRecvDataQueue->emplace_back(NetPacket(pConnection->GetConnectionID(), (IDataBuffer*)pConnection, NEW_CONNECTION));
	m_SpinLock.Unlock();
	return TRUE;
}


CConnection* ServiceBase::GetConnectionByID( UINT32 dwConnID )
{
	return CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(dwConnID);
}

BOOL ServiceBase::Update()
{
	if(m_dwLastTick == 0)
	{
		m_dwLastTick = CommonFunc::GetTickCount();
	}

	CConnectionMgr::GetInstancePtr()->CheckConntionAvalible();

	m_SpinLock.Lock();
	std::swap(m_pRecvDataQueue, m_pDispathQueue);
	m_SpinLock.Unlock();

	for (std::deque<NetPacket>::iterator itor = m_pDispathQueue->begin(); itor != m_pDispathQueue->end(); ++itor)
	{
		NetPacket& item = *itor;
		if (item.m_dwMsgID == NEW_CONNECTION)
		{
			m_pPacketDispatcher->OnNewConnect((CConnection*)item.m_pDataBuffer);
		}
		else if (item.m_dwMsgID == CLOSE_CONNECTION)
		{
			m_pPacketDispatcher->OnCloseConnect((CConnection*)item.m_pDataBuffer);
			//发送通知
			CConnectionMgr::GetInstancePtr()->DeleteConnection((CConnection*)item.m_pDataBuffer);
		}
		else
		{
			m_pPacketDispatcher->DispatchPacket(&item);

			item.m_pDataBuffer->Release();

			m_dwRecvNum += 1;
		}
	}

	m_pDispathQueue->clear();

	m_dwFps += 1;

	if((CommonFunc::GetTickCount() - m_dwLastTick) > 1000)
	{
		m_pPacketDispatcher->OnSecondTimer();

		CLog::GetInstancePtr()->SetTitle("[FPS:%d]-[RecvNum:%d]--[SendNum:%d]", m_dwFps, m_dwRecvNum, m_dwSendNum);
		m_dwFps = 0;
		m_dwRecvNum = 0;
		m_dwSendNum = 0;
		m_dwLastTick = CommonFunc::GetTickCount();
	}

	TimerManager::GetInstancePtr()->UpdateTimer();

	CLog::GetInstancePtr()->Flush();

	return TRUE;
}
