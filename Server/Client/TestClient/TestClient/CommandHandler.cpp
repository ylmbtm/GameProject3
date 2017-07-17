#include "stdafx.h"
#include "CommandHandler.h"
#include "resource.h"
#include "TestClientDlg.h"
#include "DlgSelect.h"
#include "..\Src\Message\Msg_ID.pb.h"
#include "..\Src\Message\Msg_RetCode.pb.h"
#include "Utility\Log\Log.h"
#include "..\Src\Message\Msg_Login.pb.h"
#include "PacketHeader.h"


CClientCmdHandler::CClientCmdHandler(void)
{
	m_bLoginOK = FALSE;

	m_dwAccountID = 0;

	m_ClientConnector.RegisterMsgHandler((IMessageHandler*)this);
}

CClientCmdHandler::~CClientCmdHandler(void)
{
}

CClientCmdHandler* CClientCmdHandler::GetInstancePtr()
{
	static CClientCmdHandler _Handler;

	return &_Handler;
}

BOOL CClientCmdHandler::DispatchPacket( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
{ 
	BOOL bHandled = TRUE;
	switch(dwMsgID)
	{
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_ACCOUNT_LOGIN_ACK,		OnMsgAccountLoginAck);
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_SELECT_SERVER_ACK,      OnMsgSelectServerAck);
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_ACCOUNT_REG_ACK,		OnCmdNewAccountAck);
		PROCESS_MESSAGE_ITEM_CLIENT(MSG_ROLE_LIST_ACK,			OnMsgRoleListAck);
		
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

BOOL CClientCmdHandler::OnCmdEnterGameAck( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen )
{
// 	StCharEnterGameAck CharEnterGameAck;
// 
// 	pBufferHelper->Read(CharEnterGameAck);
// 
// 	m_HostPlayer.ReadFromBuffer(pBufferHelper);
// 
// 	CHECK_PAYER_ID(m_HostPlayer.GetObjectID());
// 
// 	printf("登录成功!");
// 
// 	m_bLoginOK = TRUE;
// 
// 	((CTestClientDlg*)AfxGetMainWnd())->m_SceneView.Invalidate();
// 
// 	((CTestClientDlg*)AfxGetMainWnd())->SetWindowText((LPCTSTR)m_HostPlayer.m_szObjectName);

	return TRUE;
}

BOOL CClientCmdHandler::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}


BOOL CClientCmdHandler::SendNewAccountReq( LPCTSTR szAccountName, LPCTSTR szPassword )
{
// 	StCharNewAccountReq CharNewAccountReq;
// 	strncpy(CharNewAccountReq.szAccountName, szAccountName, 32);
// 	strncpy(CharNewAccountReq.szPassword, szPassword, 32);
// 
// 	CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());
// 
// 	WriteHelper.BeginWrite(CMD_CHAR_NEW_ACCOUNT_REQ, 0, 0);
// 
// 	WriteHelper.Write(CharNewAccountReq);
// 
// 	WriteHelper.EndWrite();
// 
// 	m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetBuffer(), m_ClientConnector.GetWriteBuffer()->GetTotalLenth());

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

	SelectServerReq Req;
	Req.set_serverid(201);
	m_ClientConnector.SendData(MSG_SELECT_SERVER_REQ, Req, 0, 0);
	return TRUE;
}


BOOL CClientCmdHandler::OnMsgSelectServerAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	SelectServerAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;

	m_ClientConnector.DisConnect();
	m_ClientConnector.ConnectToServer(Ack.serveraddr(), Ack.serverport());

	RoleListReq Req;
	Req.set_accountid(m_dwAccountID);
	Req.set_logincode(12345678);
	m_ClientConnector.SendData(MSG_ROLE_LIST_REQ, Req, 0, 0);
	return TRUE;
}

BOOL CClientCmdHandler::OnMsgRoleListAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	RoleListAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;

	for( int i =0 ; i < Ack.rolelist_size(); i++)
	{
		RoleNode tNode;
		tNode.dwLevel = 0;
		tNode.Name = Ack.rolelist(i).name();
		tNode.m_dwRoleType = Ack.rolelist(i).roletype();
		tNode.m_u64ID = Ack.rolelist(i).id();
		m_DlgSelect.AddCharPickInfo(tNode);
	}
	
	m_DlgSelect.DoModal();

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdNewAccountAck( UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen)
{
	return TRUE;
}

BOOL CClientCmdHandler::SendNewCharReq( UINT32 dwAccountID , LPCTSTR szCharName, UINT32 dwFeature)
{

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

BOOL CClientCmdHandler::SendDelCharReq( UINT32 dwAccountID,UINT64 dwCharID )
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
	//pBufferHelper->ReadCheckBufferCode();
	//m_HostPlayer.ReadFromBuffer(pBufferHelper);
	//pBufferHelper->ReadCheckBufferCode();
	//((CTestClientDlg*)AfxGetMainWnd())->m_SceneView.Invalidate();
	return TRUE;
}

BOOL CClientCmdHandler::SendLeaveGameReq( UINT64 u64CharID )
{
	//StCharLeaveGameReq CharLeaveGameReq;

	//CharLeaveGameReq.dwLeaveReason = LGR_Quit;

	//CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());

	//WriteHelper.BeginWrite(CMD_CHAR_LEAVE_GAME_REQ, 0, u64CharID);

	//WriteHelper.Write(CharLeaveGameReq);

	//WriteHelper.EndWrite();

	//m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetBuffer(), m_ClientConnector.GetWriteBuffer()->GetTotalLenth());

	return TRUE;
}

BOOL CClientCmdHandler::SendRoleLoginReq(UINT64 u64CharID)
{
	RoleLoginReq Req;
	Req.set_roleid(u64CharID);
	m_ClientConnector.SendData(MSG_ROLE_LOGIN_REQ, Req, 0, 0);
	return TRUE;
}

BOOL CClientCmdHandler::SendMoveReq( FLOAT x, FLOAT y, FLOAT z, UINT16 nDir)
{
	//StCharMoveReq _MoveGs;
	//_MoveGs.x = x;
	//_MoveGs.y = y;
	//_MoveGs.z = z;
	//_MoveGs.sDir = nDir;

	//CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());

	//WriteHelper.BeginWrite(CMD_CHAR_MOVE_REQ, 12, m_HostPlayer.GetObjectID());
// 
// 	WriteHelper.Write(_MoveGs);
// 
// 	WriteHelper.EndWrite();
// 
// 	CHECK_PAYER_ID(m_HostPlayer.GetObjectID());
// 
// 	m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetBuffer(), m_ClientConnector.GetWriteBuffer()->GetTotalLenth());

	return TRUE;
}




