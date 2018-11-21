#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_
#include "IBufferHandler.h"
#include "boost/system/error_code.hpp"
#include "boost/asio/ip/tcp.hpp"
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>


////////////////////////////////////////////////
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
	BOOL	Start(UINT16 nPortNum,  UINT32 nMaxConn, IDataHandler* pBufferHandler, std::string &strListenIp);

	BOOL	Close();

	BOOL	SendMessageByConnID(UINT32 dwConnID,  UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData,  const char* pData, UINT32 dwLen);

	BOOL    SendMsgBufByConnID(UINT32 dwConnID, IDataBuffer* pBuffer);
public:
	BOOL	WaitForConnect();
public:
	CConnection*	ConnectToOtherSvr(std::string strIpAddr, UINT16 sPort);

	void HandleConnect(CConnection* pConnection, const boost::system::error_code& e);
	void HandleAccept(CConnection* pConnection, const boost::system::error_code& e);

	boost::asio::ip::tcp::acceptor* m_pAcceptor;
	boost::asio::io_service         m_IoService;
	boost::thread*                   m_pWorkThread;
	IDataHandler*					m_pBufferHandler;
};

#endif

