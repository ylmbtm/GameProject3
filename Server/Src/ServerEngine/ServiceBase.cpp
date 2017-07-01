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
	m_DataQueue.push(NetPacket(pConnection, pDataBuffer,pHeader->dwMsgID));
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
	CConnectionMgr::GetInstancePtr()->CheckConntionAvalible();

	NetPacket item;
	//处理新连接的通知
	CConnection *pConnection = NULL;
	while(m_NewConList.pop(pConnection))
	{
		item.m_pDataBuffer = NULL;
		item.m_pConnect = pConnection;
		m_pPacketDispatcher->OnNewConnect(pConnection);
	}

	while(m_DataQueue.pop(item))
	{
		UINT32 dwTick = GetTickCount();
		m_pPacketDispatcher->DispatchPacket(&item);

		if((GetTickCount() - dwTick) >10)
		{
			CLog::GetInstancePtr()->AddLog("messageid:%d, costtime:%d", item.m_dwMsgID, GetTickCount() - dwTick);
		}
	}

	//处理断开的连接
	while(m_CloseConList.pop(pConnection))
	{
		//发送通知
		item.m_pDataBuffer = NULL;
		item.m_pConnect = pConnection;
		m_pPacketDispatcher->OnCloseConnect(pConnection);
		//发送通知
		CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
	}

	TimerManager::GetInstancePtr()->UpdateTimer();

	return TRUE;
}
