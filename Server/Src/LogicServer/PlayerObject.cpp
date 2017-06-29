#include "stdafx.h"
#include "PlayerObject.h"
#include "PacketHeader.h"
#include "PlayerManager.h"
#include "RoleModule.h"
#include "..\Message\Msg_Login.pb.h"
#include "..\Message\Msg_ID.pb.h"
#include "GameSvrMgr.h"
#include "..\GameServer\GameService.h"

CPlayerObject::CPlayerObject()
{
	
}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::Init()
{
	m_u64ID = 0;

	m_dwProxyConnID = 0;

	return TRUE;
}

BOOL CPlayerObject::Uninit()
{

	return TRUE;
}


BOOL CPlayerObject::OnCreate(UINT64 u64RoleID)
{
	for(std::vector<CModuleBase*>::iterator itor = m_MoudleList.begin(); itor != m_MoudleList.end(); itor++)
	{
		CModuleBase *pBase = *itor;
		pBase->OnCreate(u64RoleID);
	}
	return TRUE;
}

BOOL CPlayerObject::OnDestroy(UINT64 u64RoleID)
{
	for(std::vector<CModuleBase*>::iterator itor = m_MoudleList.begin(); itor != m_MoudleList.end(); itor++)
	{
		CModuleBase *pBase = *itor;
		pBase->OnDestroy(u64RoleID);
	}
	return TRUE;
}

BOOL CPlayerObject::OnLogin(UINT64 u64RoleID)
{
	for(std::vector<CModuleBase*>::iterator itor = m_MoudleList.begin(); itor != m_MoudleList.end(); itor++)
	{
		CModuleBase *pBase = *itor;
		pBase->OnLogin(u64RoleID);
	}
	return TRUE;
}

BOOL CPlayerObject::OnLogout(UINT64 u64RoleID)
{
	for(std::vector<CModuleBase*>::iterator itor = m_MoudleList.begin(); itor != m_MoudleList.end(); itor++)
	{
		CModuleBase *pBase = *itor;
		pBase->OnLogout(u64RoleID);
	}
	return TRUE;
}

BOOL CPlayerObject::OnNewDay()
{
	for(std::vector<CModuleBase*>::iterator itor = m_MoudleList.begin(); itor != m_MoudleList.end(); itor++)
	{
		CModuleBase *pBase = *itor;
		pBase->OnNewDay();
	}
	return TRUE;
}

BOOL CPlayerObject::OnLoadData(UINT64 u64RoleID)
{
	for(std::vector<CModuleBase*>::iterator itor = m_MoudleList.begin(); itor != m_MoudleList.end(); itor++)
	{
		CModuleBase *pBase = *itor;
		pBase->OnLoadData(u64RoleID);
	}

	return TRUE;
}

BOOL CPlayerObject::DispatchPacket(NetPacket *pNetPack)
{
	switch(pNetPack->m_dwMsgID)
	{
	default:
		{
			PacketHeader* pHeader = (PacketHeader*)pNetPack->m_pDataBuffer->GetBuffer();

			CPlayerObject *pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);

			pPlayer->DispatchPacket(pNetPack);
		}
		break;
	}

	return TRUE;
}

BOOL CPlayerObject::CreateAllModule()
{
	m_MoudleList.resize(MT_END);
	m_MoudleList[MT_ROLE] = new CRoleModule();

	return TRUE;
}

BOOL CPlayerObject::DestroyAllModule()
{
	return TRUE;
}

BOOL CPlayerObject::SendProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata)
{
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwProxyConnID, dwMsgID, GetObjectID(), 0, pdata);
}


BOOL CPlayerObject::OnModuleFnished()
{
	if(IsAllModuleOK())
	{
		///发送玩家登录返回的消息
		///将玩家的数据传到主场景
		///收到通知进入的消息
		///通知客户端进入
		///客户端发进入副本的请求
	    ///发玩家自己的数据给玩家
		///发视野里的玩家给玩家

		CRoleModule *pModule = GetModuleByType<CRoleModule>(MT_ROLE);
		RoleLoginAck Ack;

		SendProtoBuf(MSG_ROLE_LOGIN_ACK, Ack);

		TransRoleDataReq Req;
		Req.set_roleid(m_u64ID);
		Req.set_roletype(pModule->m_RoleType);
		Req.set_level(pModule->m_dwLevel);
		Req.set_rolename(pModule->m_strName);

		UINT32 dwSvrID, dwConnID, dwCopyID;

		CGameSvrMgr::GetInstancePtr()->GetMainScene(dwSvrID, dwConnID, dwCopyID);
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_TRANS_ROLE_DATA_REQ, 0, dwCopyID, Req);
	}

	return TRUE;
}

BOOL CPlayerObject::IsAllModuleOK()
{
	for(std::vector<CModuleBase*>::iterator itor = m_MoudleList.begin(); itor != m_MoudleList.end(); itor++)
	{
		CModuleBase *pBase = *itor;
		if(pBase->IsDataOK() == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPlayerObject::OnAllModuleOK()
{



	return TRUE;
}


UINT64 CPlayerObject::GetObjectID()
{
	return m_u64ID;
}


