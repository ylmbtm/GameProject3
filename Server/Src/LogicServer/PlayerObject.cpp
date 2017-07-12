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
#include "Utility\Log\Log.h"

CPlayerObject::CPlayerObject()
{
	
}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::Init(UINT64 u64ID)
{
	m_u64ID = u64ID;
	m_dwProxyConnID = 0;
    m_dwClientConnID = 0;

	return CreateAllModule();
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

BOOL CPlayerObject::OnDestroy()
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnDestroy();
	}
	return TRUE;
}

BOOL CPlayerObject::OnLogin()
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnLogin();
	}
	return TRUE;
}

BOOL CPlayerObject::OnLogout()
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->OnLogout();
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
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
        if(pBase == NULL)
        {
            LOG_ERROR;
            continue;
        }

		pBase->OnDestroy();
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
		CLog::GetInstancePtr()->AddLog("===========OnAllModuleFinished=============");
        OnAllModuleOK();
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
	///发送玩家登录返回的消息
	///将玩家的数据传到主场景
	///收到通知进入的消息
	///通知客户端进入
	///客户端发进入副本的请求
	///发玩家自己的数据给玩家
	///发视野里的玩家给玩家
	ERROR_RETURN_FALSE(m_u64ID != 0);
    SendRoleLoginAck();
    UINT32 dwSvrID, dwConnID, dwCopyID;
    CGameSvrMgr::GetInstancePtr()->GetMainScene(dwSvrID, dwConnID, dwCopyID);
	ERROR_RETURN_FALSE(dwSvrID == 1);
	ERROR_RETURN_FALSE(dwConnID != 0);
	ERROR_RETURN_FALSE(dwCopyID != 0);
    SendToScene(dwCopyID, dwConnID);
	return TRUE;
}

BOOL CPlayerObject::SendToScene(UINT32 dwCopyID, UINT32 dwConnID)
{
	ERROR_RETURN_FALSE(dwCopyID != 0);
	ERROR_RETURN_FALSE(dwConnID != 0);
    CRoleModule *pModule = (CRoleModule *)GetModuleByType(MT_ROLE);
    TransRoleDataReq Req;
    Req.set_roleid(m_u64ID);
    Req.set_roletype(pModule->m_pRoleDataObject->m_RoleType);
    Req.set_level(pModule->m_pRoleDataObject->m_dwLevel);
    Req.set_rolename(pModule->m_pRoleDataObject->m_szName);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_TRANS_ROLE_DATA_REQ, m_u64ID, dwCopyID, Req);
	return TRUE;
}

BOOL CPlayerObject::SendNotifyIntoScene(UINT32 dwCopyID, UINT32 dwCopyType, UINT32 dwSvrID)
{
	ERROR_RETURN_FALSE(dwCopyID != 0);
	ERROR_RETURN_FALSE(dwCopyType != 0);
	ERROR_RETURN_FALSE(dwSvrID != 0);

	NotifyIntoScene Nty;
	Nty.set_copytype(dwCopyType);
	Nty.set_copyid(dwCopyID);
	Nty.set_serverid(dwSvrID);
	Nty.set_roleid(m_u64ID);
	ERROR_RETURN_FALSE(m_u64ID != 0);
	ERROR_RETURN_FALSE(SendProtoBuf(MSG_NOTIFY_INTO_SCENE, Nty));
	return TRUE;
}

BOOL CPlayerObject::SendLeaveScene(UINT32 dwCopyID, UINT32 dwSvrID)
{
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
        if(pBase == NULL)
        {
            LOG_ERROR;
            continue;
        }

		pBase->m_bIsDataOK = TRUE;
	}

	return TRUE;
}

CModuleBase* CPlayerObject::GetModuleByType(MouduleType MType)
{
	if(MType >= m_MoudleList.size())
	{
        LOG_ERROR
		return NULL;
	}

	return m_MoudleList.at(MType);
}

UINT64 CPlayerObject::GetObjectID()
{
	return m_u64ID;
}

BOOL CPlayerObject::SendRoleLoginAck()
{
    CRoleModule *pModule = (CRoleModule *)GetModuleByType(MT_ROLE);
    RoleLoginAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);
    Ack.set_accountid(pModule->m_pRoleDataObject->m_u64AccountID);
    Ack.set_roleid(m_u64ID);
    Ack.set_name(pModule->m_pRoleDataObject->m_szName);
    Ack.set_level(1);
    Ack.set_roletype(pModule->m_pRoleDataObject->m_RoleType);
    SendProtoBuf(MSG_ROLE_LOGIN_ACK, Ack);
    return TRUE;
}


