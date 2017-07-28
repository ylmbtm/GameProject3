#ifndef __CLIENT_CMD_HANDLER_H__
#define __CLIENT_CMD_HANDLER_H__

#include "IBufferHandler.h"
#include "CommandDef.h"
#include "PlayerObject.h"
#include "ClientConnector\ClientConnector.h"

#define ST_NONE			0
#define ST_Register		1
#define ST_RegisterOK   2
#define ST_Login		3
#define ST_LoginOK		4
#define ST_Entering		5
#define ST_EnterOK		6
#define ST_SelectSvr	5
#define ST_SelectSvrOK	6
#define ST_RoleList		7
#define ST_RoleListOk	8
#define ST_EnterScene	9
#define ST_EnterSceneOK	10
#define ST_Disconnected 11


class CClientCmdHandler : public IMessageHandler
{
public:
	CClientCmdHandler(void);

	~CClientCmdHandler(void);


	BOOL DispatchPacket(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);

	BOOL OnUpdate(UINT32 dwTick);


	BOOL SendNewAccountReq(std::string szAccountName, std::string szPassword);
	BOOL SendAccountLoginReq(std::string szAccountName, std::string szPassword);
	BOOL SendSelectSvrReq(UINT32 dwSvrID);
	BOOL SendCreateRoleReq(UINT64 dwAccountID,std::string strName, UINT32 dwActorID);
	BOOL SendDelCharReq(UINT64 dwAccountID,UINT64 dwCharID);
	BOOL SendRoleLogoutReq(UINT64 u64CharID);
	BOOL SendRoleLoginReq(UINT64 u64CharID);
	BOOL SendRoleListReq();
	BOOL SendMainCopyReq();
	BOOL SendAbortCopyReq();


	//*********************消息处理定义开始******************************
public:
	BOOL OnCmdNewAccountAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgAccountLoginAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgSelectServerAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgRoleListAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgCreateRoleAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnCmdEnterSceneAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgNotifyIntoScene(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	
	BOOL OnMsgObjectNewNty(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgObjectActionNty(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgObjectRemoveNty(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);

	
	//*********************消息处理定义结束******************************

	std::vector<UINT64>	m_RoleIDList;

	CPlayerObject		m_HostPlayer;

	CPlayerObjectMgr	m_PlayerObjMgr;

	CClientConnector	m_ClientConnector;

	BOOL				m_bLoginOK;

	UINT64				m_dwAccountID;
	UINT32				m_dwHostState;

	std::string         m_strAccountName;
	std::string			m_strPassword;
	std::string			m_strRoleName;

	FLOAT               m_x;
	FLOAT				m_z;
	FLOAT				m_vx;
	FLOAT				m_vz;

	UINT32				m_dwCopySvrID;
	UINT32				m_dwCopyGuid;
	UINT32				m_dwCopyID;

public:
	VOID  TestMove();
};


#endif