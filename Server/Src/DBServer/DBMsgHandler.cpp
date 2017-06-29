#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "DBMsgHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "Utility/CommonSocket.h"
#include "DataBuffer.h"
#include "DBStoredProcMgr.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"




CDBMsgHandler::CDBMsgHandler()
{

}

CDBMsgHandler::~CDBMsgHandler()
{

}

BOOL CDBMsgHandler::Init(UINT32 dwReserved)
{
	if(!CCommonMsgHandler::Init(dwReserved))
	{
		return FALSE;
	}

	if(!m_DBProcManager.Init())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDBMsgHandler::Uninit()
{
	CCommonMsgHandler::Uninit();

	m_DBProcManager.Uninit();

	return TRUE;
}


BOOL CDBMsgHandler::DispatchPacket(NetPacket *pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_REQ,			OnMsgRoleListReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_CREATE_REQ,		OnMsgCreateRoleReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_DELETE_REQ,		OnMsgDeleteRoleReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_REQ,		OnMsgRoleLoginReq);
	default:
		{

		}
		break;
	}


	return TRUE;
}

BOOL CDBMsgHandler::OnUpdate(UINT32 dwTick)
{
	return TRUE;
}

BOOL CDBMsgHandler::OnThreadBegin()
{
	m_DBProcManager.Init();
	/*
	m_DBConnection.Init();

	m_DBConnection.Connect("127.0.0.1","root","123456", "db_log", 3306);
	m_DBProceduceMgr.InitStoredProcedures();
	CDBStoredProcedure *pProcedure = NULL;
	
 	pProcedure = m_DBProceduceMgr.GetStoredProcedure(DB_INSERT_PLAYER_INFO);
 	pProcedure->set_int8(0, 10);
	pProcedure->set_string(1, "test", 5);
	pProcedure->set_int32(2, 10);
	pProcedure->set_int32(3, 10);
 	m_DBConnection.Execute(pProcedure);
 
	while(pProcedure->m_DBRecordSet.MoveNext())
	{
		int nValue = pProcedure->m_DBRecordSet.get_int32((size_t)0);
		printf("%d", nValue);
	}

	pProcedure->m_DBRecordSet.ClearRecordSet();

 	//pProcedure->m_DBRecordSet.MoveNext(0);
 


	/*
	pProcedure = m_DBProceduceMgr.GetStoredProcedure(DB_FIND_PLAYER_INFO);
	pProcedure->set_int32(0, 2);
	m_DBConnection.Query(pProcedure);


	int n = pProcedure->m_DBRecordSet.GetRowCount();

	while(pProcedure->m_DBRecordSet.MoveNext())
	{
		int nValue = pProcedure->m_DBRecordSet.get_int32("id");
		nValue = pProcedure->m_DBRecordSet.get_int32("fb");
		nValue = pProcedure->m_DBRecordSet.get_int32("fi");
		char * p = pProcedure->m_DBRecordSet.get_string("fc");

		printf(p);
	}
	*/

	return TRUE;
}

BOOL CDBMsgHandler::OnThreadEnd()
{
	m_DBConnection.Uninit();

	return TRUE;
}

BOOL CDBMsgHandler::OnCommandHandle(UINT32 dwMsgID, UINT64 u64ConnID, IDataBuffer *pDataBuffer)
{
	return TRUE;
}

BOOL CDBMsgHandler::OnMsgRoleListReq(NetPacket *pPacket)
{
	RoleListReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	RoleListAck Ack;
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_pConnect->GetConnectionID(),  MSG_ROLE_LIST_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
}

BOOL CDBMsgHandler::OnMsgCreateRoleReq(NetPacket *pPacket)
{
	RoleCreateReq  Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->dwUserData != 0);

	m_DBProcManager.CreateRole(Req.roleid(), Req.accountid(), (char*)Req.name().c_str(), Req.roletype(), 1);

	RoleCreateAck Ack;
	Ack.set_accountid(Req.accountid());
	Ack.set_roleid(Req.roleid());
	Ack.set_roletype(Req.roletype());
	Ack.set_name(Req.name());
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_pConnect->GetConnectionID(),  MSG_ROLE_CREATE_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);

	return TRUE;
}

BOOL CDBMsgHandler::OnMsgDeleteRoleReq(NetPacket *pPacket)
{
	return TRUE;
}

BOOL CDBMsgHandler::OnMsgRoleLoginReq(NetPacket *pPacket)
{
	RoleLoginReq  Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	RoleLoginAck Ack;
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_pConnect->GetConnectionID(),  MSG_ROLE_LOGIN_ACK, 0, pHeader->dwUserData, Ack);
}



