#include "stdafx.h"
#include "CommandHandler.h"
#include "PacketDef/ServerPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "ConnectionType.h"
#include "PacketDef/ClientPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "Error.h"
#include "ObjectID.h"
#include <complex>

int g_LoginReqCount = 0;
int g_LoginCount = 0;
int g_EnterCount = 0;

CClientCmdHandler::CClientCmdHandler(void)
{
	m_dwHostState = ST_NONE;

	m_ClientConnector.RegisterMsgHandler((IMessageHandler*)this);
}

CClientCmdHandler::~CClientCmdHandler(void)
{
}

BOOL CClientCmdHandler::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{ 
	BOOL bHandled = TRUE;
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_LOGIN_ACK,		OnCmdLoginGameAck);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEW_CHAR_ACK,	OnCmdNewCharAck);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_DEL_CHAR_ACK,	OnCmdDelCharAck);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEW_ACCOUNT_ACK,OnCmdNewAccountAck);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_ENTER_GAME_ACK,	OnCmdEnterGameAck)

		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEARBY_ADD,		OnCmdNearByAdd);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEARBY_UPDATE,	OnCmdNearByUpdate);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEARBY_REMOVE,	OnCmdNearByRemove);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_UPDATE_MYSELF,	OnCmdUpdateMyself);

	default:
		{
			bHandled = FALSE;
		}
		break;
	}

	return bHandled;
}

BOOL CClientCmdHandler::OnCmdNearByAdd( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);
	
	for(UINT32 i = 0; i < dwCount; i++)
	{
		UINT8 type = 0;

		pBufferHelper->Read(type);

		if(type != 1)
		{
			continue;
		}

		CPlayerObject *pObject = new CPlayerObject;

		pObject->ReadFromBuffer(pBufferHelper);

		m_PlayerObjMgr.AddPlayer(pObject);

		/*printf("添加角色:%d, 坐标x = %f, z = %f", (UINT32)pObject->GetObjectID(), pObject->m_ObjectPos.x, pObject->m_ObjectPos.z);*/
	}

	printf("-------周围队列的人数:%d------OnCmdNearByAdd---\n", m_PlayerObjMgr.size());

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdNearByUpdate( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);

	for(UINT32 i = 0; i < dwCount; i++)
	{
		UINT64 u64CharID = 0;

		pBufferHelper->Read(u64CharID);

		CPlayerObject *pObject = m_PlayerObjMgr.GetPlayer(u64CharID);
		if(pObject != NULL)
		{
			pObject->ReadFromBuffer(pBufferHelper);

			//printf("更新角色:%d, 坐标x = %f, z = %f", (UINT32)pObject->GetObjectID(), pObject->m_ObjectPos.x, pObject->m_ObjectPos.z);
		}
		else
		{
			//ASSERT_FAIELD;
		}
	}

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdNearByRemove( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);

	//printf("BEGIN---删除角色消息，预计删除人数:%d, 现在人数:%d", dwCount, m_PlayerObjMgr.size());

	for(UINT32 i = 0; i < dwCount; i++)
	{
		UINT64 u64CharID = 0;

		pBufferHelper->Read(u64CharID);

		CPlayerObject *pObj = m_PlayerObjMgr.GetPlayer(u64CharID);
		if(pObj == NULL)
		{
			//ASSERT_FAIELD;
			return 0;
		}

		m_PlayerObjMgr.RemovePlayer(u64CharID);

		delete pObj;

		//printf("删除角色:%d成功", (UINT32)u64CharID);
	}

	printf("-------周围队列的人数:%d-----OnCmdNearByRemove----\n", m_PlayerObjMgr.size());

	//printf("END---删除角色消息");

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdEnterGameAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharEnterGameAck CharEnterGameAck;

	pBufferHelper->Read(CharEnterGameAck);

	m_HostPlayer.ReadFromBuffer(pBufferHelper);

	CHECK_PAYER_ID(m_HostPlayer.GetObjectID());

	m_HostPlayer.m_ObjectStatus.nDir = rand()%4+1; 

	g_EnterCount++;
	printf("%s己成功进入游戏服,总人数:%d\n",m_strRoleName.c_str(), g_EnterCount);

	m_dwHostState = ST_EnterGame;

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

			m_ClientConnector.ConnectToServer("127.0.0.1", 7994);
		}

		if(m_ClientConnector.GetConnectState() == Succ_Connect)
		{
			m_ClientConnector.Login(m_strAccountName.c_str(), m_strPassword.c_str());

			m_dwHostState = ST_Logining;

			g_LoginReqCount++;
			printf("申请登录人数:%d\n", g_LoginReqCount);
		}
	}

	if(m_dwHostState == ST_Logined)
	{
		UINT32 dwTick = GetTickCount();

		SendPickCharReq(m_RoleIDList[0]);

		m_dwHostState = ST_Entering;
	}

	if(m_dwHostState == ST_EnterGame)
	{
		int randValue = rand()%100;
		if((randValue < 60)&&(randValue > 50))
		{
			m_ClientConnector.DisConnect();

			m_dwHostState = ST_Disconnected;
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


BOOL CClientCmdHandler::SendNewAccountReq( LPCTSTR szAccountName, LPCTSTR szPassword )
{
	StCharNewAccountReq CharNewAccountReq;
	strncpy(CharNewAccountReq.szAccountName, szAccountName, 32);
	strncpy(CharNewAccountReq.szPassword, szPassword, 32);

	CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());

	WriteHelper.BeginWrite(CMD_CHAR_NEW_ACCOUNT_REQ, 0, 0, 0);

	WriteHelper.Write(CharNewAccountReq);

	WriteHelper.EndWrite();

	m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetData(), m_ClientConnector.GetWriteBuffer()->GetDataLenth());

	return TRUE;
}



BOOL CClientCmdHandler::OnCmdLoginGameAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharLoginAck MsgLoginAck;

	pBufferHelper->Read(MsgLoginAck);

	if(MsgLoginAck.nRetCode == E_FAILED)
	{
		::MessageBox(NULL, "登录失败! 密码或账号不对!!","提示", MB_OK);
	}
	else
	{
		/*m_DlgSelect.m_dwAccountID = MsgLoginAck.dwAccountID;
		m_DlgSelect.m_nCount = MsgLoginAck.nCount;
		for(int i = 0; i < MsgLoginAck.nCount; i++)
		{
			m_DlgSelect.m_CharInfoList.push_back(MsgLoginAck.CharPickInfo[i]);
		}
		
		m_DlgSelect.DoModal();*/

		if(MsgLoginAck.nCount <= 0)
		{
			SendNewCharReq(MsgLoginAck.dwAccountID, m_strRoleName.c_str(), 123);
		}
		else 
		{
			for(int i = 0; i < MsgLoginAck.nCount; i++)
			{
				m_RoleIDList.push_back(MsgLoginAck.CharPickInfo[i].u64CharID);
			}

			m_dwHostState = ST_Logined;
			g_LoginCount++;
			printf("己登录人数:%d\n", g_LoginCount);
		}
	}

	return TRUE;
}


BOOL CClientCmdHandler::SendPickCharReq( UINT64 u64CharID )
{
	m_dwHostState = ST_Entering;
	StCharPickCharReq CharPickCharReq;
	CharPickCharReq.u64CharID = u64CharID;

	CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());

	CHECK_PAYER_ID(u64CharID);

	WriteHelper.BeginWrite(CMD_CHAR_PICK_CHAR_REQ, 0, 0, 0);

	WriteHelper.Write(CharPickCharReq);

	WriteHelper.EndWrite();

	m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetData(), m_ClientConnector.GetWriteBuffer()->GetDataLenth());

	return TRUE;
}


BOOL CClientCmdHandler::OnCmdNewAccountAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharNewAccountAck CharNewAccountAck;
	pBufferHelper->Read(CharNewAccountAck);
	if(CharNewAccountAck.nRetCode == E_SUCCESSED)
	{
		MessageBox(NULL,"注册账号成功!", "提示", MB_OK);
	}
	else
	{
		MessageBox(NULL,"注册账号失败!", "提示", MB_OK);
	}

	return TRUE;
}

BOOL CClientCmdHandler::SendNewCharReq( UINT32 dwAccountID , LPCTSTR szCharName, UINT32 dwFeature)
{
	StCharNewCharReq CharNewCharReq;
	CharNewCharReq.dwFeature = dwFeature;
	CharNewCharReq.dwAccountID = dwAccountID;
	strncpy(CharNewCharReq.szCharName, szCharName, 32);
	CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());

	WriteHelper.BeginWrite(CMD_CHAR_NEW_CHAR_REQ, 0, 0, 0);

	WriteHelper.Write(CharNewCharReq);

	WriteHelper.EndWrite();

	m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetData(), m_ClientConnector.GetWriteBuffer()->GetDataLenth());

	return TRUE;
}


BOOL CClientCmdHandler::OnCmdNewCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharNewCharAck CharNewCharAck;
	pBufferHelper->Read(CharNewCharAck);

	CHECK_PAYER_ID(CharNewCharAck.CharPickInfo.u64CharID);

	return TRUE;
}

BOOL CClientCmdHandler::SendDelCharReq( UINT32 dwAccountID,UINT64 dwCharID )
{
	StCharDelCharReq CharDelCharReq;
	CharDelCharReq.dwAccountID = dwAccountID;
	CharDelCharReq.u64CharID    = dwCharID;
	CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());

	CHECK_PAYER_ID(dwCharID);

	WriteHelper.BeginWrite(CMD_CHAR_DEL_CHAR_REQ, 0, 0, 0);

	WriteHelper.Write(CharDelCharReq);

	WriteHelper.EndWrite();

	m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetData(), m_ClientConnector.GetWriteBuffer()->GetDataLenth());

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdDelCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharDelCharAck CharDelCharAck;
	pBufferHelper->Read(CharDelCharAck);

	return TRUE;
}

BOOL CClientCmdHandler::OnCmdUpdateMyself( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	pBufferHelper->ReadCheckBufferCode();
	m_HostPlayer.ReadFromBuffer(pBufferHelper);
	pBufferHelper->ReadCheckBufferCode();

	return TRUE;
}

BOOL CClientCmdHandler::SendLeaveGameReq( UINT64 u64CharID )
{
	StCharLeaveGameReq CharLeaveGameReq;

	CharLeaveGameReq.dwLeaveReason = LGR_Quit;

	CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());

	WriteHelper.BeginWrite(CMD_CHAR_LEAVE_GAME_REQ, CMDH_SENCE, 0, u64CharID);

	WriteHelper.Write(CharLeaveGameReq);

	WriteHelper.EndWrite();

	m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetData(), m_ClientConnector.GetWriteBuffer()->GetDataLenth());

	return TRUE;
}

BOOL CClientCmdHandler::SendMoveReq( FLOAT x, FLOAT y, FLOAT z, UINT16 nDir)
{
	StCharMoveReq _MoveGs;
	_MoveGs.x = x;
	_MoveGs.y = y;
	_MoveGs.z = z;
	_MoveGs.sDir = nDir;

	CBufferHelper WriteHelper(TRUE, m_ClientConnector.GetWriteBuffer());

	WriteHelper.BeginWrite(CMD_CHAR_MOVE_REQ, CMDH_SENCE, 12, m_HostPlayer.GetObjectID());

	WriteHelper.Write(_MoveGs);

	WriteHelper.EndWrite();

	CHECK_PAYER_ID(m_HostPlayer.GetObjectID());

	m_ClientConnector.SendData(m_ClientConnector.GetWriteBuffer()->GetData(), m_ClientConnector.GetWriteBuffer()->GetDataLenth());

	return TRUE;
}

VOID CClientCmdHandler::MoveHost()
{
	static UINT32 dwLastTick = 0;

	UINT32 dwCurTick = GetTickCount();

	if((dwCurTick - dwLastTick) >1000)
	{
		dwLastTick = dwCurTick;

		m_HostPlayer.m_ObjectPos.x += 1;

		m_HostPlayer.m_ObjectPos.z += 1;

		if((m_HostPlayer.m_ObjectPos.x>900)||(m_HostPlayer.m_ObjectPos.x<-900))
		{
			m_HostPlayer.m_ObjectPos.x = 0;

			m_HostPlayer.m_ObjectStatus.nDir = rand()%4+1; 

		}

		if((m_HostPlayer.m_ObjectPos.z>900)||(m_HostPlayer.m_ObjectPos.z<-900))
		{
			m_HostPlayer.m_ObjectPos.z = 0;

			m_HostPlayer.m_ObjectStatus.nDir = rand()%4+1; 
		}

		SendMoveReq(m_HostPlayer.m_ObjectPos.x, m_HostPlayer.m_ObjectPos.y, m_HostPlayer.m_ObjectPos.z, m_HostPlayer.m_ObjectStatus.nDir);
	}
	
}




