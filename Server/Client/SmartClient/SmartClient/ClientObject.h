#ifndef __CLIENT_OBJECT_H__
#define __CLIENT_OBJECT_H__
#include "behaviac/behaviac.h"
#include "../../Connector/ClientConnector.h"

#define ST_NONE				0
#define ST_Register			1
#define ST_RegisterOK		2
#define ST_AccountLogin		3
#define ST_AccountLoginOK	4
#define ST_SelectSvr		5
#define ST_SelectSvrOK		6
#define ST_RoleList			7
#define ST_RoleListOK		8
#define ST_RoleCreate		9
#define ST_RoleCreateOK		10
#define ST_RoleLogin		11
#define ST_RoleLoginOK		12
#define ST_EnterScene		13
#define ST_EnterSceneOK		14
#define ST_EnterCopy		15
#define ST_EnterCopyOK		16
#define ST_AbortCopy		17
#define ST_AbortCopyOK		18

#define ST_Disconnected		19

#define ST_Overed			20

struct CSceneObject
{

};

class CClientObject : public behaviac::Agent, public IMessageHandler
{
public:
	CClientObject(void);

	~CClientObject(void);

	BEHAVIAC_DECLARE_AGENTTYPE(CClientObject, behaviac::Agent);

	BOOL    DispatchPacket(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);

	BOOL    OnUpdate(UINT32 dwTick);

	BOOL    SendNewAccountReq(std::string szAccountName, std::string szPassword);
	BOOL    SendAccountLoginReq(std::string szAccountName, std::string szPassword);
	BOOL    SendSelectSvrReq(UINT32 dwSvrID);
	BOOL    SendCreateRoleReq(UINT64 dwAccountID, std::string strName, UINT32 dwCarrerID);
	BOOL    SendDelCharReq(UINT64 dwAccountID, UINT64 u64RoleID);

	BOOL    SendRoleLogoutReq(UINT64 u64CharID);
	BOOL    SendRoleLoginReq(UINT64 u64CharID);
	BOOL    SendRoleListReq();
	BOOL    SendMainCopyReq();
	BOOL    SendAbortCopyReq();

	void    SayHello(behaviac::string strContent);
	//*********************消息处理定义开始******************************
public:
	BOOL    OnCmdNewAccountAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgAccountLoginAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgSelectServerAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgRoleListAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgCreateRoleAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnCmdEnterSceneAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgNotifyIntoScene(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgOtherLoginNty(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgRoleLoginAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgObjectNewNty(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL OnMsgObjectChangeNty(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);
	BOOL    OnMsgObjectRemoveNty(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen);


	//*********************消息处理定义结束******************************


public:


	std::vector<UINT64>	m_RoleIDList;

	std::map<UINT64, CSceneObject> m_mapSceneObject;

	CClientConnector	m_ClientConnector;

	BOOL				m_bLoginOK;

	UINT64				m_dwAccountID;
	UINT32				m_dwHostState;

	std::string         m_strAccountName;
	std::string			m_strPassword;
	std::string			m_strRoleName;
	UINT32				m_dwCarrerID;

	FLOAT               m_x;
	FLOAT				m_y;
	FLOAT				m_z;
	FLOAT				m_ft;

	UINT32				m_dwCopySvrID;
	UINT32				m_dwCopyGuid;
	UINT32				m_dwCopyID;
	UINT32				m_dwToCopyID;
	UINT64              m_uMoveTime;

public:
	VOID  TestMove();
	VOID  TestCopy();
	VOID  TestExitCopy();

	BOOL MoveForward(FLOAT fDistance);
};


#endif //__CLIENT_OBJECT_H__