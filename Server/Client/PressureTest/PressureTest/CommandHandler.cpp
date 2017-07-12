#include "stdafx.h"
#include "CommandHandler.h"
#include <complex>
#include "..\Src\Message\Msg_Login.pb.h"
#include "..\Src\Message\Msg_RetCode.pb.h"
#include "PacketHeader.h"
#include "..\Src\Message\Msg_Move.pb.h"
#include "Utility\CommonConvert.h"

int g_LoginReqCount = 0;
int g_LoginCount = 0;
int g_EnterCount = 0;

CClientCmdHandler::CClientCmdHandler(void)
{
	m_bLoginOK = FALSE;

	m_dwAccountID = 0;
	m_dwHostState = ST_NONE;

	m_ClientConnector.RegisterMsgHandler((IMessageHandler*)this);
}

CClientCmdHandler::~CClientCmdHandler(void)
{
}

BOOL CClientCmdHandler::DispatchPacket(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{ 
	BOOL bHandled = TRUE;
	switch(dwMsgID)
	{
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_ACCOUNT_REG_ACK,		OnCmdNewAccountAck);
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_ACCOUNT_LOGIN_ACK,		OnMsgAccountLoginAck);
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_SELECT_SERVER_ACK,      OnMsgSelectServerAck);
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_ROLE_LIST_ACK,			OnMsgRoleListAck);
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_NOTIFY_INTO_SCENE,		OnMsgNotifyIntoScene);
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_ROLE_CREATE_ACK,		OnMsgCreateRoleAck);
		
		//PROCESS_MESSAGE_ITEM(CMD_CHAR_ENTER_GAME_ACK,	        OnCmdEnterGameAck)

		//PROCESS_MESSAGE_ITEM(CMD_CHAR_NEARBY_ADD,		OnCmdNearByAdd);
		//PROCESS_MESSAGE_ITEM(CMD_CHAR_NEARBY_UPDATE,	OnCmdNearByUpdate);
		//PROCESS_MESSAGE_ITEM(CMD_CHAR_NEARBY_REMOVE,	OnCmdNearByRemove);
		//PROCESS_MESSAGE_ITEM(CMD_CHAR_UPDATE_MYSELF,	OnCmdUpdateMyself);

	default:
		{
			bHandled = FALSE;
		}
		break;
	}

	return bHandled;
}

BOOL CClientCmdHandler::OnCmdNearByAdd( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
{
// 	UINT32 dwCount = 0;
// 	pBufferHelper->Read(dwCount);
// 
// 	printf("BEGIN---添加角色消息，添加人数:%d\n", dwCount);
// 	
// 	for(UINT32 i = 0; i < dwCount; i++)
// 	{
// 		CString strText;
// 
// 		UINT8 type = 0;
// 
// 		pBufferHelper->Read(type);
// 
// 		if(type != 1)
// 		{
// 			continue;
// 		}
// 
// 		CPlayerObject *pObject = new CPlayerObject;
// 
// 		pObject->ReadFromBuffer(pBufferHelper);
// 
// 		m_PlayerObjMgr.insert(std::make_pair(pObject->GetObjectID(), pObject));
// 
// 		printf("添加角色:%d, 坐标x = %f, z = %f\n", (UINT32)pObject->GetObjectID(), pObject->m_ObjectPos.x, pObject->m_ObjectPos.z);
// 
// 	}
// 
// 	printf("END---添加角色消息\n");
// 
// 
// 	((CTestClientDlg*)AfxGetMainWnd())->m_SceneView.Invalidate();

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdNearByUpdate( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
{
// 	UINT32 dwCount = 0;
// 	pBufferHelper->Read(dwCount);
// 
// 	for(UINT32 i = 0; i < dwCount; i++)
// 	{
// 		UINT64 u64CharID = 0;
// 
// 		pBufferHelper->Read(u64CharID);
// 
// 		CPlayerObject *pObject = m_PlayerObjMgr.GetPlayer(u64CharID);
// 		if(pObject != NULL)
// 		{
// 			pObject->ReadFromBuffer(pBufferHelper);
// 
// 			printf("更新角色:%d, 坐标x = %f, z = %f\n", (UINT32)pObject->GetObjectID(), pObject->m_ObjectPos.x, pObject->m_ObjectPos.z);
// 		}
// 		else
// 		{
// 			ASSERT_FAIELD;
// 		}
// 	}
// 
// 	((CTestClientDlg*)AfxGetMainWnd())->m_SceneView.Invalidate();

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdNearByRemove( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
{
// 	UINT32 dwCount = 0;
// 	pBufferHelper->Read(dwCount);
// 
// 	printf("BEGIN---删除角色消息，预计删除人数:%d, 现在人数:%d", dwCount, m_PlayerObjMgr.size());
// 
// 	for(UINT32 i = 0; i < dwCount; i++)
// 	{
// 		UINT64 u64CharID = 0;
// 
// 		pBufferHelper->Read(u64CharID);
// 
// 		CPlayerObject *pObj = m_PlayerObjMgr.GetPlayer(u64CharID);
// 		if(pObj == NULL)
// 		{
// 			ASSERT_FAIELD;
// 			return 0;
// 		}
// 
// 		m_PlayerObjMgr.RemovePlayer(u64CharID);
// 
// 		delete pObj;
// 	}
// 
// 	printf("END---删除角色消息\n");
// 
// 
// 	((CTestClientDlg*)AfxGetMainWnd())->m_SceneView.Invalidate();

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdEnterSceneAck( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
{
	EnterSceneAck Ack;


	
	m_dwHostState = ST_EnterSceneOK;
	
	return TRUE;
}

BOOL CClientCmdHandler::OnMsgNotifyIntoScene(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	NotifyIntoScene Nty;
	Nty.ParsePartialFromArray(PacketBuf, BufLen);
	EnterSceneReq Req;
	Req.set_roleid(Nty.roleid());
	Req.set_serverid(Nty.serverid());
	Req.set_copyid(Nty.copyid());

	m_dwCopySvrID = Nty.serverid();
	m_dwCopyType = Nty.copytype();
	m_dwCopyID = Nty.copyid();
	m_ClientConnector.SendData(MSG_ENTER_SCENE_REQ, Req, Nty.serverid(), Nty.copyid());
	return TRUE;
}

BOOL CClientCmdHandler::OnUpdate( UINT32 dwTick )
{
	m_ClientConnector.Render();

	if(m_dwHostState == ST_NONE)
	{
		if(m_ClientConnector.GetConnectState() == Not_Connect)
		{
			m_ClientConnector.SetClientID(0);

			m_ClientConnector.ConnectToServer("127.0.0.1", 5678);
		}

		if(m_ClientConnector.GetConnectState() == Succ_Connect)
		{
			SendNewAccountReq(m_strAccountName, m_strPassword);

			m_dwHostState = ST_Register;
		}
	}

	if(m_dwHostState == ST_RegisterOK)
	{
		SendAccountLoginReq(m_strAccountName, m_strPassword);

		m_dwHostState = ST_Login;
	}

	if(m_dwHostState == ST_LoginOK)
	{
		SendSelectSvrReq(0);
		
		m_dwHostState = ST_SelectSvr;
	}

	if(m_dwHostState == ST_SelectSvrOK)
	{
		SendRoleListReq();

		m_dwHostState = ST_RoleList;
	}

	if(m_dwHostState == ST_RoleListOk)
	{
		//SendCreateRoleReq();
		SendRoleLoginReq(0);
		m_dwHostState = ST_EnterScene;
	}

	if(m_dwHostState == ST_EnterSceneOK)
	{
		int randValue = rand()%100;
		if((randValue < 60)&&(randValue > 50))
		{
			//m_ClientConnector.DisConnect();

			//m_dwHostState = ST_Disconnected;
		}
		else
		{
			MoveHost();
		}
	}

	if(m_dwHostState == ST_Disconnected)
	{
		int randValue = rand()%100;
		if((randValue < 80)&&(randValue > 70))
		{
			m_dwHostState = ST_NONE;
		}
	}

	return TRUE;
}


BOOL CClientCmdHandler::SendNewAccountReq( std::string szAccountName, std::string szPassword )
{
	AccountRegReq Req;
	Req.set_accountname(szAccountName);
	Req.set_password(szPassword);
	m_ClientConnector.SendData(MSG_ACCOUNT_REG_REQ, Req, 0, 0);
	return TRUE;
}



BOOL CClientCmdHandler::SendAccountLoginReq(std::string szAccountName, std::string szPassword)
{
	AccountLoginReq Req;
	Req.set_accountname(szAccountName);
	Req.set_password(szPassword);
	m_ClientConnector.SendData(MSG_ACCOUNT_LOGIN_REQ,Req, 0, 0);

	return TRUE;
}

BOOL CClientCmdHandler::SendSelectSvrReq(UINT32 dwSvrID)
{
	SelectServerReq Req;
	Req.set_serverid(201);
	m_ClientConnector.SendData(MSG_SELECT_SERVER_REQ, Req, 0, 0);
	return TRUE;
}

BOOL CClientCmdHandler::OnMsgAccountLoginAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	AccountLoginAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;

	if(Ack.retcode() == MRC_FAILED)
	{
		MessageBox(NULL, "登录失败! 密码或账号不对!!","提示", MB_OK);
	}
	else
	{
		m_dwAccountID = Ack.accountid();
	}

	m_dwHostState = ST_LoginOK;
	return TRUE;
}


BOOL CClientCmdHandler::OnMsgSelectServerAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	SelectServerAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;
	m_ClientConnector.DisConnect();
	m_ClientConnector.ConnectToServer(Ack.serveraddr(), Ack.serverport());
	m_dwHostState = ST_SelectSvrOK;
	return TRUE;
}

BOOL CClientCmdHandler::OnMsgRoleListAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	RoleListAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;

	for( int i =0 ; i < Ack.rolelist_size(); i++)
	{
		m_RoleIDList.push_back(Ack.rolelist(i).id());
		m_dwHostState = ST_RoleListOk;
	}

	if(Ack.rolelist_size() <= 0)
	{
		SendCreateRoleReq(m_dwAccountID, m_strAccountName + CommonConvert::IntToString(rand()%1000), 1);
	}
	
	return TRUE;
}

BOOL CClientCmdHandler::OnMsgCreateRoleAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	RoleCreateAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;
	m_RoleIDList.push_back(Ack.roleid());
	m_dwHostState = ST_RoleListOk;
	return TRUE;
}

BOOL CClientCmdHandler::OnCmdNewAccountAck( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	AccountRegAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;
	m_dwHostState = ST_RegisterOK;
	return TRUE;
}

BOOL CClientCmdHandler::SendCreateRoleReq( UINT64 dwAccountID , std::string strName, UINT32 dwRoleType)
{
	RoleCreateReq Req;
	Req.set_accountid(dwAccountID);
	Req.set_name(strName);
	Req.set_roletype(dwRoleType);
	m_ClientConnector.SendData(MSG_ROLE_CREATE_REQ, Req, 0, 0);
	return TRUE;
}


// BOOL CClientCmdHandler::OnCmdNewCharAck( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
// {
// 	//m_DlgSelect.AddCharPickInfo(CharNewCharAck.CharPickInfo);
// 
// 	//m_DlgSelect.DoModal();
// 
// 	return TRUE;
// }

BOOL CClientCmdHandler::SendDelCharReq( UINT64 dwAccountID,UINT64 dwCharID )
{

	return TRUE;
}

// BOOL CClientCmdHandler::OnCmdDelCharAck( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
// {
// // 	StCharDelCharAck CharDelCharAck;
// // 	pBufferHelper->Read(CharDelCharAck);
// // 
// // 	if(CharDelCharAck.nRetCode == E_SUCCESSED)
// // 	{
// // 		m_DlgSelect.DelChar(CharDelCharAck.u64CharID);
// // 	}
// 
// 	m_DlgSelect.DoModal();
// 
// 	return TRUE;
// }

BOOL CClientCmdHandler::OnCmdUpdateMyself( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
{

	return TRUE;
}

VOID CClientCmdHandler::MoveHost()
{
	ObjectMoveReq Req;
	Req.set_objectid(m_RoleIDList[0]);
	m_ClientConnector.SendData(MSG_ROLE_MOVE_REQ, Req, 0, 0);
}

BOOL CClientCmdHandler::SendLeaveGameReq( UINT64 u64CharID )
{


	return TRUE;
}

BOOL CClientCmdHandler::SendRoleLoginReq(UINT64 u64CharID)
{
	RoleLoginReq Req;
	Req.set_roleid(m_RoleIDList[0]);
	m_ClientConnector.SendData(MSG_ROLE_LOGIN_REQ, Req, 0, 0);
	return TRUE;
}

BOOL CClientCmdHandler::SendMoveReq( FLOAT x, FLOAT y, FLOAT z, UINT16 nDir)
{


	return TRUE;
}




BOOL CClientCmdHandler::SendRoleListReq()
{
	RoleListReq Req;
	Req.set_accountid(m_dwAccountID);
	Req.set_logincode(12345678);
	m_ClientConnector.SendData(MSG_ROLE_LIST_REQ, Req, 0, 0);
	return TRUE;
}

BOOL CClientCmdHandler::SendCopyBattleReq()
{
	CopyBattleReq Req;
	Req.set_copytype(rand()%100);
	Req.set_roleid(m_RoleIDList[0]);
	m_ClientConnector.SendData(MSG_COPY_BATTLE_REQ, Req, 0, 0);
	return TRUE;
}
