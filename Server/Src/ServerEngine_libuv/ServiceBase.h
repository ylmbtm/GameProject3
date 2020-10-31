#ifndef __SERVICE_BASE_H__
#define __SERVICE_BASE_H__

#include "IBufferHandler.h"
#include "Connection.h"
#include "google/protobuf/message.h"
#include "ConfigFile.h"

class ServiceBase : public IDataHandler//, public CEventFuncManager
{
protected:
	ServiceBase(void);
	virtual ~ServiceBase(void);
public:
	static ServiceBase* GetInstancePtr();

	BOOL            StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher* pDispather, std::string strListenIp = "");

	BOOL            StopNetwork();

	BOOL			OnDataHandle(IDataBuffer* pDataBuffer, UINT32 nConnID);

	BOOL			OnCloseConnect(UINT32 nConnID);

	BOOL			OnNewConnect(UINT32 nConnID);

	CConnection*	ConnectTo(std::string strIpAddr, UINT16 sPort);

	BOOL            CloseConnect(UINT32 nConnID);

	template<typename T>
	BOOL			SendMsgStruct(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, T& Data);

	BOOL			SendMsgProtoBuf(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata);

	BOOL			SendMsgRawData(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pdata, UINT32 dwLen);

	BOOL			SendMsgBuffer(UINT32 dwConnID, IDataBuffer* pDataBuffer);

	CConnection*	GetConnectionByID(UINT32 dwConnID);

	BOOL			Update();

protected:
	IPacketDispatcher*					m_pPacketDispatcher;

	std::deque<NetPacket>*				m_pRecvDataQueue;
	std::deque<NetPacket>*				m_pDispathQueue;
	CSpinLock							m_QueueLock;

	//以下用于统计
	UINT64								m_dwLastTick;
	UINT32								m_dwRecvNum;
	UINT32								m_dwSendNum;
	UINT32								m_dwFps;
	UINT32								m_dwLastMsgID;
};


#endif /*__SERVICE_BASE_H__*/