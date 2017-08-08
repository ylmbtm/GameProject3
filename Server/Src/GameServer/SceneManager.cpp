#include "stdafx.h"
#include "SceneManager.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_Game.pb.h"



CSceneManager::CSceneManager()
{

}

CSceneManager::~CSceneManager()
{
	SceneMap::iterator itor = m_mapSceneList.begin();
	for(; itor != m_mapSceneList.end(); ++itor)
	{
		CScene* pScene = itor->second;
		if(pScene != NULL)
		{
			pScene->Uninit();
			delete pScene;
			pScene = NULL;
		}
	}
}

BOOL CSceneManager::Init(BOOL bMainLand)
{
	m_MaxCopyBaseID = CGameService::GetInstancePtr()->GetServerID() << 24;

	if(bMainLand)
	{
		if(!LoadMainScene())
		{
			ASSERT_FAIELD;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CSceneManager::Uninit()
{
	return TRUE;
}

BOOL CSceneManager::CreateScene(UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCopyType, UINT32 dwPlayerNum)
{
	CScene* pScene = new CScene;

	if(!pScene->Init(dwCopyID, dwCopyGuid, dwCopyType, dwPlayerNum))
	{
		ASSERT_FAIELD;

		delete pScene;

		return FALSE;
	}

	m_mapSceneList.insert(std::make_pair(dwCopyGuid, pScene));

	return TRUE;
}

BOOL CSceneManager::DispatchPacket(NetPacket* pNetPacket)
{
	BOOL bHandled = TRUE;
	PacketHeader* pPacketHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);

	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_CREATE_SCENE_REQ,   OnMsgCreateSceneReq);
		default:
		{
			bHandled = FALSE;
		}
		break;
	}

	if(bHandled) //消息己经被处理
	{
		return TRUE;
	}

	CScene* pScene = GetSceneByCopyGuid(pPacketHeader->dwUserData);
	ERROR_RETURN_FALSE(pScene != NULL);
	pScene->DispatchPacket(pNetPacket);

	return TRUE;
}

CScene* CSceneManager::GetSceneByCopyGuid( UINT32 dwCopyGuid )
{
	SceneMap::iterator itor = m_mapSceneList.find(dwCopyGuid);
	if(itor != m_mapSceneList.end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CSceneManager::OnUpdate( UINT32 dwTick )
{
	for(SceneMap::iterator itor = m_mapSceneList.begin(); itor != m_mapSceneList.end();)
	{
		CScene* pScene = itor->second;

		pScene->OnUpdate(dwTick);

		if(pScene->IsFinished())
		{
			pScene->Uninit();
			delete pScene;
			itor = m_mapSceneList.erase(itor);
		}
		else
		{
			itor++;
		}
	}

	return TRUE;
}

UINT32 CSceneManager::MakeCopyID()
{
	m_MaxCopyBaseID += 1;

	return m_MaxCopyBaseID;
}

BOOL CSceneManager::SendCityReport()
{
	CopyReportReq Req;

	Req.set_serverid(CGameService::GetInstancePtr()->GetServerID());

	for(SceneMap::iterator itor = m_mapSceneList.begin(); itor != m_mapSceneList.end(); itor++)
	{
		CScene* pScene = itor->second;
		ERROR_RETURN_FALSE(pScene != NULL);

		if(pScene->GetCopyType() == CPT_CITY)
		{
			CopyItem* pItem = Req.add_copylist();
			pItem->set_copyguid(pScene->GetCopyGuid());
			pItem->set_copyid(pScene->GetCopyID());
			pItem->set_copytype(pScene->GetCopyType());
		}
	}

	return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_COPYINFO_REPORT_REQ, 0, 0, Req);
}

BOOL CSceneManager::OnMsgCreateSceneReq(NetPacket* pNetPacket)
{
	CreateNewSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	ERROR_RETURN_TRUE(Req.copyid() != 0);
	ERROR_RETURN_TRUE(Req.createparam() != 0);

	UINT32 dwNewCopyGuid = MakeCopyID();

	CreateNewSceneAck Ack;
	Ack.set_createparam(Req.createparam());
	Ack.set_copyguid(dwNewCopyGuid);
	Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
	Ack.set_copyid(Req.copyid());
	Ack.set_playernum(Req.playernum());
	Ack.set_copytype(Req.copytype());
	if (!CreateScene(Req.copyid(), dwNewCopyGuid, Req.copytype(), Req.playernum()))
	{
		ASSERT_FAIELD;

		Ack.set_retcode(MRC_FAILED);
	}
	else
	{
		Ack.set_retcode(MRC_SUCCESSED);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_CREATE_SCENE_ACK, 0, 0, Ack);

	return TRUE;
}

BOOL CSceneManager::LoadMainScene()
{
	if(!CreateScene(6, MakeCopyID(), 3, 0))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	return TRUE;
}
