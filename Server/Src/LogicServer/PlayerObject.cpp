#include "stdafx.h"
#include "PlayerObject.h"
#include "PacketHeader.h"
#include "PlayerManager.h"
#include "RoleModule.h"
#include "..\Message\Msg_Login.pb.h"
#include "..\Message\Msg_ID.pb.h"
#include "GameSvrMgr.h"
#include "..\GameServer\GameService.h"
#include "..\Message\Msg_RetCode.pb.h"

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

	CreateAllModule();

	return TRUE;
}

BOOL CPlayerObject::Uninit()
{
	DestroyAllModule();

	return TRUE;
}


BOOL CPlayerObject::OnCreate(UINT64 u64RoleID)
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnCreate(u64RoleID);
	}
	return TRUE;
}

BOOL CPlayerObject::OnDestroy(UINT64 u64RoleID)
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnDestroy(u64RoleID);
	}
	return TRUE;
}

BOOL CPlayerObject::OnLogin(UINT64 u64RoleID)
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnLogin(u64RoleID);
	}
	return TRUE;
}

BOOL CPlayerObject::OnLogout(UINT64 u64RoleID)
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnLogout(u64RoleID);
	}
	return TRUE;
}

BOOL CPlayerObject::OnNewDay()
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnNewDay();
	}
	return TRUE;
}

BOOL CPlayerObject::OnLoadData(UINT64 u64RoleID)
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
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
			break;
		}
		break;
	}

	return TRUE;
}

BOOL CPlayerObject::CreateAllModule()
{
	m_MoudleList.push_back(new CRoleModule(this));

	return TRUE;
}

BOOL CPlayerObject::DestroyAllModule()
{
	for(int i = MT_ROLE+1; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnDestroy(m_u64ID);
		delete pBase;
	}

	m_MoudleList.clear();
	return TRUE;
}

BOOL CPlayerObject::SendProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata)
{
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwProxyConnID, dwMsgID, GetObjectID(), m_dwClientConnID, pdata);
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

		CRoleModule *pModule = (CRoleModule *)GetModuleByType(MT_ROLE);
		RoleLoginAck Ack;

		SendProtoBuf(MSG_ROLE_LOGIN_ACK, Ack);

		TransRoleDataReq Req;
		Req.set_roleid(m_u64ID);
		//Req.set_roletype(pModule->m_RoleType);
		//Req.set_level(pModule->m_dwLevel);
		//Req.set_rolename(pModule->m_strName);

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
	RoleLoginAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwProxyConnID, MSG_ROLE_LOGIN_ACK, 0, m_dwClientConnID, Ack);

	SendToScene(1,1);

	return TRUE;
}


BOOL CPlayerObject::SendToScene(UINT32 dwCopyID,UINT32 dwSvrID)
{
	TransRoleDataReq Req;
	Req.set_roleid(m_u64ID);
	UINT32 dwConnID = CGameSvrMgr::GetInstancePtr()->GetConnIDBySvrID(dwSvrID);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_TRANS_ROLE_DATA_REQ, m_u64ID, 0, Req);

	return TRUE;
}

BOOL CPlayerObject::SetConnectID(UINT32 dwProxyID, UINT32 dwClientID)
{
	m_dwProxyConnID = dwProxyID;
	m_dwClientConnID = dwClientID;

	return TRUE;
}

BOOL CPlayerObject::SetAllModuleOK()
{
	for(std::vector<CModuleBase*>::iterator itor = m_MoudleList.begin(); itor != m_MoudleList.end(); itor++)
	{
		CModuleBase *pBase = *itor;

		pBase->m_bIsDataOK = TRUE;
	}

	return TRUE;
}

CModuleBase* CPlayerObject::GetModuleByType(MouduleType MType)
{
	if(MType >= m_MoudleList.size())
	{
		return NULL;
	}

	return m_MoudleList.at(MType);
}

UINT64 CPlayerObject::GetObjectID()
{
	return m_u64ID;
}


