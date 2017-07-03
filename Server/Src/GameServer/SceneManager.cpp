#include "stdafx.h"
#include "SceneManager.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"
#include "../Message/Msg_RetCode.pb.h"



CSceneManager::CSceneManager()
{

}

CSceneManager::~CSceneManager()
{
	SceneMap::iterator itor = m_mapSceneList.begin();
	for(; itor != m_mapSceneList.end(); ++itor)
	{
		CScene *pScene = itor->second;
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
	if(bMainLand)
	{
		if(!LoadMainScene())
		{
			ASSERT_FAIELD;
			return FALSE;
		}
	}


	m_MaxCopyBaseID = 1;

	return TRUE;
}

BOOL CSceneManager::Uninit()
{
	return TRUE;
}

BOOL CSceneManager::CreateScene(UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwLogicType)
{
	CScene *pScene = new CScene;

	if(!pScene->Init(dwCopyType, dwCopyID, dwLogicType))
	{
		ASSERT_FAIELD;

		delete pScene;

		return FALSE;
	}

	m_mapSceneList.insert(std::make_pair(dwCopyID, pScene));

	return TRUE;
}

BOOL CSceneManager::DispatchPacket(NetPacket *pNetPacket)
{
	BOOL bHandled = TRUE;
	PacketHeader *pPacketHeader = (PacketHeader *)pNetPacket->m_pDataBuffer->GetBuffer();
	if(pPacketHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

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

	CScene *pScene = GetSceneByID(pPacketHeader->dwUserData);
	if(pScene == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	pScene->DispatchPacket(pNetPacket);
		
	return TRUE;
}

CScene* CSceneManager::GetSceneByID( UINT32 dwSceneID )
{
	SceneMap::iterator itor = m_mapSceneList.find(dwSceneID);
	if(itor != m_mapSceneList.end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CSceneManager::OnUpdate( UINT32 dwTick )
{
	for(SceneMap::iterator itor = m_mapSceneList.begin(); itor != m_mapSceneList.end(); ++itor)
	{
		CScene *pScene = itor->second;

		pScene->OnUpdate(dwTick);
	}

	return TRUE;
}

UINT32 CSceneManager::MakeCopyID(UINT32 dwCopyType)
{
	m_MaxCopyBaseID += 1;

	return CGameService::GetInstancePtr()->GetServerID()<<24|m_MaxCopyBaseID;
}

BOOL CSceneManager::OnMsgCreateSceneReq(NetPacket *pNetPacket)
{
	CreateNewSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	CreateNewSceneAck Ack;
	Ack.set_createparam(Req.createparam());
	UINT32 dwNewCopyID = MakeCopyID(Req.copytype());
	Ack.set_copyid(dwNewCopyID);
	Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
	if (!CreateScene(Req.copytype(), dwNewCopyID, 1))
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
	if(!CreateScene(1, MakeCopyID(1), 1))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	if(!CreateScene(1, MakeCopyID(1), 1))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	if(!CreateScene(1, MakeCopyID(1), 1))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	if(!CreateScene(1, MakeCopyID(1), 1))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	return TRUE;
}
