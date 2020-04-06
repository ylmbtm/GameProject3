#include "stdafx.h"
#include "ClientObject.h"
#include "..\Src\Message\Msg_RetCode.pb.h"
#include "..\Src\Message\Msg_Move.pb.h"
#include "..\Src\Message\Game_Define.pb.h"
#include "..\Src\Message\Msg_Copy.pb.h"
#include "..\Src\Message\Msg_Game.pb.h"
#include "..\Src\Message\Msg_LoginCltData.pb.h"
#include "..\Src\ServerEngine\XMath.h"
#include "..\Src\ServerEngine\CommonFunc.h"
#include "..\Src\ServerEngine\PacketHeader.h"

#define PROCESS_MESSAGE_ITEM_CLIENT__(dwMsgID, Func) \
		case dwMsgID:{\
		if(Func(dwMsgID, PacketBuf, BufLen)){return TRUE;}}break;


#define PROCESS_MESSAGE_ITEM_CLIENT(dwMsgID, Func) \
		case dwMsgID:{\
		printf("---Receive Message:[%s]---- \n", #dwMsgID); \
		if(Func(dwMsgID, PacketBuf, BufLen)){return TRUE;}}break;

int g_LoginReqCount = 0;
int g_LoginCount	= 0;
int g_EnterCount	= 0;

CClientObject::CClientObject(void)
{
	m_bLoginOK = FALSE;

	m_dwAccountID = 0;
	m_dwHostState = ST_NONE;
	m_x = 0;
	m_y = -2;
	m_z = 11;
	m_dwCarrerID = 0;
	m_ft = rand() % 360;
	m_uSkillTime = 0;
	m_SkillID = 0;
	m_uMoveTime = 0;
	m_ClientConnector.RegisterMsgHandler((IMessageHandler*)this);
}

CClientObject::~CClientObject(void)
{
	m_uMoveTime = 0;
}

BOOL CClientObject::DispatchPacket(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
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
			PROCESS_MESSAGE_ITEM_CLIENT(MSG_OBJECT_NEW_NTF,			OnMsgObjectNewNty);
			PROCESS_MESSAGE_ITEM_CLIENT(MSG_OBJECT_CHANGE_NTF,		OnMsgObjectChangeNty);
			PROCESS_MESSAGE_ITEM_CLIENT(MSG_OBJECT_REMOVE_NTF,		OnMsgObjectRemoveNty);
			PROCESS_MESSAGE_ITEM_CLIENT(MSG_ENTER_SCENE_ACK,		OnCmdEnterSceneAck);
			PROCESS_MESSAGE_ITEM_CLIENT(MSG_ROLE_LOGIN_ACK,			OnMsgRoleLoginAck);
			PROCESS_MESSAGE_ITEM_CLIENT(MSG_ROLE_OTHER_LOGIN_NTY,	OnMsgOtherLoginNty);


		default:
		{
			bHandled = FALSE;
		}
		break;
	}

	return bHandled;
}


BOOL CClientObject::OnCmdEnterSceneAck( UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen )
{
	EnterSceneAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;
	if(Ack.copyid() == 6)
	{
		m_dwHostState = ST_EnterSceneOK;

		//表示进入主城完成
	}
	else if(Ack.copyid() == m_dwToCopyID)
	{
		m_dwHostState = ST_EnterCopyOK;
	}

	m_dwCopyID = Ack.copyid();
	m_dwCopyGuid = Ack.copyguid();
	//if(Ack.copytype() == 6)
	//{
	//	SendMainCopyReq();
	//}
	//else
	//{
	//	SendAbortCopyReq();
	//}


	return TRUE;
}

BOOL CClientObject::OnMsgNotifyIntoScene(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	NotifyIntoScene Nty;
	Nty.ParsePartialFromArray(PacketBuf, BufLen);
	EnterSceneReq Req;
	Req.set_roleid(Nty.roleid());
	Req.set_serverid(Nty.serverid());
	Req.set_copyguid(Nty.copyguid());
	Req.set_copyid(Nty.copyid());

	m_dwCopySvrID = Nty.serverid();
	m_dwCopyID = Nty.copyid();
	m_dwCopyGuid = Nty.copyguid();

	m_ClientConnector.SendData(MSG_ENTER_SCENE_REQ, Req, Nty.serverid(), Nty.copyguid());
	return TRUE;
}

BOOL CClientObject::OnMsgObjectNewNty(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	ObjectNewNty Nty;
	Nty.ParsePartialFromArray(PacketBuf, BufLen);

	for(int i = 0; i < Nty.newlist_size(); i++)
	{
		const NewItem& item = Nty.newlist(i);

		//printf("x:%f", item.x());
		//printf("y:%f", item.y());
		//printf("z:%f\n", item.z());
		//printf("actorid:%d\n", item.actorid());
	}

	return TRUE;
}

BOOL CClientObject::OnMsgObjectChangeNty(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	ObjectActionNty Nty;
	Nty.ParsePartialFromArray(PacketBuf, BufLen);
	for(int i = 0; i < Nty.actionlist_size(); i++)
	{
		const ActionNtyItem& Item = Nty.actionlist(i);

		printf("x:%f, z:%f, ft:%f\n", Item.hostx(), Item.hostz(), Item.hostft());
	}

	return TRUE;
}

BOOL CClientObject::OnMsgObjectRemoveNty(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	return TRUE;
}

BOOL CClientObject::OnUpdate( UINT32 dwTick )
{
	m_ClientConnector.Render();

	if(m_dwHostState == ST_NONE)
	{
		if(m_ClientConnector.GetConnectState() == ECS_NO_CONNECT)
		{
			//m_ClientConnector.ConnectTo("127.0.0.1", 9001);
			m_ClientConnector.ConnectTo("47.93.31.69", 9001);
			//m_ClientConnector.ConnectTo("47.105.89.43", 9001);
		}
		else if (m_ClientConnector.GetConnectState() == ECS_CONNECTED)
		{
			SendNewAccountReq(m_strAccountName, m_strPassword);

			m_dwHostState = ST_Register;
		}
	}

	if(m_dwHostState == ST_RegisterOK)
	{
		SendAccountLoginReq(m_strAccountName, m_strPassword);

		m_dwHostState = ST_AccountLogin;
	}

	if(m_dwHostState == ST_AccountLoginOK)
	{
		SendSelectSvrReq(202);

		m_dwHostState = ST_SelectSvr;
	}

	if(m_dwHostState == ST_SelectSvrOK)
	{
		SendRoleListReq();

		m_dwHostState = ST_RoleList;
	}

	if(m_dwHostState == ST_RoleListOK)
	{
		SendRoleLoginReq(m_RoleIDList[0]);
		m_dwHostState = ST_EnterScene;
	}

	if(m_dwHostState == ST_EnterSceneOK)
	{
		TestMove();
		//TestCopy();
	}

	if(m_dwHostState == ST_EnterCopyOK)
	{
		//TestMove();
		//TestExitCopy();
	}

	if(m_dwHostState == ST_Disconnected)
	{
		int randValue = rand() % 100;
		if((randValue < 80) && (randValue > 70))
		{
			m_dwHostState = ST_NONE;
		}
	}

	if(m_dwHostState == ST_Overed)
	{

	}

	return TRUE;
}


BOOL CClientObject::SendNewAccountReq( std::string szAccountName, std::string szPassword )
{
	AccountRegReq Req;
	Req.set_accountname(szAccountName);
	Req.set_password(szPassword);
	m_ClientConnector.SendData(MSG_ACCOUNT_REG_REQ, Req, 0, 0);
	return TRUE;
}



BOOL CClientObject::SendAccountLoginReq(std::string szAccountName, std::string szPassword)
{
	AccountLoginReq Req;
	Req.set_accountname(szAccountName);
	Req.set_password(szPassword);
	m_ClientConnector.SendData(MSG_ACCOUNT_LOGIN_REQ, Req, 0, 0);

	return TRUE;
}

BOOL CClientObject::SendSelectSvrReq(UINT32 dwSvrID)
{
	SelectServerReq Req;
	Req.set_serverid(dwSvrID);
	Req.set_accountid(m_dwAccountID);
	m_ClientConnector.SendData(MSG_SELECT_SERVER_REQ, Req, 0, 0);
	return TRUE;
}

BOOL CClientObject::OnMsgAccountLoginAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	AccountLoginAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;

	if(Ack.retcode() == MRC_UNKNOW_ERROR)
	{
		m_dwHostState = ST_Overed;
		return TRUE;
	}
	else
	{
		m_dwAccountID = Ack.accountid();
	}

	m_dwHostState = ST_AccountLoginOK;
	return TRUE;
}


BOOL CClientObject::OnMsgSelectServerAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	SelectServerAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;
	m_ClientConnector.DisConnect();
	m_ClientConnector.ConnectTo(Ack.serveraddr(), Ack.serverport());
	m_dwHostState = ST_SelectSvrOK;
	return TRUE;
}

BOOL CClientObject::OnMsgRoleListAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	RoleListAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;

	for( int i = 0 ; i < Ack.rolelist_size(); i++)
	{
		m_RoleIDList.push_back(Ack.rolelist(i).roleid());
		m_dwHostState = ST_RoleListOK;
	}

	if(Ack.rolelist_size() <= 0)
	{
		m_dwHostState = ST_RoleCreate;

		SendCreateRoleReq(m_dwAccountID, m_strAccountName + CommonConvert::IntToString(m_dwAccountID) + CommonConvert::IntToString(rand() % 1000), m_dwCarrerID);
	}

	return TRUE;
}

BOOL CClientObject::OnMsgCreateRoleAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	RoleCreateAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;
	m_RoleIDList.push_back(Ack.roleid());
	m_dwHostState = ST_RoleListOK;
	return TRUE;
}

BOOL CClientObject::OnCmdNewAccountAck( UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	AccountRegAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;
	m_dwHostState = ST_RegisterOK;
	return TRUE;
}

BOOL CClientObject::SendCreateRoleReq( UINT64 dwAccountID, std::string strName, UINT32 dwCarrerID)
{
	RoleCreateReq Req;
	Req.set_accountid(dwAccountID);
	Req.set_name(strName);
	Req.set_carrer(dwCarrerID);
	m_ClientConnector.SendData(MSG_ROLE_CREATE_REQ, Req, 0, 0);
	return TRUE;
}



BOOL CClientObject::SendDelCharReq( UINT64 dwAccountID, UINT64 u64RoleID )
{
	RoleDeleteReq Req;
	Req.set_accountid(dwAccountID);
	Req.set_roleid(u64RoleID);
	m_ClientConnector.SendData(MSG_ROLE_DELETE_REQ, Req, 0, 0);
	return TRUE;
}

VOID CClientObject::TestCopy()
{
	MainCopyReq Req;
	Req.set_copyid(rand() % 67 + 10001);
	m_dwToCopyID = Req.copyid();
	m_ClientConnector.SendData(MSG_MAIN_COPY_REQ, Req, m_RoleIDList[0], 0);
	m_dwHostState = ST_EnterCopy;
}


VOID CClientObject::TestExitCopy()
{
	AbortSceneReq Req;
	Req.set_roleid(m_RoleIDList[0]);
	Req.set_reason(0);
	m_ClientConnector.SendData(MSG_ABORT_SCENE_REQ, Req, m_RoleIDList[0], m_dwCopyGuid);
	m_dwHostState = ST_EnterCopy;
}

BOOL CClientObject::MoveForward(FLOAT fDistance)
{
	if(m_ft <= 90.0f)
	{
		m_x += fDistance * sin(m_ft * PI / 180); //+
		m_z += fDistance * cos(m_ft * PI / 180); //+
	}
	else if(m_ft <= 180.0f)
	{
		m_x += fDistance * sin(m_ft * PI / 180);  //+
		m_z += fDistance * cos(m_ft * PI / 180);  //-
	}
	else if(m_ft <= 270.0f)
	{
		m_x += fDistance * sin(m_ft * PI / 180); //-
		m_z += fDistance * cos(m_ft * PI / 180); //-
	}
	else if(m_ft <= 360.0f)
	{
		m_x += fDistance * sin(m_ft * PI / 180); //-
		m_z += fDistance * cos(m_ft * PI / 180); //+
	}

	return TRUE;
}

VOID CClientObject::TestMove()
{
	ObjectActionReq Req;
	ActionReqItem* pItem =  Req.add_actionlist();
	pItem->set_actionid(AT_RUN);
	pItem->set_objectguid(m_RoleIDList[0]);

	if (m_uMoveTime <= 0)
	{
		m_uMoveTime = CommonFunc::GetTickCount();
	}


	UINT64 dwTimeDiff = CommonFunc::GetTickCount() - m_uMoveTime;
	if (dwTimeDiff > 100)
	{
		FLOAT fSpeed = 6.25f;

		FLOAT fTime = dwTimeDiff;

		fTime = fTime / 1000.0f;

		printf("Distance:%f\n", fSpeed * fTime);

		MoveForward(fSpeed * fTime);

		m_uMoveTime = CommonFunc::GetTickCount();

		bool bTrun = false;

		if (m_x > 10)
		{
			bTrun = true;
			m_x = 10;
		}

		if (m_z > 20)
		{
			bTrun = true;
			m_z = 20;
		}

		if (m_x < -10)
		{
			bTrun = true;
			m_x = -10;
		}

		if (m_z < 0)
		{
			bTrun = true;
			m_z = 0;
		}

		if (bTrun)
		{
			m_ft += 180;
		}

		m_ft = m_ft > 360 ? (m_ft - 360) : m_ft;

		pItem->set_hostx(m_x);
		pItem->set_hosty(-2.45);
		pItem->set_hostz(m_z);
		pItem->set_hostft(m_ft);
		m_ClientConnector.SendData(MSG_OBJECT_ACTION_REQ, Req, m_RoleIDList[0], m_dwCopyGuid);
	}
}

VOID CClientObject::TestCastSkill()
{
	if (m_uSkillTime == 0)
	{
		m_uSkillTime = CommonFunc::GetTickCount();
	}

	UINT64 dwTimeDiff = CommonFunc::GetTickCount() - m_uSkillTime;
	if (dwTimeDiff < 10000)
	{

		return;
	}

	SkillCastReq Req;
	Req.set_objectguid(m_RoleIDList[0]);
	Req.set_skillid(m_SkillID);

	m_uSkillTime = CommonFunc::GetTickCount();

	m_ClientConnector.SendData(MSG_SKILL_CAST_REQ, Req, m_RoleIDList[0], m_dwCopyGuid);
}

BOOL CClientObject::SendRoleLogoutReq( UINT64 u64CharID )
{
	return TRUE;
}

BOOL CClientObject::SendRoleLoginReq(UINT64 u64CharID)
{
	RoleLoginReq Req;
	Req.set_accountid(m_dwAccountID);
	Req.set_roleid(u64CharID);
	Req.set_logincode(11111);
	m_ClientConnector.SendData(MSG_ROLE_LOGIN_REQ, Req, 0, 0);
	return TRUE;
}

BOOL CClientObject::SendRoleListReq()
{
	if(m_dwAccountID == 0)
	{
		ASSERT_FAIELD;
		m_dwHostState = ST_Overed;
		return TRUE;
	}

	RoleListReq Req;
	Req.set_accountid(m_dwAccountID);
	Req.set_logincode(12345678);
	m_ClientConnector.SendData(MSG_ROLE_LIST_REQ, Req, 0, 0);
	return TRUE;
}

BOOL CClientObject::SendMainCopyReq()
{
	MainCopyReq Req;
	Req.set_copyid(rand() % 67 + 10000);
	m_ClientConnector.SendData(MSG_MAIN_COPY_REQ, Req, m_RoleIDList[0], 0);
	return TRUE;
}

BOOL CClientObject::SendAbortCopyReq()
{
	AbortSceneReq Req;
	Req.set_roleid(m_RoleIDList[0]);
	Req.set_reason(0);
	m_ClientConnector.SendData(MSG_ABORT_SCENE_REQ, Req, m_RoleIDList[0], m_dwCopyGuid);
	m_dwHostState = ST_AbortCopy;
	return TRUE;
}

BOOL CClientObject::OnMsgOtherLoginNty( UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen )
{
	m_ClientConnector.DisConnect();

	return TRUE;
}

BOOL CClientObject::OnMsgRoleLoginAck(UINT32 dwMsgID, CHAR* PacketBuf, INT32 BufLen)
{
	RoleLoginAck Ack;
	Ack.ParsePartialFromArray(PacketBuf, BufLen);
	PacketHeader* pHeader = (PacketHeader*)PacketBuf;
	m_RoleIDList.push_back(Ack.roleid());

	for (int i = 0; i < Ack.skilllist_size(); i++)
	{
		const SkillItem skillItem = Ack.skilllist(i);
		if (skillItem.keypos() == 1)
		{
			m_SkillID = skillItem.skillid();
		}

	}


	static int loginnum = 0;

	loginnum++;

	printf("login success num:%d\n", loginnum);

	return TRUE;
}
