#include "stdafx.h"
#include "SceneManager.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "GameService.h"
#include "Error.h"



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

BOOL CSceneManager::Init(UINT32 dwReserved)
{
	if(!LoadDefaultScene())
	{
		ASSERT_FAIELD;
		return FALSE;
	}


	return TRUE;
}

BOOL CSceneManager::Uninit()
{
	return TRUE;
}

BOOL CSceneManager::CreateScene( UINT32 dwSceneType, UINT32 dwLogicType )
{
	CScene *pScene = new CScene;

	if(!pScene->Init(dwSceneType, dwLogicType))
	{
		ASSERT_FAIELD;

		delete pScene;

		return FALSE;
	}

	m_mapSceneList.insert(std::make_pair(dwSceneID, pScene));

	return TRUE;
}

BOOL CSceneManager::DispatchPacket(NetPacket *pNetPack)
{
	BOOL bHandled = TRUE;
	PacketHeader *pPacketHeader = (PacketHeader *)pNetPack->m_pDataBuffer->GetBuffer();
	if(pPacketHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	switch(pNetPack->dwMsgID)
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


	CScene *pScene = GetSceneByID(pPacketHeader->dwSceneID);
	if(pScene == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	pScene->DispatchPacket(pNetPack);
		
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

BOOL CSceneManager::OnMsgCreateSceneReq(NetPacket *pNetPack)
{
	StSvrCreateSceneReq CreateSceneReq;
	CBufferHelper bh(FALSE, pNetPack->m_pDataBuffer);
	bh.Read(CreateSceneReq);

	StSvrCreateSceneAck CreateSceneAck;
	CreateSceneAck.dwCreateParam = CreateSceneReq.CreateParam;
	CreateSceneAck.dwSceneID = CreateSceneReq.dwSceneID;
	CreateSceneAck.dwServerID = 1;

	if (!CreateScene(CreateSceneReq.dwSceneID))
	{
		ASSERT_FAIELD;

		CreateSceneAck.dwAckCode = E_FAILED;
	}
	else
	{
		CreateSceneAck.dwAckCode = E_SUCCESSED;
	}
	
	CBufferHelper WriteHelper(TRUE, 1024);
	WriteHelper.BeginWrite(CMD_SVR_CREATE_SCENE_ACK, 0, 0);
	WriteHelper.Write(CreateSceneAck);
	WriteHelper.EndWrite();

	ServiceBase::GetInstancePtr()->SendCmdToConnection(pNetPack->m_pConnect->GetConnectionID(), WriteHelper.GetDataBuffer());

	return TRUE;
}

BOOL CSceneManager::LoadDefaultScene()
{
	if(!CreateScene(12))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	/*StSvrCreateSceneAck CreateSceneAck;
	CreateSceneAck.dwCreateParam = 0;
	CreateSceneAck.dwSceneID = 12;
	CreateSceneAck.dwServerID= CGameService::GetInstancePtr()->GetServerID();

	CBufferHelper WriteHelper(TRUE, 1024);
	WriteHelper.BeginWrite(CMD_SVR_CREATE_SCENE_ACK , 0, 0);
	WriteHelper.Write(CreateSceneAck);
	WriteHelper.EndWrite();

	ServiceBase::GetInstancePtr()->SendCmdToConnection(CGameService::GetInstancePtr()->m_dwWorldServerID, &m_WriteBuffer);*/

	return TRUE;
}
