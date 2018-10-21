#include "stdafx.h"
#include "LogicMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "PlayerObject.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Game_Define.pb.h"
#include "RoleModule.h"
#include "SimpleManager.h"
#include "../ServerData/ServerDefine.h"
#include "GlobalDataMgr.h"
#include "../ConfigData/ConfigData.h"
#include "BagModule.h"
#include "../ServerData/RoleData.h"
#include "PartnerModule.h"

CLogicMsgHandler::CLogicMsgHandler()
{

}

CLogicMsgHandler::~CLogicMsgHandler()
{

}

BOOL CLogicMsgHandler::Init(UINT32 dwReserved)
{


	return TRUE;
}

BOOL CLogicMsgHandler::Uninit()
{
	return TRUE;
}

BOOL CLogicMsgHandler::OnUpdate(UINT64 uTick)
{
	CPlayerManager::GetInstancePtr()->TryCleanPlayer();

	if (CPlayerManager::GetInstancePtr()->GetCount() > 0)
	{
		CPlayerManager::TNodeTypePtr pNode = CPlayerManager::GetInstancePtr()->MoveFirst();
		ERROR_RETURN_FALSE(pNode != NULL);

		CPlayerObject* pTempObj = NULL;
		for (; pNode != NULL; pNode = CPlayerManager::GetInstancePtr()->MoveNext(pNode))
		{
			pTempObj = pNode->GetValue();
			ERROR_RETURN_FALSE(pTempObj != NULL);

			if (pTempObj->IsOnline())
			{
				pTempObj->NotifyChange();
			}
		}
	}

	return TRUE;
}

BOOL CLogicMsgHandler::OnSecondTimer()
{
	return TRUE;
}

BOOL CLogicMsgHandler::DispatchPacket(NetPacket* pNetPacket)
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
			PROCESS_MESSAGE_ITEM(MSG_ROLE_RECONNECT_REQ,	OnMsgReconnectReq);
			PROCESS_MESSAGE_ITEM(MSG_TEST_ADD_ITEM,			OnMsgTestAddItemReq);
	}


	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
	ERROR_RETURN_FALSE(pPlayer != NULL);
	if (pPlayer->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return FALSE;
}



BOOL CLogicMsgHandler::OnMsgSelectServerReq(NetPacket* pNetPacket)
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

BOOL CLogicMsgHandler::OnMsgRoleListReq(NetPacket* pNetPacket)
{
	RoleListReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	ERROR_RETURN_TRUE(Req.accountid() != 0);
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetDBConnID(),  MSG_ROLE_LIST_REQ, pNetPacket->m_dwConnID, pHeader->dwUserData, Req);
	return TRUE;
}

BOOL CLogicMsgHandler::OnMsgRoleListAck(NetPacket* pNetPacket)
{
	RoleListAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);

	//可能有新创建的角色还没有写回数据库
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf((UINT32)pHeader->u64TargetID,  MSG_ROLE_LIST_ACK, 0, pHeader->dwUserData, Ack);
}


BOOL CLogicMsgHandler::OnMsgRoleCreateReq(NetPacket* pNetPacket)
{
	RoleCreateReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	//检验名字是否可用
	ERROR_RETURN_TRUE(Req.accountid() != 0);
	ERROR_RETURN_TRUE(Req.carrer() != 0);

	RoleCreateAck Ack;

	Ack.set_accountid(Req.accountid());
	Ack.set_roleid(0);
	Ack.set_carrer(Req.carrer());
	Ack.set_name(Req.name());

	if (CSimpleManager::GetInstancePtr()->CheckNameExist(Req.name()))
	{
		Ack.set_retcode(MRC_NAME_EXIST);
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_ROLE_CREATE_ACK, 0, pHeader->dwUserData, Ack);
		return TRUE;
	}

	StCarrerInfo* pCarrerInfo = CConfigData::GetInstancePtr()->GetCarrerInfo(Req.carrer());
	if (pCarrerInfo == NULL)
	{
		Ack.set_retcode(MRC_INVALID_CARRERID);
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_ROLE_CREATE_ACK, 0, pHeader->dwUserData, Ack);
		return TRUE;
	}

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

	Ack.set_retcode(MRC_SUCCESSED);
	Ack.set_roleid(u64RoleID);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID,  MSG_ROLE_CREATE_ACK, 0, pHeader->dwUserData, Ack);
	return TRUE;
}

BOOL CLogicMsgHandler::OnMsgRoleDeleteReq(NetPacket* pNetPacket)
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

BOOL CLogicMsgHandler::OnMsgRoleLoginReq(NetPacket* pNetPacket)
{
	RoleLoginReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	ERROR_RETURN_TRUE(Req.roleid() != 0);
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

BOOL CLogicMsgHandler::OnMsgRoleLoginAck(NetPacket* pNetPacket)
{
	DBRoleLoginAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	ERROR_RETURN_TRUE(Ack.roleid() != 0);
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->CreatePlayer(Ack.roleid());
	pPlayer->Init(Ack.roleid());
	pPlayer->SetConnectID((UINT32)pHeader->u64TargetID, pHeader->dwUserData);
	pPlayer->ReadFromDBLoginData(Ack);
	pPlayer->OnLogin();
	pPlayer->OnAllModuleOK();
	return TRUE;
}

BOOL CLogicMsgHandler::OnMsgRoleLogoutReq(NetPacket* pNetPacket)
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



BOOL CLogicMsgHandler::OnMsgRoleDisconnect(NetPacket* pNetPacket)
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

BOOL CLogicMsgHandler::OnMsgMainCopyReq(NetPacket* pNetPacket)
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

BOOL CLogicMsgHandler::OnMsgAbortCopyReq(NetPacket* pNetPacket)
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

BOOL CLogicMsgHandler::OnMsgBackToCityReq( NetPacket* pNetPacket )
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

BOOL CLogicMsgHandler::OnMsgRegToLoginAck(NetPacket* pNetPacket)
{
	LogicRegToLoginAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	return TRUE;
}

BOOL CLogicMsgHandler::OnMsgChatMessageReq(NetPacket* pNetPacket)
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

BOOL CLogicMsgHandler::ProcessGameCommand(UINT64 u64ID, std::vector<std::string>& vtParam)
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
	else if(vtParam[0].compare("@@AddPartner") == 0)
	{
		CPartnerModule *pPartnerModule = (CPartnerModule*)pPlayer->GetModuleByType(MT_PARTNER);
		ERROR_RETURN_TRUE(pPartnerModule != NULL);
		
		for (int i = 0; i < 20; i++)
		{
			pPartnerModule->AddPartner(i + 1);
		}
	}

	return TRUE;
}


BOOL CLogicMsgHandler::OnMsgTestAddItemReq(NetPacket* pNetPacket)
{
	ChatMessageReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.srcid());
	ERROR_RETURN_TRUE(pPlayer != NULL);
	CBagModule* pBag = (CBagModule*)pPlayer->GetModuleByType(MT_BAG);
	ERROR_RETURN_TRUE(pBag != NULL);
	//pBag->AddItem(Req.guildid(), 1);
	return TRUE;
}

BOOL CLogicMsgHandler::OnMsgReconnectReq( NetPacket* pNetPacket )
{
	RoleReconnectReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
	if(pPlayer == NULL)
	{
		//重连失败，请重新登录
		return TRUE;
	}

	if((pPlayer->m_dwProxyConnID != 0) || (pPlayer->m_dwClientConnID != 0))
	{
		//当重连发生的时候，理论上这个连接己经断开了。这两个值应该为空了，
		//如果此时这两个值为空，那么，还会有其它的情况发生。
		//此时应该直接选设置这两个值
		//由于副本己经被退出， 所以此时应该将玩家放到主城
		CLog::GetInstancePtr()->LogError("OnMsgReconnectReq 断开消都还没有收到， 重连的消息就到了");
	}

	pPlayer->SetConnectID(pNetPacket->m_dwConnID, pHeader->dwUserData);

	CGameSvrMgr::GetInstancePtr()->SendPlayerToMainCity(pPlayer->GetObjectID(), pPlayer->GetCityCopyID());

	pPlayer->m_dwCopyID = 0;

	pPlayer->m_dwCopyGuid = 0;

	return TRUE;
}
