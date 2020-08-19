#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_
#include "IBufferHandler.h"
#include "Connection.h"

class CNetManager
{
	CNetManager(void);

	virtual ~CNetManager(void);
public:
	static CNetManager* GetInstancePtr()
	{
		static CNetManager NetManager;

		return &NetManager;
	}
public:
	BOOL	Start(UINT16 nPortNum,  UINT32 nMaxConn, IDataHandler* pBufferHandler, std::string& strListenIp);

	BOOL	Stop();

	BOOL    SendMessageData(UINT32 dwConnID,  UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData,  const char* pData, UINT32 dwLen);

	BOOL    SendMessageBuff(UINT32 dwConnID, IDataBuffer* pBuffer);

	CConnection*	ConnectTo_Sync(std::string strIpAddr, UINT16 sPort);

	CConnection*	ConnectTo_Async(std::string strIpAddr, UINT16 sPort);

	void HandleConnect(CConnection* pConnection, INT32 dwStatus);

	void HandleAccept(CConnection* pConnection, INT32 dwStatus);

	BOOL PostSendOperation(CConnection* pConnection);

	void RunLoop();

	uv_tcp_t						m_ListenSocket;
	uv_loop_t*						m_pMainLoop;
	uv_thread_t						m_LoopThreadID;
	IDataHandler*					m_pBufferHandler;

public:
};

#endif

