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
#include "..\ServerData\RoleData.h"
#include "CopyModule.h"
#include "BagModule.h"
#include "EquipModule.h"
#include "PetModule.h"
#include "..\ServerData\ServerDefine.h"
#include "..\ConfigData\ConfigData.h"
#include "..\ServerData\CopyData.h"

CPlayerObject::CPlayerObject()
{
	
}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::Init(UINT64 u64ID)
{
	m_u64ID             = u64ID;
	m_dwProxyConnID     = 0;
    m_dwClientConnID    = 0;
    m_dwCopyGuid          = 0;      //当前的副本ID
    m_dwCopyID			= 0;        //当前的副本类型
    m_dwCopySvrID       = 0;        //副本服务器的ID
    m_dwToCopyGuid      = 0;        //正在前往的副本ID
    m_dwToCopyID       = 0;         //正在前往的副本ID
     

	return CreateAllModule();
}

BOOL CPlayerObject::Uninit()
{
	DestroyAllModule();
    m_u64ID             = 0;
    m_dwProxyConnID     = 0;
    m_dwClientConnID    = 0;
    m_dwCopyGuid        = 0;        //当前的副本ID
    m_dwCopyID          = 0;        //当前的副本类型
    m_dwCopySvrID       = 0;        //副本服务器的ID
    m_dwToCopyGuid      = 0;        //正在前往的副本ID
    m_dwToCopyID        = 0;        //正在前往的副本类型

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

BOOL CPlayerObject::ReadFromLoginAck(DBRoleLoginAck &Ack)
{
	for(int i = MT_ROLE; i< MT_END; i++)
	{
		CModuleBase *pBase = m_MoudleList.at(i);
		pBase->ReadFromLoginAck(Ack);
	}

	return TRUE;
}

BOOL CPlayerObject::DispatchPacket(NetPacket *pNetPack)
{
	switch(pNetPack->m_dwMsgID)
	{
	default:
		{
			for(int i = MT_ROLE; i< MT_END; i++)
			{
				CModuleBase *pBase = m_MoudleList.at(i);
				if(pBase == NULL)
				{
					LOG_ERROR;
					continue;
				}

				if(pBase->DispatchPacket(pNetPack))
				{
					return TRUE;
				}
			}
		}
		break;
	}

	return TRUE;
}

BOOL CPlayerObject::CreateAllModule()
{
	m_MoudleList.assign(MT_END, NULL);
	m_MoudleList[MT_ROLE] = new CRoleModule(this);
	m_MoudleList[MT_COPY] = new CCopyModule(this);
	m_MoudleList[MT_BAG] = new CBagModule(this);
	m_MoudleList[MT_EQUIP] = new CEquipModule(this);
	m_MoudleList[MT_PET] = new CPetModule(this);
	

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

BOOL CPlayerObject::SendMsgProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata)
{
	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwProxyConnID, dwMsgID, GetObjectID(), m_dwClientConnID, pdata);
}

BOOL CPlayerObject::SendMsgRawData(UINT32 dwMsgID, const char * pdata,UINT32 dwLen)
{
	return ServiceBase::GetInstancePtr()->SendMsgRawData(m_dwProxyConnID, dwMsgID, GetObjectID(), m_dwClientConnID, pdata, dwLen);
}



BOOL CPlayerObject::OnAllModuleOK()
{
	ERROR_RETURN_FALSE(m_u64ID != 0);
    SendRoleLoginAck();

    CGameSvrMgr::GetInstancePtr()->SendPlayerToMainCity(m_u64ID, GetCityCopyID());
	m_dwCopyID = 0;
	m_dwCopyGuid = 0;
	return TRUE;
}

UINT32 CPlayerObject::CheckCopyConditoin(UINT32 dwCopyID)
{
	return TRUE;
	StCopyInfo *pCopyInfo = CConfigData::GetInstancePtr()->GetCopyInfo(dwCopyID);
	ERROR_RETURN_CODE(m_u64ID != 0, MRC_INVALID_COPYID);

	CRoleModule *pRoleModule = (CRoleModule*)GetModuleByType(MT_ROLE);
	ERROR_RETURN_CODE(pRoleModule != NULL, MRC_FAILED);

	if(!pRoleModule->CheckActionEnough(pCopyInfo->dwCostActID, pCopyInfo->dwCostActNum))
	{
		return MRC_NOT_ENOUGH_ACTOIN;
	}

	CCopyModule *pCopyModule = (CCopyModule*)GetModuleByType(MT_COPY);
	ERROR_RETURN_CODE(pCopyModule != NULL, MRC_FAILED);

	CopyDataObject *pObject = pCopyModule->GetCopyData(dwCopyID);
	ERROR_RETURN_CODE(pObject != NULL, MRC_FAILED);

	if(pObject->m_dwBattleTimes >= pCopyInfo->dwBattleTimes)
	{
		return MRC_NOT_ENOUGH_TIMES;
	}

	return MRC_SUCCESSED;
}

BOOL CPlayerObject::SendIntoSceneNotify(UINT32 dwCopyGuid, UINT32 dwCopyID, UINT32 dwSvrID)
{
	ERROR_RETURN_FALSE(dwCopyID != 0);
	ERROR_RETURN_FALSE(dwCopyGuid != 0);
	ERROR_RETURN_FALSE(dwSvrID != 0);

    ERROR_RETURN_FALSE(m_dwCopyGuid != dwCopyGuid);
    ERROR_RETURN_FALSE(m_dwCopyID != dwCopyID);

	NotifyIntoScene Nty;
	Nty.set_copyid(dwCopyID);
	Nty.set_copyguid(dwCopyGuid);
	Nty.set_serverid(dwSvrID);
	Nty.set_roleid(m_u64ID);
	ERROR_RETURN_FALSE(m_u64ID != 0);
	SendMsgProtoBuf(MSG_NOTIFY_INTO_SCENE, Nty);
	return TRUE;
}

BOOL CPlayerObject::SendLeaveScene(UINT32 dwCopyGuid, UINT32 dwSvrID)
{
    LeaveSceneReq LeaveReq;
    LeaveReq.set_roleid(m_u64ID);
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameSvrMgr::GetInstancePtr()->GetConnIDBySvrID(dwSvrID), MSG_LEAVE_SCENE_REQ, m_u64ID, dwCopyGuid, LeaveReq);
	return TRUE;
}

BOOL CPlayerObject::SetConnectID(UINT32 dwProxyID, UINT32 dwClientID)
{
	m_dwProxyConnID = dwProxyID;
	m_dwClientConnID = dwClientID;

	return TRUE;
}


CModuleBase* CPlayerObject::GetModuleByType(UINT32 dwModuleType)
{
	if(dwModuleType >= (UINT32)m_MoudleList.size())
	{
        LOG_ERROR;
		return NULL;
	}

	return m_MoudleList.at(dwModuleType);
}

UINT64 CPlayerObject::GetObjectID()
{
	return m_u64ID;
}

UINT32 CPlayerObject::GetCityCopyID()
{
	CRoleModule *pModule = (CRoleModule*)GetModuleByType(MT_ROLE);
	ERROR_RETURN_FALSE(pModule != NULL);
	ERROR_RETURN_FALSE(pModule->m_pRoleDataObject != NULL);
	//return pModule->m_pRoleDataObject->m_CityCopyID;
	return 6;
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
    Ack.set_actorid(pModule->m_pRoleDataObject->m_ActorID);
    SendMsgProtoBuf(MSG_ROLE_LOGIN_ACK, Ack);
    return TRUE;
}

BOOL CPlayerObject::ToTransRoleData( TransRoleDataReq &Req )
{
    CRoleModule *pModule = (CRoleModule *)GetModuleByType(MT_ROLE);
    Req.mutable_roledata()->set_roleid(m_u64ID);
    Req.mutable_roledata()->set_actorid(pModule->m_pRoleDataObject->m_ActorID);
    Req.mutable_roledata()->set_level(pModule->m_pRoleDataObject->m_Level);
    Req.mutable_roledata()->set_rolename(pModule->m_pRoleDataObject->m_szName);
	
    return TRUE;
}

BOOL CPlayerObject::ClearCopyState()
{
    m_dwCopyGuid    = 0;        //当前的副本ID
    m_dwCopyID		= 0;        //当前的副本类型
    m_dwCopySvrID   = 0;        //副本服务器的ID
    m_dwToCopyGuid  = 0;        //正在前往的副本ID
    m_dwToCopyID    = 0;        //正在前往的副本类型

    return TRUE;
}


