#include "stdafx.h"
#include "SceneManager.h"
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
        ERROR_RETURN_FALSE(LoadMainScene());
    }

    return TRUE;
}

BOOL CSceneManager::Uninit()
{
    return TRUE;
}

BOOL CSceneManager::CreateScene(UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCopyType, UINT32 dwPlayerNum, UINT64 uCreateKey)
{
	CScene* pScene = new CScene;

	if(!pScene->Init(dwCopyID, dwCopyGuid, dwCopyType, dwPlayerNum, uCreateKey))
	{
		CLog::GetInstancePtr()->LogError("Error Create Scene Failed, CopyID:%d; CopyType:%d, PlayerNum:%d", dwCopyID, dwCopyType, dwPlayerNum);

		delete pScene;

		return FALSE;
	}

	m_mapSceneList.insert(std::make_pair(dwCopyGuid, pScene));

	return TRUE;
}

BOOL CSceneManager::DispatchPacket(NetPacket* pNetPacket)
{
	PacketHeader* pPacketHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_FALSE(pPacketHeader != NULL);

	switch(pNetPacket->m_nMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_CREATE_SCENE_REQ,   OnMsgCreateSceneReq);
	}

	CScene* pScene = GetSceneByCopyGuid(pPacketHeader->dwUserData);
	if (pScene == NULL)
	{
		CLog::GetInstancePtr()->LogError("Error : Invalid CopyGuid:%d, MessageID:%d", pPacketHeader->dwUserData, pNetPacket->m_nMsgID);
		return TRUE;
	}

	if (pScene->DispatchPacket(pNetPacket))
	{
		return TRUE;
	}

	return FALSE;
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

BOOL CSceneManager::OnUpdate( UINT64 uTick )
{
    for(SceneMap::iterator itor = m_mapSceneList.begin(); itor != m_mapSceneList.end();)
    {
        CScene* pScene = itor->second;

        if((pScene->GetLastTick() < uTick) && ( uTick - pScene->GetLastTick() < FPS_TIME_TICK))
        {
            itor++;
            continue;
        }

        pScene->OnUpdate(uTick);

        pScene->SetLastTick(uTick);

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

UINT32 CSceneManager::MakeCopyGUID()
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
            CopyInsItem* pItem = Req.add_copylist();
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

	UINT32 dwNewCopyGuid = MakeCopyGUID();

	CreateNewSceneAck Ack;
	Ack.set_createparam(Req.createparam());
	Ack.set_copyguid(dwNewCopyGuid);
	Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
	Ack.set_copyid(Req.copyid());
	Ack.set_playernum(Req.playernum());
	Ack.set_copytype(Req.copytype());
	if (!CreateScene(Req.copyid(), dwNewCopyGuid, Req.copytype(), Req.playernum(), Req.createparam()))
	{
		Ack.set_retcode(MRC_UNKNOW_ERROR);
	}
	else
	{
		Ack.set_retcode(MRC_SUCCESSED);
	}

	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_nConnID, MSG_CREATE_SCENE_ACK, 0, 0, Ack);

	return TRUE;
}

BOOL CSceneManager::LoadMainScene()
{
	ERROR_RETURN_FALSE(CreateScene(6, MakeCopyGUID(), 3, 0, 0));

	return TRUE;
}
