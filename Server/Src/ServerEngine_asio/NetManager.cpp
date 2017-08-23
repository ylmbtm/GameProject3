#include "stdafx.h"
#include "NetManager.h"
#include "Connection.h"
#include "CommandDef.h"
#include "boost/asio/placeholders.hpp"
#include <boost/asio/impl/connect.hpp>
#include "../ServerEngine/CommonConvert.h"


CNetManager::CNetManager(void)
{
	m_pBufferHandler	= NULL;
}

CNetManager::~CNetManager(void)
{
}

BOOL CNetManager::Start(UINT16 nPortNum, UINT32 nMaxConn, IDataHandler* pBufferHandler )
{
	if(pBufferHandler == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	m_pBufferHandler = pBufferHandler;

	CConnectionMgr::GetInstancePtr()->InitConnectionList(nMaxConn, m_IoService);

	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::from_string("0.0.0.0"), nPortNum);

	m_pAcceptor = new boost::asio::ip::tcp::acceptor(m_IoService, ep);

	WaitForConnect();

	m_pWorkThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &m_IoService));

	return TRUE;
}

BOOL CNetManager::Close()
{
	m_IoService.stop();

	CConnectionMgr::GetInstancePtr()->CloseAllConnection();

	CConnectionMgr::GetInstancePtr()->DestroyAllConnection();

	return TRUE;
}


BOOL CNetManager::WaitForConnect()
{
	CConnection* pConnection = CConnectionMgr::GetInstancePtr()->CreateConnection();
	if(pConnection == NULL)
	{
		return FALSE;
	}

	m_pAcceptor->async_accept(pConnection->GetSocket(), boost::bind(&CNetManager::HandleAccept, this,  pConnection, boost::asio::placeholders::error));

	return TRUE;
}

CConnection* CNetManager::ConnectToOtherSvr( std::string strIpAddr, UINT16 sPort )
{
	//boost::asio::ip::tcp::resolver resolver(m_IoService);
	//boost::asio::ip::tcp::resolver::query query(serverip.c_str(), Helper::IntToString(portnumber));
	//boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	//boost::asio::ip::tcp::resolver::iterator end;
	//boost::system::error_code error = boost::asio::error::host_not_found;

	CConnection* pConnection = CConnectionMgr::GetInstancePtr()->CreateConnection();
	if(pConnection == NULL)
	{
		return NULL;
	}

	//boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(strIpAddr), sPort);

	boost::asio::ip::tcp::resolver resolver(m_IoService);
	boost::asio::ip::tcp::resolver::query query(strIpAddr, CommonConvert::IntToString(sPort));
	boost::asio::ip::tcp::resolver::iterator enditor = resolver.resolve(query);

	boost::asio::async_connect(pConnection->GetSocket(), enditor, boost::bind(&CNetManager::HandleConnect, this, pConnection, boost::asio::placeholders::error));

	return pConnection;
}


void CNetManager::HandleConnect(CConnection* pConnection, const boost::system::error_code& e)
{
	m_pBufferHandler->OnNewConnect(pConnection);

	pConnection->DoReceive();

	return ;
}

void CNetManager::HandleAccept(CConnection* pConnection, const boost::system::error_code& e)
{
	m_pBufferHandler->OnNewConnect(pConnection);

	pConnection->DoReceive();

	WaitForConnect();

	return ;
}

BOOL	CNetManager::SendMsgBufByConnID(UINT32 dwConID, IDataBuffer* pDataBuffer)
{
	if((pDataBuffer == NULL) || (dwConID == 0))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	CConnection* pConn = CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(dwConID);

	return pConn->SendBuffer(pDataBuffer);
}
