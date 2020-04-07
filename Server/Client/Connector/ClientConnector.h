#pragma once
#ifndef __CLIENT_CONNECTOR_H__
#define __CLIENT_CONNECTOR_H__
#include "..\Src\Message\Msg_ID.pb.h"

enum ConnectState
{
	ECS_NO_CONNECT,	//无连接
	ECS_CONNECTING, //连接中
	ECS_CONNECTED	//己连接
};

#define CONST_BUFF_SIZE 1024000

struct IMessageHandler
{
	virtual BOOL DispatchPacket(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen) = 0;
};

class CClientConnector
{
public:
	CClientConnector();

	~CClientConnector();

public:

	BOOL	ConnectTo(std::string strIpAddr, UINT16 sPort);

	BOOL	DisConnect();

	BOOL	SendData(UINT32 dwMsgID, const google::protobuf::Message& pdata, UINT64 u64TargetID, UINT32 dwUserData);

	BOOL	RegisterMsgHandler(IMessageHandler* pMsgHandler);

	BOOL	UnregisterMsgHandler(IMessageHandler* pMsgHandler);

	BOOL	Render();

	UINT32  GetServerTime();

	BOOL	IsConnected();

	ConnectState GetConnectState(VOID);
protected:

	VOID	SetConnectState(ConnectState val);

	BOOL	ReceiveData();

	BOOL	ProcessData();

	BOOL	SendData(char* pData, INT32 dwLen);

	//以下是内部的消息处理
	BOOL	DispatchPacket(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);


protected:
	ConnectState		m_ConnectState;

	SOCKET				m_hSocket;

	INT32				m_nDataLen;
	CHAR				m_DataBuffer[CONST_BUFF_SIZE];

	INT32               m_PacketLen;
	CHAR				m_PackBuffer[CONST_BUFF_SIZE];

	std::vector<IMessageHandler*> m_vtMsgHandler;

	UINT32				m_dwServerTime;
	UINT32				m_dwServerTick;
};



#endif
