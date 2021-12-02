#include "stdafx.h"
#include "DBMsgHandler.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "DataBuffer.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Role.pb.h"

CDBMsgHandler::CDBMsgHandler()
{

}

CDBMsgHandler::~CDBMsgHandler()
{

}

BOOL CDBMsgHandler::Init(INT32 nReserved)
{
	ERROR_RETURN_FALSE(m_DBManager.Init());

	//m_bRun = TRUE;

	//m_pThread = new std::thread(&CDBMsgHandler::Run, this);

	//ERROR_RETURN_FALSE(m_pThread != NULL);

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
	m_bRun = FALSE;

//     m_pThread->join();
//
//     delete m_pThread;
//
//     m_pThread = NULL;

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
	switch(pNetPacket->m_nMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_REQ,			OnMsgRoleListReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_REQ,		OnMsgRoleLoginReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_DELETE_REQ,       OnMsgRoleDeleteReq);
			PROCESS_MESSAGE_ITEM(MSG_DB_EXE_SQL_REQ,		OnMsgExeSqlReq);
			PROCESS_MESSAGE_ITEM(MSG_LOGIC_REGTO_DBSVR_REQ, OnMsgLogicSvrRegReq);
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
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID,  MSG_ROLE_LIST_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
}

BOOL CDBMsgHandler::OnMsgRoleLoginReq(NetPacket* pPacket)
{
	RoleLoginReq  Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	DBRoleLoginAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	Ack.set_roleid(Req.roleid());

	//一个角色其它的数据可能都没有，但角色基本信息必须有
	if (!m_DBManager.GetRoleData(Req.roleid(), Ack))
	{
		Ack.set_retcode(MRC_INVALID_ROLEID);
		Ack.set_roleid(Req.roleid());
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_ROLE_LOGIN_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);
		return TRUE;
	}

	m_DBManager.GetBagData(Req.roleid(),		Ack);
	m_DBManager.GetCopyData(Req.roleid(),		Ack);
	m_DBManager.GetEquipData(Req.roleid(),		Ack);
	m_DBManager.GetGemData(Req.roleid(),		Ack);
	m_DBManager.GetPetData(Req.roleid(),		Ack);
	m_DBManager.GetPartnerData(Req.roleid(),	Ack);
	m_DBManager.GetTaskData(Req.roleid(),		Ack);
	m_DBManager.GetMountData(Req.roleid(),		Ack);
	m_DBManager.GetActivtyData(Req.roleid(),	Ack);
	m_DBManager.GetMailData(Req.roleid(),		Ack);
	m_DBManager.GetCounterData(Req.roleid(),	Ack);
	m_DBManager.GetFriendData(Req.roleid(),		Ack);
	m_DBManager.GetSkillData(Req.roleid(),		Ack);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID,  MSG_ROLE_LOGIN_ACK, pHeader->u64TargetID, pHeader->dwUserData, Ack);

	return TRUE;
}

BOOL CDBMsgHandler::OnMsgRoleDeleteReq(NetPacket* pPacket)
{
	RoleDeleteReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	return TRUE;
}

BOOL CDBMsgHandler::OnMsgExeSqlReq(NetPacket* pPacket)
{



	return TRUE;
}

BOOL CDBMsgHandler::OnMsgLogicSvrRegReq(NetPacket* pPacket)
{
	LogicRegToDbSvrReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	LogicRegToDbSvrAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_nConnID, MSG_LOGIC_REGTO_DBSVR_ACK, 0, 0, Ack);

	return TRUE;
}

