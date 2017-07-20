#include "stdafx.h"
#include "ServiceBase.h"
#include "NetManager.h"
#include "Utility/CommonSocket.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonEvent.h"
#include "Utility/CommonConvert.h"
#include "DataBuffer.h"
#include "Connection.h"
#include "Utility/CommonFunc.h"
#include "TimerManager.h"
#include "PacketHeader.h"

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


BOOL ServiceBase::OnDataHandle(IDataBuffer *pDataBuffer , CConnection *pConnection)
{
	PacketHeader *pHeader = (PacketHeader *)pDataBuffer->GetBuffer();
	ERROR_RETURN_FALSE(m_DataQueue[m_dwWriteIndex%2].push(NetPacket(pConnection->GetConnectionID(), pDataBuffer,pHeader->dwMsgID)));
	return TRUE;
}

BOOL ServiceBase::StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher *pDispather)
{
	if (pDispather == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if((nPortNum <= 0)||(nMaxConn <= 0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_pPacketDispatcher = pDispather;

	if (!CNetManager::GetInstancePtr()->Start(nPortNum, nMaxConn, this))
	{
		CLog::GetInstancePtr()->AddLog("启动网络层失败!");
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("服务器启动成功!");

	m_dwLastTick = 0;
	m_dwPackNum = 0;
	m_dwFps = 0;

	return TRUE;
}

BOOL ServiceBase::StopNetwork()
{
	CLog::GetInstancePtr()->AddLog("==========服务器开始关闭=======================");

	CNetManager::GetInstancePtr()->Close();

	CLog::GetInstancePtr()->CloseLog();

	return TRUE;
}

template<typename T>
BOOL ServiceBase::SendMsgStruct(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID,UINT32 dwUserData, T &Data)
{
	if(dwConnID == 0)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, &Data, sizeof(T));
}

 BOOL ServiceBase::SendMsgProtoBuf(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
 {
	 if(dwConnID == 0)
	 {
		 ASSERT_FAIELD;

		 return FALSE;
	 }

	 char szBuff[10240] = {0};

	 pdata.SerializePartialToArray(szBuff, pdata.ByteSize());

	 return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, szBuff, pdata.ByteSize());
 }

 BOOL ServiceBase::SendMsgRawData(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char * pdata,UINT32 dwLen)
 {
	 if(dwConnID == 0)
	 {
		 ASSERT_FAIELD;

		 return FALSE;
	 }

	 return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, pdata, dwLen);
 }

 BOOL ServiceBase::SendMsgBuffer(UINT32 dwConnID, IDataBuffer *pDataBuffer)
 {
	 return CNetManager::GetInstancePtr()->SendMsgBufByConnID(dwConnID, pDataBuffer);
 }

 CConnection* ServiceBase::ConnectToOtherSvr( std::string strIpAddr, UINT16 sPort )
{
	if(strIpAddr.empty()||sPort <= 0)
	{
		ASSERT_FAIELD;
		return NULL;
	}

	return CNetManager::GetInstancePtr()->ConnectToOtherSvrEx(strIpAddr, sPort);
}

BOOL ServiceBase::OnCloseConnect( CConnection *pConnection )
{
	if(pConnection->GetConnectionID() == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_CloseConList.push(pConnection);
	AtomicAdd(&m_dwWriteIndex, 1);
	return TRUE;
}

BOOL ServiceBase::OnNewConnect( CConnection *pConnection )
{
	if(pConnection->GetConnectionID() == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_NewConList.push(pConnection);
	AtomicAdd(&m_dwWriteIndex, 1);
	return TRUE;
}


CConnection* ServiceBase::GetConnectionByID( UINT32 dwConnID )
{
	return CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(dwConnID);
}

// BOOL ServiceBase::Update()
// {
// 	NetPacket item;
// 	//处理新连接的通知
// 	CConnection *pConnection = NULL;
// 	while(m_NewConList.pop(pConnection))
// 	{
// 		item.m_pDataBuffer = NULL;
// 		item.m_pConnect = pConnection;
// 		FireMessage(1, &item);
// 	}
// 
// 	while(m_DataQueue.pop(item))
// 	{
// 		PacketHeader *pPacketHeader = (PacketHeader *)item.m_pDataBuffer->GetBuffer();
// 
// 		FireMessage(pPacketHeader->dwMsgID, &item);
// 	}
// 
// 	//处理断开的连接
// 	while(m_CloseConList.pop(pConnection))
// 	{
// 		//发送通知
// 		item.m_pDataBuffer = NULL;
// 		item.m_pConnect = pConnection;
// 		FireMessage(2, &item);
// 		//发送通知
// 		CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
// 	}
// 
// 	return TRUE;
// }

BOOL ServiceBase::Update()
{
	if(m_dwLastTick == 0)
	{
		m_dwLastTick = CommonFunc::GetTickCount();
	}

	CConnectionMgr::GetInstancePtr()->CheckConntionAvalible();

	//处理新连接的通知
	CConnection *pConnection = NULL;
	while(m_NewConList.pop(pConnection))
	{
		m_pPacketDispatcher->OnNewConnect(pConnection);
	}

	NetPacket item;
	while(m_DataQueue[m_dwReadIndex].pop(item))
	{
		//UINT32 dwTick = GetTickCount();
		m_pPacketDispatcher->DispatchPacket(&item);

		//if((GetTickCount() - dwTick) >10)
		//{
		//	CLog::GetInstancePtr()->AddLog("messageid:%d, costtime:%d", item.m_dwMsgID, GetTickCount() - dwTick);
		//}

		item.m_pDataBuffer->Release();

		m_dwPackNum += 1;
	}

	m_dwFps += 1;

	if((CommonFunc::GetTickCount()-m_dwLastTick)>1000)
	{
		CLog::GetInstancePtr()->AddLog("fps:%d, packetnum:%d", m_dwFps , m_dwPackNum);
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

	m_dwReadIndex = m_dwReadIndex + 1;
	m_dwReadIndex = m_dwReadIndex % 2;

	return TRUE;
}
