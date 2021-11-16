#include "stdafx.h"
#include "GameSvrMgr.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Game_Define.pb.h"
#include "../ServerData/ServerDefine.h"
#include "../StaticData/StaticStruct.h"
#include "../StaticData/StaticData.h"
#include "../ServerData/RoleData.h"

#include "CopyModule.h"
#include "BagModule.h"
#include "RoleModule.h"
#include "MsgHandlerManager.h"

CGameSvrMgr::CGameSvrMgr(void)
{
    RegisterMessageHanler();
}

CGameSvrMgr::~CGameSvrMgr(void)
{
}

CGameSvrMgr* CGameSvrMgr::GetInstancePtr()
{
    static CGameSvrMgr _PlayerManager;

    return &_PlayerManager;
}


BOOL CGameSvrMgr::Init()
{
    return TRUE;
}

BOOL CGameSvrMgr::Uninit()
{
    return TRUE;
}

BOOL CGameSvrMgr::TakeCopyRequest(UINT64 uID, INT32 dwCamp, UINT32 dwCopyID, UINT32 dwCopyType)
{
    ERROR_RETURN_FALSE(uID > 0);

    AddWaitItem(uID, dwCamp);

    ERROR_RETURN_TRUE(CGameSvrMgr::GetInstancePtr()->CreateScene(dwCopyID, uID, 1, dwCopyType));

    return TRUE;
}

BOOL CGameSvrMgr::TakeCopyRequest(UINT64 uID[], INT32 dwCamp[], INT32 nNum,  UINT32 dwCopyID, UINT32 dwCopyType)
{
    ERROR_RETURN_FALSE(nNum > 0);
    ERROR_RETURN_FALSE(nNum < 11);
    ERROR_RETURN_FALSE(uID[0] > 0);

    AddWaitItem(uID[0], uID, dwCamp, nNum);

    ERROR_RETURN_TRUE(CGameSvrMgr::GetInstancePtr()->CreateScene(dwCopyID, uID[0], nNum, dwCopyType));

    return TRUE;
}

UINT32 CGameSvrMgr::GetServerIDByCopyGuid(UINT32 dwCopyGuid)
{
    auto itor = m_GuidToSvrID.find(dwCopyGuid);
    if (itor != m_GuidToSvrID.end())
    {
        return itor->second;
    }
    return 1;
}

BOOL CGameSvrMgr::CreateScene(UINT32 dwCopyID, UINT64 uCreateParam, UINT32 dwPlayerNum, UINT32 dwCopyType )
{
    ERROR_RETURN_FALSE(dwCopyID != 0);
    ERROR_RETURN_FALSE(uCreateParam != 0);

    //选择一个可用的副本服务器
    UINT32 dwServerID = GetBestGameServerID();
    if(dwServerID == 0)
    {
        CLog::GetInstancePtr()->LogError("没有找到可用的场景服务器，或者说没有找到可用的副本服务器");
        return FALSE;
    }

    //向副本服务器发送创建副本的消息
    if(!SendCreateSceneCmd(dwServerID, dwCopyID, dwCopyType, uCreateParam, dwPlayerNum))
    {
        //发送创建副本的消息失败
        CLog::GetInstancePtr()->LogError("发送创建副本的消息失败");
        return FALSE;
    }

    return TRUE;
}


BOOL CGameSvrMgr::CreateScene(UINT32 dwCopyID, UINT64 CreateParam, UINT32 dwCopyType)
{
    return CreateScene(dwCopyID, CreateParam, 0, dwCopyType);
}

BOOL CGameSvrMgr::BroadMsgToAll(INT32 nMsgID, CHAR* pData, INT32 nSize)
{
    for (std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.begin(); itor != m_mapGameSvr.end(); itor++)
    {
        GameSvrInfo& Info = itor->second;

        ERROR_CONTINUE_EX(ServiceBase::GetInstancePtr()->SendMsgRawData(Info.m_nConnID, nMsgID, 0, 0, pData, nSize));
    }

    return TRUE;
}

VOID CGameSvrMgr::RegisterMessageHanler()
{
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_GAME_REGTO_LOGIC_REQ, &CGameSvrMgr::OnMsgGameSvrRegister, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_CREATE_SCENE_ACK, &CGameSvrMgr::OnMsgCreateSceneAck, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_TRANSFER_DATA_ACK, &CGameSvrMgr::OnMsgTransRoleDataAck, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_COPYINFO_REPORT_REQ, &CGameSvrMgr::OnMsgCopyReportReq, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_BATTLE_RESULT_NTY, &CGameSvrMgr::OnMsgBattleResultNty, this);
}

BOOL CGameSvrMgr::SendCreateSceneCmd( UINT32 dwServerID, UINT32 dwCopyID, UINT32 dwCopyType, UINT64 CreateParam, UINT32 dwPlayerNum )
{
    CreateNewSceneReq Req;
    Req.set_copyid(dwCopyID);
    Req.set_createparam(CreateParam);
    Req.set_copytype(dwCopyType);
    Req.set_playernum(dwPlayerNum);
    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(GetConnIDBySvrID(dwServerID), MSG_CREATE_SCENE_REQ, 0, 0, Req);
}


UINT32 CGameSvrMgr::GetConnIDBySvrID(UINT32 dwServerID)
{
    std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.find(dwServerID);
    if(itor == m_mapGameSvr.end())
    {
        return 0;
    }

    return itor->second.m_nConnID;
}

BOOL CGameSvrMgr::SendPlayerToMainCity(UINT64 u64ID, UINT32 dwCopyID)
{
    UINT32 dwSvrID, nConnID, dwCopyGuid;
    CGameSvrMgr::GetInstancePtr()->GetMainCityInfo(dwCopyID, dwSvrID, nConnID, dwCopyGuid);
    ERROR_RETURN_TRUE(nConnID != 0);
    ERROR_RETURN_FALSE(u64ID != 0);
    ERROR_RETURN_FALSE(dwCopyGuid != 0);
    ERROR_RETURN_FALSE(dwSvrID != 0);
    ERROR_RETURN_FALSE(dwCopyID != 0);
    SendPlayerToCopy(u64ID, dwSvrID, dwCopyID, dwCopyGuid, 1);

    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(u64ID);
    ERROR_RETURN_FALSE(pPlayer != NULL);
    pPlayer->m_bMainCity = TRUE;

    return TRUE;
}

BOOL CGameSvrMgr::SendPlayerToCopy(UINT64 u64ID, UINT32 dwServerID, UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCamp)
{
    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(u64ID);
    ERROR_RETURN_FALSE(pPlayer != NULL);
    ERROR_RETURN_FALSE(pPlayer->m_dwCopyID != dwCopyID);
    ERROR_RETURN_FALSE(pPlayer->m_dwCopyGuid != dwCopyGuid);

    INT32 nConnID = CGameSvrMgr::GetInstancePtr()->GetConnIDBySvrID(dwServerID);
    ERROR_RETURN_FALSE(nConnID != 0);

    TransferDataReq Req;
    TransferDataItem* pItem = Req.add_transdatas();
    pItem->set_camp(dwCamp);
    ERROR_RETURN_FALSE(pPlayer->ToTransferData(pItem));

    AddWaitItem(u64ID, dwCamp);

    if (pPlayer->m_bMainCity)
    {
        pPlayer->SendLeaveScene(pPlayer->m_dwCopyGuid, pPlayer->m_dwCopySvrID);
    }

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_TRANSFER_DATA_REQ, u64ID, dwCopyGuid, Req);

    ERROR_RETURN_FALSE(pPlayer->SetCopyStatus(dwCopyGuid, dwCopyID, dwServerID, FALSE));

    return TRUE;
}

BOOL CGameSvrMgr::GetMainCityInfo(UINT32 dwCopyID, UINT32& dwServerID, UINT32& nConnID, UINT32& dwCopyGuid)
{
    auto itor = m_mapCity.find(dwCopyID);
    if(itor != m_mapCity.end())
    {
        dwServerID = itor->second.m_dwSvrID;
        nConnID = itor->second.m_nConnID;
        dwCopyGuid = itor->second.m_dwCopyGuid;
        return TRUE;
    }

    dwServerID = 1;
    dwCopyGuid = dwServerID << 24 | 1;
    nConnID = GetConnIDBySvrID(dwServerID);

    return TRUE;
}


BOOL CGameSvrMgr::AddWaitItem(UINT64 u64ID, INT32 dwCamp)
{
    ERROR_RETURN_FALSE(u64ID > 0);

    CWaitItem* pItem = m_WaitCopyList.InsertAlloc(u64ID);
    ERROR_RETURN_FALSE(pItem != NULL);
    pItem->uID[0] = u64ID;
    pItem->nCamp[0] = dwCamp;

    return TRUE;
}

BOOL CGameSvrMgr::AddWaitItem(UINT64 uKey, UINT64 uID[], INT32 dwCamp[], INT32 nNum)
{
    CWaitItem* pItem = m_WaitCopyList.InsertAlloc(uKey);
    ERROR_RETURN_FALSE(pItem != NULL);

    for (int i = 0; i < nNum; i++)
    {
        pItem->uID[i] = uID[i];
        pItem->nCamp[i] = dwCamp[i];
    }

    return TRUE;
}

BOOL CGameSvrMgr::OnMsgGameSvrRegister(NetPacket* pNetPacket)
{
    SvrRegToSvrReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.find(Req.serverid());
    if(itor != m_mapGameSvr.end())
    {
        itor->second.m_nConnID = pNetPacket->m_nConnID;
        itor->second.m_nSvrID = Req.serverid();
    }

    m_mapGameSvr.insert(std::make_pair(Req.serverid(), GameSvrInfo(Req.serverid(), pNetPacket->m_nConnID)));

    return TRUE;
}

BOOL CGameSvrMgr::OnMsgCopyReportReq(NetPacket* pNetPacket)
{
    CopyReportReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    return TRUE;
    for(int i = 0; i < Req.copylist_size(); i++)
    {
        const CopyInsItem& item = Req.copylist(i);
        m_mapCity.insert(std::make_pair(item.copyid(), CityInfo(item.copyid(), item.serverid(), pNetPacket->m_nConnID, item.copyguid())));

        m_GuidToSvrID.insert(std::make_pair(item.copyguid(), item.serverid()));
    }

    return TRUE;
}

BOOL CGameSvrMgr::OnMsgCreateSceneAck(NetPacket* pNetPacket)
{
    CreateNewSceneAck Ack;
    Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    ERROR_RETURN_TRUE(Ack.copyid() != 0);
    ERROR_RETURN_TRUE(Ack.copyguid() != 0);
    ERROR_RETURN_TRUE(Ack.serverid() != 0);
    ERROR_RETURN_TRUE(Ack.createparam() != 0);
    ERROR_RETURN_TRUE(Ack.playernum() != 0);
    ERROR_RETURN_TRUE(Ack.copytype() != 0);

    m_GuidToSvrID.insert(std::make_pair(Ack.copyguid(), Ack.serverid()));

    if (Ack.playernum() == 0)
    {
        //表示这是一个任意人数,任意进出的副本,人员信息将在后面放进去
        return TRUE;
    }

    INT32 nConnID = CGameSvrMgr::GetInstancePtr()->GetConnIDBySvrID(Ack.serverid());
    ERROR_RETURN_FALSE(nConnID != 0);

    CWaitItem* pWaitItem = m_WaitCopyList.GetWaitItem(Ack.createparam());
    ERROR_RETURN_FALSE(pWaitItem != NULL);

    TransferDataReq Req;
    for (int i = 0; i < 10; i++)
    {
        if (pWaitItem->uID[i] <= 0)
        {
            break;
        }

        CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pWaitItem->uID[i]);
        ERROR_RETURN_FALSE(pPlayer != NULL);
        ERROR_RETURN_FALSE(pPlayer->m_dwCopyID != Ack.copyid());
        ERROR_RETURN_FALSE(pPlayer->m_dwCopyGuid != Ack.copyguid());
        TransferDataItem* pItem = Req.add_transdatas();
        pItem->set_camp(pWaitItem->nCamp[i]);
        ERROR_RETURN_FALSE(pPlayer->ToTransferData(pItem));

        if (pPlayer->m_bMainCity)
        {
            pPlayer->SendLeaveScene(pPlayer->m_dwCopyGuid, pPlayer->m_dwCopySvrID);
        }

        pPlayer->SetCopyStatus(Ack.copyguid(), Ack.copyid(), Ack.serverid(), FALSE);
    }

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(nConnID, MSG_TRANSFER_DATA_REQ, Ack.createparam(), Ack.copyguid(), Req);

    return TRUE;
}

BOOL CGameSvrMgr::OnMsgTransRoleDataAck(NetPacket* pNetPacket)
{
    TransferDataAck Ack;
    Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

    CWaitItem* pWaitItem = m_WaitCopyList.GetWaitItem(pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pWaitItem != NULL);

    for (int i = 0; i < 10; i++)
    {
        if (pWaitItem->uID[i] <= 0)
        {
            break;
        }

        CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pWaitItem->uID[i]);
        ERROR_RETURN_TRUE(pPlayer != NULL);
        ERROR_RETURN_TRUE(Ack.copyid() != 0);
        ERROR_RETURN_TRUE(Ack.copyguid() != 0);
        ERROR_RETURN_TRUE(Ack.serverid() != 0);
        pPlayer->SendIntoSceneNotify(Ack.copyguid(), Ack.copyid(), Ack.serverid());
    }

    m_WaitCopyList.Delete(pHeader->u64TargetID);

    return TRUE;
}

BOOL CGameSvrMgr::OnCloseConnect(INT32 nConnID)
{


    return TRUE;
}

UINT32 CGameSvrMgr::GetBestGameServerID()
{
    INT32 nMinLoad = 1000000;
    INT32 nSvrID = 0;
    for(std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.begin(); itor != m_mapGameSvr.end(); itor++)
    {
        if(itor->second.m_nLoad < nMinLoad)
        {
            nSvrID = itor->second.m_nSvrID;
            nMinLoad = itor->second.m_nLoad;
        }
    }

    return nSvrID;
}

GameSvrInfo* CGameSvrMgr::GetGameSvrInfo(UINT32 dwSvrID)
{
    std::map<UINT32, GameSvrInfo>::iterator itor = m_mapGameSvr.find(dwSvrID);
    if (itor == m_mapGameSvr.end())
    {
        return NULL;
    }

    return &itor->second;
}

BOOL CGameSvrMgr::OnMsgBattleResultNty( NetPacket* pNetPacket )
{
    BattleResultNty Nty;
    Nty.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    ERROR_RETURN_TRUE(Nty.copytype() != 0);
    ERROR_RETURN_TRUE(Nty.copyid() != 0);
    ERROR_RETURN_TRUE(Nty.copyguid() != 0);
    ERROR_RETURN_TRUE(Nty.serverid() != 0);

    switch(Nty.copytype())
    {
        case CPT_MAIN:
            OnMainCopyResult(Nty);
            break;
    }

    return TRUE;
}


BOOL CGameSvrMgr::OnMainCopyResult(BattleResultNty& Nty)
{
    ERROR_RETURN_TRUE(Nty.playerlist_size() > 0);

    for (int i = 0; i < Nty.playerlist_size(); i++)
    {
        const ResultPlayer& Result = Nty.playerlist(i);

        CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(Result.objectid());
        ERROR_RETURN_TRUE(pPlayer != NULL);

        CCopyModule* pCopyModule = (CCopyModule*)pPlayer->GetModuleByType(MT_COPY);
        ERROR_RETURN_TRUE(pCopyModule != NULL);

        pCopyModule->OnMainCopyResult(&Nty, i);
    }

    return TRUE;
}

CWaitCopyList::CWaitCopyList()
{

}

CWaitCopyList::~CWaitCopyList()
{

}

CWaitItem* CWaitCopyList::GetWaitItem(UINT64 uParam)
{
    return GetByKey(uParam);
}
