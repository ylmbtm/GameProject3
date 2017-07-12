#pragma once
#ifndef __CLIENT_CONNECTOR_H__
#define __CLIENT_CONNECTOR_H__
#include "..\Src\Message\Msg_ID.pb.h"

enum ConnectState
{
	Not_Connect,	//无连接
	Start_Connect,
	Succ_Connect
};


#define CONST_BUFF_SIZE 8192

struct IMessageHandler
{
	virtual BOOL DispatchPacket(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen) = 0;
};

class CClientConnector : public IMessageHandler
{
public:
	CClientConnector();

	~CClientConnector();

public:
	BOOL	InitConnector();

	BOOL	CloseConnector();

	BOOL	SetClientID(UINT64 u64ClientID);  //一般是角色ID

	BOOL	SetLoginServerAddr(std::string strIpAddr, UINT16 sPort);

	BOOL	ConnectToServer(std::string strIpAddr, UINT16 sPort);

	BOOL	DisConnect();

	BOOL	SendData(char *pData, INT32 dwLen);

	BOOL	SendData(UINT32 dwMsgID, const google::protobuf::Message& pdata, UINT64 u64TargetID, UINT32 dwUserData);
	
	BOOL	ConnectLoginSvr(BOOL bConnect = FALSE);

	BOOL	RegisterMsgHandler(IMessageHandler *pMsgHandler);

	BOOL	UnregisterMsgHandler(IMessageHandler *pMsgHandler);

	BOOL	Render();

	UINT32  GetServerTime();
	
	ConnectState GetConnectState(VOID);

protected:
	VOID	SetConnectState(ConnectState val);

	BOOL	ReceiveData();

	BOOL	ProcessData();

	//以下是内部的消息处理
	BOOL	DispatchPacket(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);

	BOOL	OnUpdate(UINT32 dwTick){return TRUE;};
protected:
	UINT64				m_u64ClientID;
	UINT32				m_dwIdentifyCode;

	ConnectState		m_ConnectState;

	SOCKET				m_hSocket;

	INT32				m_nDataLen;
	CHAR				m_DataBuffer[CONST_BUFF_SIZE];

	INT32               m_PacketLen;
	CHAR				m_PackBuffer[CONST_BUFF_SIZE];
	
	std::vector<IMessageHandler*> m_vtMsgHandler;


	UINT32				m_dwServerTime;
	UINT32				m_dwServerTick;

	std::string			m_strLoginIp;
	UINT16				m_sLoginPort;

};



#endif
