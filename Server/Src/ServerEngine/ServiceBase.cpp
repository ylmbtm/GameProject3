#include "stdafx.h"
#include "ServiceBase.h"
#include "NetManager.h"
#include "CommonSocket.h"
#include "CommandDef.h"
#include "CommonEvent.h"
#include "CommonConvert.h"
#include "DataBuffer.h"
#include "Connection.h"
#include "CommonFunc.h"
#include "TimerManager.h"
#include "PacketHeader.h"
#include "Log.h"

ServiceBase::ServiceBase(void)
{
	m_pPacketDispatcher = NULL;
	m_dwReadIndex = 0;
	m_dwWriteIndex = 1;
}

ServiceBase::~ServiceBase(void)
{
}

ServiceBase* ServiceBase::GetInstancePtr()
{
	static ServiceBase _ServiceBase;

	return &_ServiceBase;
}


BOOL ServiceBase::OnDataHandle(IDataBuffer* pDataBuffer, CConnection* pConnection)
{
	PacketHeader* pHeader = (PacketHeader*)pDataBuffer->GetBuffer();
	if(!m_DataQueue[m_dwWriteIndex % 2].push(NetPacket(pConnection->GetConnectionID(), pDataBuffer, pHeader->dwMsgID)))
	{
		AtomicAdd(&m_dwWriteIndex, 1);
		ERROR_RETURN_FALSE(m_DataQueue[m_dwWriteIndex % 2].push(NetPacket(pConnection->GetConnectionID(), pDataBuffer, pHeader->dwMsgID)));
	}
	return TRUE;
}

BOOL ServiceBase::StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher* pDispather)
{
	if (pDispather == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if((nPortNum <= 0) || (nMaxConn <= 0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_pPacketDispatcher = pDispather;

	if (!CNetManager::GetInstancePtr()->Start(nPortNum, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动网络层失败!");
		return FALSE;
	}

	m_dwLastTick = 0;
	m_dwPackNum = 0;
	m_dwFps = 0;

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
	ERROR_RETURN_FALSE(dwConnID != 0);

	return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, &Data, sizeof(T));
}

BOOL ServiceBase::SendMsgProtoBuf(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
{
	ERROR_RETURN_FALSE(dwConnID != 0);

	char szBuff[10240] = {0};

	ERROR_RETURN_FALSE(pdata.ByteSize() < 10240);

	pdata.SerializePartialToArray(szBuff, pdata.GetCachedSize());

	return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, szBuff, pdata.GetCachedSize());
}

BOOL ServiceBase::SendMsgRawData(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pdata, UINT32 dwLen)
{
	ERROR_RETURN_FALSE(dwConnID != 0);

	return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, pdata, dwLen);
}

BOOL ServiceBase::SendMsgBuffer(UINT32 dwConnID, IDataBuffer* pDataBuffer)
{
	return CNetManager::GetInstancePtr()->SendMsgBufByConnID(dwConnID, pDataBuffer);
}

CConnection* ServiceBase::ConnectToOtherSvr( std::string strIpAddr, UINT16 sPort )
{
	if(strIpAddr.empty() || sPort <= 0)
	{
		ASSERT_FAIELD;
		return NULL;
	}

	return CNetManager::GetInstancePtr()->ConnectToOtherSvrEx(strIpAddr, sPort);
}

BOOL ServiceBase::OnCloseConnect( CConnection* pConnection )
{
	ERROR_RETURN_FALSE(pConnection->GetConnectionID() != 0);
	m_CloseConList.push(pConnection);

	if(m_dwReadIndex == (m_dwWriteIndex % 2))
	{
		AtomicAdd(&m_dwWriteIndex, 1);
	}

	return TRUE;
}

BOOL ServiceBase::OnNewConnect( CConnection* pConnection )
{
	ERROR_RETURN_FALSE(pConnection->GetConnectionID() != 0);

	m_NewConList.push(pConnection);

	if(m_dwReadIndex == (m_dwWriteIndex % 2))
	{
		AtomicAdd(&m_dwWriteIndex, 1);
	}

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

	//处理新连接的通知
	CConnection* pConnection = NULL;
	while(m_NewConList.pop(pConnection))
	{
		m_pPacketDispatcher->OnNewConnect(pConnection);
	}

	NetPacket item;
	while(m_DataQueue[m_dwReadIndex].pop(item))
	{
		m_pPacketDispatcher->DispatchPacket(&item);

		item.m_pDataBuffer->Release();

		m_dwPackNum += 1;
	}

	m_dwFps += 1;

	if((CommonFunc::GetTickCount() - m_dwLastTick) > 1000)
	{
		m_dwFps = 0;
		m_dwPackNum = 0;
		m_dwLastTick = CommonFunc::GetTickCount();
	}

	//处理断开的连接
	while(m_CloseConList.pop(pConnection))
	{
		//发送通知
		m_pPacketDispatcher->OnCloseConnect(pConnection);
		//发送通知
		CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
	}

	TimerManager::GetInstancePtr()->UpdateTimer();

	CLog::GetInstancePtr()->Flush();

	m_dwReadIndex = (m_dwReadIndex + 1) % 2;

	return TRUE;
}
