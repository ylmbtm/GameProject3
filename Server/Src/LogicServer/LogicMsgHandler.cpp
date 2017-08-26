#include "stdafx.h"
#include "CommandDef.h"
#include "LogicMsgHandler.h"
#include "Log.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "CommonSocket.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Game_Define.pb.h"
#include "RoleModule.h"
#include "SimpleManager.h"
#include "../ServerData/ServerDefine.h"
#include "GlobalDataMgr.h"
#include "../ConfigData/ConfigData.h"
#include "CommonConvert.h"
#include "BagModule.h"

CWorldMsgHandler::CWorldMsgHandler()
{

}

CWorldMsgHandler::~CWorldMsgHandler()
{

}

BOOL CWorldMsgHandler::Init(UINT32 dwReserved)
{


	return TRUE;
}

BOOL CWorldMsgHandler::Uninit()
{
	return TRUE;
}

BOOL CWorldMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_SELECT_SERVER_REQ,		OnMsgSelectServerReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_REQ,			OnMsgRoleListReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LIST_ACK,			OnMsgRoleListAck);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_CREATE_REQ,		OnMsgRoleCreateReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_DELETE_REQ,		OnMsgRoleDeleteReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_REQ,		OnMsgRoleLoginReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_ACK,		OnMsgRoleLoginAck);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGOUT_REQ,		OnMsgRoleLogoutReq);
			PROCESS_MESSAGE_ITEM(MSG_DISCONNECT_NTY,		OnMsgRoleDisconnect);
			PROCESS_MESSAGE_ITEM(MSG_COPY_ABORT_REQ,		OnMsgAbortCopyReq);
			PROCESS_MESSAGE_ITEM(MSG_MAIN_COPY_REQ,			OnMsgMainCopyReq);
			PROCESS_MESSAGE_ITEM(MSG_BACK_TO_CITY_REQ,		OnMsgBackToCityReq);
			PROCESS_MESSAGE_ITEM(MSG_LOGIC_REGTO_LOGIN_ACK,	OnMsgRegToLoginAck);
			PROCESS_MESSAGE_ITEM(MSG_CHAT_MESSAGE_REQ,		OnMsgChatMessageReq);
		default:
		{
			PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

			CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
			ERROR_RETURN_FALSE(pPlayer != NULL);
			pPlayer->DispatchPacket(pNetPacket);
		}
		break;
	}

	return TRUE;
}



BOOL CWorldMsgHandler::OnMsgSelectServerReq(NetPacket* pNetPacket)
{
	SelectServerReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	SelectServerAck Ack;
	Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
	Ack.set_logincode(12345678);
	Ack.set_retcode(MRC_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_SELECT_SERVER_ACK, 0, pHeader->dwUserData, Ack);
}

BOOL CWorldMsgHandler::OnMsgRoleListReq(NetPacket* pNetPacket)
{
	RoleListReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	ERROR_RETURN_TRUE(Req.accountid() != 0);
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetDBConnID(),  MSG_ROLE_LIST_REQ, pNetPacket->m_dwConnID, pHeader->dwUserData, Req);
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleListAck(NetPacket* pNetPacket)
{
	RoleListAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf((UINT32)pHeader->u64TargetID,  MSG_ROLE_LIST_ACK, 0, pHeader->dwUserData, Ack);
}


BOOL CWorldMsgHandler::OnMsgRoleCreateReq(NetPacket* pNetPacket)
{
	RoleCreateReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	//检验名字是否可用
	ERROR_RETURN_TRUE(Req.accountid() != 0);

	UINT64 u64RoleID = CGlobalDataManager::GetInstancePtr()->MakeNewGuid();
	ERROR_RETURN_TRUE(u64RoleID != 0);

	CSimpleInfo* pSimpleInfo = CSimpleManager::GetInstancePtr()->CreateSimpleInfo(u64RoleID, Req.accountid(), Req.name(), Req.carrer());
	ERROR_RETURN_TRUE(pSimpleInfo != NULL);
	CPlayerObject* pPlayer  = CPlayerManager::GetInstancePtr()->CreatePlayer(u64RoleID);
	ERROR_RETURN_TRUE(pPlayer != NULL);
	ERROR_RETURN_TRUE(pPlayer->Init(u64RoleID));
	CRoleModule* pRoleModule = (CRoleModule*)pPlayer->GetModuleByType(MT_ROLE);
	pRoleModule->InitBaseData(u64RoleID, Req.name(), Req.carrer(), Req.accountid(), Req.channel());
	pPlayer->OnCreate(u64RoleID);

	RoleCreateAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	Ack.set_accountid(Req.accountid());
	Ack.set_roleid(u64RoleID);
	Ack.set_carrer(Req.carrer());
	Ack.set_name(Req.name());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID,  MSG_ROLE_CREATE_ACK, 0, pHeader->dwUserData, Ack);
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleDeleteReq(NetPacket* pNetPacket)
{
	RoleDeleteReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	//如果玩家在内存中，就在内存中删除，然后释放掉，如果玩家不在内存中，就向数据库发消息删除
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	if(pPlayer != NULL)
	{
		CRoleModule* pRoleModule = (CRoleModule*)pPlayer->GetModuleByType(MT_ROLE);
		ERROR_RETURN_TRUE(pRoleModule != NULL);
		pRoleModule->SetDelete(TRUE);
	}
	else
	{

	}

	RoleDeleteAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_ROLE_DELETE_REQ, pHeader->u64TargetID, 0, Ack);
}

BOOL CWorldMsgHandler::OnMsgRoleLoginReq(NetPacket* pNetPacket)
{
	RoleLoginReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	if(pPlayer == NULL)
	{
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetDBConnID(), MSG_ROLE_LOGIN_REQ, pNetPacket->m_dwConnID, pHeader->dwUserData, Req);
		return TRUE;
	}

	if((pPlayer->m_dwProxyConnID != 0) || (pPlayer->m_dwClientConnID != 0))
	{
		//表示玩家还没有退出，这个时候就是相当于挤的人情况发生了
		//如果在副本中，通知副本里人物离开
		//如果在场景中，通知场景中人物离开
		//清理副本状态
		//还需要通知玩家被人挤走了

		pPlayer->SendMsgRawData(MSG_ROLE_OTHER_LOGIN_NTY, NULL, 0);
		if(pPlayer->m_dwCopyGuid != 0)
		{
			//表示明确己登录到其它的副本
			pPlayer->SendLeaveScene(pPlayer->m_dwCopyGuid, pPlayer->m_dwCopySvrID);
		}
		else
		{
			CLog::GetInstancePtr()->LogError("玩家还没有主动登录副本,就又重新进入了");
			if(pPlayer->m_dwToCopyGuid != 0)
			{
				pPlayer->SendLeaveScene(pPlayer->m_dwToCopyGuid, pPlayer->m_dwToCopySvrID);
			}
		}
		pPlayer->SetConnectID(0, 0);
		pPlayer->ClearCopyState();
		pPlayer->OnLogout();
	}

	pPlayer->SetConnectID(pNetPacket->m_dwConnID, pHeader->dwUserData);
	pPlayer->OnLogin();
	pPlayer->OnAllModuleOK();
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleLoginAck(NetPacket* pNetPacket)
{
	DBRoleLoginAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->CreatePlayer(Ack.roleid());
	pPlayer->Init(Ack.roleid());
	pPlayer->SetConnectID((UINT32)pHeader->u64TargetID, pHeader->dwUserData);
	pPlayer->ReadFromDBLoginData(Ack);
	pPlayer->OnLogin();
	pPlayer->OnAllModuleOK();
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRoleLogoutReq(NetPacket* pNetPacket)
{
	RoleLogoutReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_TRUE(pPlayer != NULL);
	pPlayer->OnLogout();
	RoleLogoutAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	pPlayer->SendMsgProtoBuf(MSG_ROLE_LOGOUT_ACK, Ack);

	ERROR_RETURN_TRUE(pPlayer->m_dwCopyID != 0);
	ERROR_RETURN_TRUE(pPlayer->m_dwCopyGuid != 0);
	pPlayer->SendLeaveScene(pPlayer->m_dwCopyGuid, pPlayer->m_dwCopySvrID);

	pPlayer->SetConnectID(0, 0);
	pPlayer->ClearCopyState();

	return TRUE;
}



BOOL CWorldMsgHandler::OnMsgRoleDisconnect(NetPacket* pNetPacket)
{
	RoleDisconnectReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	ERROR_RETURN_TRUE(pPlayer != NULL);
	if(pPlayer->m_dwProxyConnID == 0)
	{
		return TRUE;
	}

	if((pPlayer->m_dwCopyGuid != 0) && (pPlayer->m_dwCopySvrID != 0))
	{
		pPlayer->SendLeaveScene(pPlayer->m_dwCopyGuid, pPlayer->m_dwCopySvrID);
	}

	pPlayer->OnLogout();
	pPlayer->SetConnectID(0, 0);
	pPlayer->ClearCopyState();

	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgMainCopyReq(NetPacket* pNetPacket)
{
	MainCopyReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_TRUE(pPlayer != NULL);
	ERROR_RETURN_TRUE(pPlayer->m_dwToCopyID == 0);
	ERROR_RETURN_TRUE(Req.copyid() != 0);

	StCopyInfo* pCopyInfo = CConfigData::GetInstancePtr()->GetCopyInfo(Req.copyid());
	ERROR_RETURN_TRUE(pCopyInfo != NULL);

	UINT32 dwRetCode = pPlayer->CheckCopyConditoin(Req.copyid());
	if(dwRetCode != MRC_SUCCESSED)
	{
		MainCopyAck Ack;
		Ack.set_retcode(dwRetCode);
		pPlayer->SendMsgProtoBuf(MSG_MAIN_COPY_ACK, Ack);
	}

	ERROR_RETURN_TRUE(CGameSvrMgr::GetInstancePtr()->CreateScene(Req.copyid(), pHeader->u64TargetID, 1, pCopyInfo->dwCopyType));
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgAbortCopyReq(NetPacket* pNetPacket)
{
	AbortCopyReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_TRUE(pPlayer != NULL);
	ERROR_RETURN_TRUE(pPlayer->m_dwCopyID == Req.copyid());
	ERROR_RETURN_TRUE(pPlayer->m_dwCopyGuid == Req.copyguid());
	ERROR_RETURN_TRUE(pPlayer->m_dwToCopyID == 0);
	ERROR_RETURN_TRUE(pPlayer->m_dwToCopyGuid == 0);
	pPlayer->SendLeaveScene(pPlayer->m_dwCopyGuid, pPlayer->m_dwCopySvrID);

	CGameSvrMgr::GetInstancePtr()->SendPlayerToMainCity(pHeader->u64TargetID, pPlayer->GetCityCopyID());

	pPlayer->m_dwCopyID = 0;
	pPlayer->m_dwCopyGuid = 0;
	pPlayer->m_dwCopySvrID = 0;
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgBackToCityReq( NetPacket* pNetPacket )
{
	BackToCityReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_TRUE(pPlayer != NULL);
	ERROR_RETURN_TRUE(pPlayer->m_dwToCopyID == 0);
	ERROR_RETURN_TRUE(pPlayer->m_dwToCopyGuid == 0);
	pPlayer->SendLeaveScene(pPlayer->m_dwCopyGuid, pPlayer->m_dwCopySvrID);

	CGameSvrMgr::GetInstancePtr()->SendPlayerToMainCity(pHeader->u64TargetID, pPlayer->GetCityCopyID());
	pPlayer->m_dwCopyID = 0;
	pPlayer->m_dwCopyGuid = 0;
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgRegToLoginAck(NetPacket* pNetPacket)
{
	return TRUE;
}

BOOL CWorldMsgHandler::OnMsgChatMessageReq(NetPacket* pNetPacket)
{
	ChatMessageReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

	if((Req.content().size() > 2) && (Req.content().at(0) == '@') && (Req.content().at(1) == '@'))
	{
		std::vector<std::string> vtParam;
		CommonConvert::SpliteString(Req.content(), " ", vtParam);
		ProcessGameCommand(pHeader->u64TargetID, vtParam);
	}
	else
	{
		switch(Req.channel())
		{
			case CHL_WORLD:
			{

			}
			break;
			case CHL_PRIVATE:
			{

			}
			break;
			case CHL_GUILD:
			{

			}
			break;
			default:
				break;
		}
	}


	return TRUE;
}

BOOL CWorldMsgHandler::ProcessGameCommand(UINT64 u64ID, std::vector<std::string>& vtParam)
{
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(u64ID);
	ERROR_RETURN_TRUE(pPlayer != NULL);
	ERROR_RETURN_TRUE(vtParam.size() >= 1);

	if(std::strcmp(vtParam[0].c_str(), "@@additem") == 0)
	{
		ERROR_RETURN_TRUE(vtParam.size() >= 3);
		CBagModule* pBag = (CBagModule*)pPlayer->GetModuleByType(MT_BAG);
		ERROR_RETURN_TRUE(pBag != NULL);
		pBag->AddItem(CommonConvert::StringToInt(vtParam[1].c_str()), CommonConvert::StringToInt(vtParam[2].c_str()));
	}
	else if(vtParam[0].compare("xxxx") == 0)
	{

	}

	return TRUE;
}
