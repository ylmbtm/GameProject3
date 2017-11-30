#ifndef __SERVICE_BASE_H__
#define __SERVICE_BASE_H__

#include "IBufferHandler.h"
#include "Connection.h"
#include "EventFuncManager.h"
#include "LockFreeQueue.h"
#include "ConfigFile.h"
#include "google/protobuf/message.h"



class ServiceBase : public IDataHandler//, public CEventFuncManager
{
protected:
	ServiceBase(void);
	virtual ~ServiceBase(void);
public:
	static ServiceBase* GetInstancePtr();

	BOOL            StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher* pDispather);

	BOOL            StopNetwork();

	BOOL			OnDataHandle(IDataBuffer* pDataBuffer, CConnection* pConnection);

	BOOL			OnCloseConnect(CConnection* pConnection);

	BOOL			OnNewConnect(CConnection* pConnection);

	CConnection*	ConnectToOtherSvr(std::string strIpAddr, UINT16 sPort);

	template<typename T>
	BOOL			SendMsgStruct(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, T& Data);

	BOOL			SendMsgProtoBuf(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata);

	BOOL			SendMsgRawData(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pdata, UINT32 dwLen);

	BOOL			SendMsgBuffer(UINT32 dwConnID, IDataBuffer* pDataBuffer);

	CConnection*	GetConnectionByID(UINT32 dwConnID);

	BOOL			Update();

protected:
	IPacketDispatcher*					m_pPacketDispatcher;
	ArrayLockFreeQueue<NetPacket>		m_DataQueue[2];
	UINT32								m_dwReadIndex;
	UINT32								m_dwWriteIndex;
	ArrayLockFreeQueue<CConnection*>	m_NewConList;
	ArrayLockFreeQueue<CConnection*>	m_CloseConList;


	//以下用于统计
	UINT64								m_dwLastTick;
	UINT32								m_dwRecvNum;
	UINT32								m_dwSendNum;
	UINT32								m_dwFps;
};


#endif /*__SERVICE_BASE_H__*/