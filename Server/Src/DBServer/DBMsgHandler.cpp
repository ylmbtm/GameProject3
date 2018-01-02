#include "stdafx.h"
#include "CommandDef.h"
#include "DBMsgHandler.h"
#include "Log.h"
#include "CommonFunc.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "DataBuffer.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"


Th_RetName _DBWorkThread(void* pParam)
{
	CDBMsgHandler* pHandler = (CDBMsgHandler*)pParam;

	pHandler->Run();

	return Th_RetValue;
}


CDBMsgHandler::CDBMsgHandler()
{

}

CDBMsgHandler::~CDBMsgHandler()
{

}

BOOL CDBMsgHandler::Init(UINT32 dwReserved)
{
	ERROR_RETURN_FALSE(m_DBManager.Init());

	//m_bRun = TRUE;

	//m_hThread = CommonThreadFunc::CreateThread(_DBWorkThread, this);

	//if (m_hThread != NULL)
	//{
	//	return TRUE;
	//}

	return TRUE;
}

BOOL CDBMsgHandler::Run()
{
	while (m_bRun)
	{
		NetPacket* pPacket = NULL;

		while (m_PacketQueue.pop(pPacket))
		{
			if (pPacket != NULL && pPacket->m_pDataBuffer != NULL)
			{
				DispatchPacket(pPacket);

				pPacket->m_pDataBuffer->Release();
			}
		}

		CommonFunc::Sleep(1);
	}

	return TRUE;
}

BOOL CDBMsgHandler::Uninit()
{
	m_DBManager.Uninit();

	return TRUE;
}


BOOL CDBMsgHandler::AddPacket(NetPacket* pNetPacket)
{
	m_PacketQueue.push(pNetPacket);
	return TRUE;
}


BOOL CDBMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_REQ,			OnMsgRoleListReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_REQ,		OnMsgRoleLoginReq);
			PROCESS_MESSAGE_ITEM(MSG_DB_EXE_SQL_REQ,		OnMsgExeSqlReq);
	}


	return FALSE;
}

BOOL CDBMsgHandler::OnUpdate(UINT64 uTick)
{
	return TRUE;
}

BOOL CDBMsgHandler::OnMsgRoleListReq(NetPacket* pPacket)
{
	RoleListReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	RoleListAck Ack;
	m_DBManager.GetRoleList(Req.accountid(), Ack);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID,  MSG_ROLE_LIST_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
}

BOOL CDBMsgHandler::OnMsgRoleLoginReq(NetPacket* pPacket)
{
	RoleLoginReq  Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	DBRoleLoginAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	Ack.set_roleid(Req.roleid());
	m_DBManager.GetRoleData(Req.roleid(),		Ack);
	m_DBManager.GetBagData(Req.roleid(),		Ack);
	m_DBManager.GetCopyData(Req.roleid(),		Ack);
	m_DBManager.GetEquipData(Req.roleid(),		Ack);
	m_DBManager.GetPetData(Req.roleid(),		Ack);
	m_DBManager.GetPartnerData(Req.roleid(),	Ack);
	m_DBManager.GetTaskData(Req.roleid(),		Ack);
	m_DBManager.GetMountData(Req.roleid(),		Ack);
	m_DBManager.GetActivtyData(Req.roleid(),	Ack);
	m_DBManager.GetMailData(Req.roleid(),		Ack);
	m_DBManager.GetCounterData(Req.roleid(),	Ack);
	m_DBManager.GetFriendData(Req.roleid(),		Ack);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID,  MSG_ROLE_LOGIN_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
}

BOOL CDBMsgHandler::OnMsgExeSqlReq(NetPacket* pPacket)
{
	return TRUE;
}


