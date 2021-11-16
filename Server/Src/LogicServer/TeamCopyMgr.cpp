#include "stdafx.h"
#include "TeamCopyMgr.h"
#include "GameService.h"
#include "PacketHeader.h"

#include "ServerDefine.h"
#include "../StaticData/StaticStruct.h"
#include "../StaticData/StaticData.h"
#include "RoleData.h"

#include "CopyModule.h"
#include "BagModule.h"
#include "RoleModule.h"
#include "TimerManager.h"
#include "GameSvrMgr.h"

#include "../Message/Msg_ID.pb.h"
#include "../Message/Game_Define.pb.h"
#include "../Message/Msg_Copy.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "EquipModule.h"

CTeamCopyMgr::CTeamCopyMgr(void)
{
    RegisterMessageHanler();

    TimerManager::GetInstancePtr()->AddDiffTimer(60, 1, &CTeamCopyMgr::OnTimer, this);
}

CTeamCopyMgr::~CTeamCopyMgr(void)
{
}

CTeamCopyMgr* CTeamCopyMgr::GetInstancePtr()
{
    static CTeamCopyMgr _TeamCopyManager;

    return &_TeamCopyManager;
}


BOOL CTeamCopyMgr::Init()
{
    return TRUE;
}

BOOL CTeamCopyMgr::OnTimer(INT32 nParam)
{
    return TRUE;
}

CRoomItem* CTeamCopyMgr::CreateRoom(UINT32 dwCopyID, UINT64 uRoleID)
{
    CRoomItem* pRoomItem = m_WaitRoomList.CreateEmptyRoom();
    ERROR_RETURN_NULL(pRoomItem != NULL);

    pRoomItem->m_dwCopyID = dwCopyID;

    pRoomItem->m_dwCopyType = CStaticData::GetInstancePtr()->GetCopyType(dwCopyID);

    pRoomItem->AddPlayer(uRoleID, EBC_PLAYER);

    return pRoomItem;
}

VOID CTeamCopyMgr::RegisterMessageHanler()
{
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_CREATE_ROOM_REQ, &CTeamCopyMgr::OnMsgCreateRoom, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_JOIN_ROOM_REQ, &CTeamCopyMgr::OnMsgJoinRoom, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_LEAVE_ROOM_REQ, &CTeamCopyMgr::OnMsgLeaveRoom, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_START_ROOM_REQ, &CTeamCopyMgr::OnMsgStartBattle, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_KICK_ROOM_REQ, &CTeamCopyMgr::OnMsgKickRoom, this);
}

BOOL CTeamCopyMgr::OnMsgCreateRoom(NetPacket* pNetPacket)
{
    Msg_CreateRoomReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pPlayer != NULL);

    //理论上不应该有在房间的情况还来创建房间，除非是掉线消息丢失
    ERROR_RETURN_TRUE(pPlayer->GetRoomID() == 0);

    CRoomItem* pRoomItem = CreateRoom(Req.copyid(), pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pRoomItem != NULL);

    Msg_CreateRoomAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);
    Ack.set_roomid(pRoomItem->m_uRoomID);

    pPlayer->SetRoomID(pRoomItem->m_uRoomID);

    pPlayer->SendMsgProtoBuf(MSG_CREATE_ROOM_ACK, Ack);

    m_WaitRoomList.NotifyRoomChange(pRoomItem);

    //验证玩家是否合法
    //玩家是否在其它的房间内没有退出。
    //清理其它房间内的玩家
    //创建新的房间  6
    //返回房间的id给客户端
    //返回成功的消息
    return TRUE;
}

BOOL CTeamCopyMgr::OnMsgLeaveRoom(NetPacket* pNetPacket)
{
    Msg_LeaveRoomReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    Msg_LeaveRoomAck Ack;

    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pPlayer != NULL);

    pPlayer->SetRoomID(0);

    CRoomItem* pRoomItem = m_WaitRoomList.GetRoomItem(Req.roomid());
    if (pRoomItem == NULL)
    {
        return FALSE;
    }

    pRoomItem->DeletePlayer(pHeader->u64TargetID);

    if (pRoomItem->GetCount() <= 0)
    {
        m_WaitRoomList.DestroyRoom(pRoomItem->m_uRoomID);
        pPlayer->SendMsgProtoBuf(MSG_LEAVE_ROOM_ACK, Ack);
        return TRUE;
    }

    m_WaitRoomList.NotifyRoomChange(pRoomItem);

    //验证玩家是否合法
    //验证玩家是否在这个房间内
    //如果房间只有一个人，退出，房间销毁
    //如果房间内还有其它的人，转移房主的权力
    //房间状态通知其它的玩家
    //返回成功
    return TRUE;
}

BOOL CTeamCopyMgr::OnMsgKickRoom(NetPacket* pNetPacket)
{
    Msg_KickRoomReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pPlayer != NULL);

    Msg_KickRoomAck Ack;
    CRoomItem* pRoomItem = m_WaitRoomList.GetRoomItem(Req.roomid());
    ERROR_RETURN_TRUE(pRoomItem != NULL);

    pRoomItem->DeletePlayer(Req.roleid());

    m_WaitRoomList.NotifyRoomChange(pRoomItem);

    CPlayerObject* pTartet = CPlayerManager::GetInstancePtr()->GetPlayer(Req.roleid());
    ERROR_RETURN_TRUE(pTartet != NULL);

    pTartet->SetRoomID(0);

    return TRUE;
}

BOOL CTeamCopyMgr::OnMsgJoinRoom(NetPacket* pNetPacket)
{
    Msg_JoinRoomReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pPlayer != NULL);

    ERROR_RETURN_TRUE(pPlayer->GetRoomID() == 0);

    Msg_JoinRoomAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);

    CRoomItem* pRoomItem = NULL;
    if (Req.roomid() > 0)
    {
        pRoomItem = m_WaitRoomList.GetRoomItem(Req.roomid());
    }
    else
    {
        pRoomItem = m_WaitRoomList.GetAvailableRoom();
    }

    ERROR_RETURN_TRUE(pRoomItem != NULL);

    if (!pRoomItem->AddPlayer(pHeader->u64TargetID, EBC_PLAYER))
    {
        Ack.set_retcode(MRC_UNKNOW_ERROR);
        pPlayer->SendMsgProtoBuf(MSG_JOIN_ROOM_ACK, Ack);
        return TRUE;
    }

    Ack.set_roomid(pRoomItem->m_uRoomID);
    pPlayer->SendMsgProtoBuf(MSG_JOIN_ROOM_ACK, Ack);

    pPlayer->SetRoomID(pRoomItem->m_uRoomID);

    m_WaitRoomList.NotifyRoomChange(pRoomItem);

    //1.指定加入
    //验证玩家是否合法
    //验证房间是否存在
    //房间内是否还有位置
    //加入房间
    //房间状态通知其它的玩家
    //返回结果

    //2.搜索加入
    //验证玩家是否合法
    //是否能找到需要人的房间
    //加入房间
    //房间状态通知其它的玩家
    //返回结果

    return TRUE;
}

BOOL CTeamCopyMgr::OnMsgStartBattle(NetPacket* pNetPacket)
{
    Msg_StartRoomReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CRoomItem* pItem = m_WaitRoomList.GetRoomItem(Req.roomid());
    ERROR_RETURN_TRUE(pItem != NULL);

    CGameSvrMgr::GetInstancePtr()->TakeCopyRequest(pItem->m_uRoleID, pItem->m_nCamp, pItem->m_nCount, pItem->m_dwCopyID, pItem->m_dwCopyType);


    for (int i = 0; i < pItem->GetCount(); i++)
    {
        CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pItem->m_uRoleID[i]);
        ERROR_CONTINUE_EX(pPlayer != NULL);

        pPlayer->SetRoomID(0);
    }

    m_WaitRoomList.DestroyRoom(pItem->m_uRoomID);

    return TRUE;
}

CRoomList::CRoomList()
{
    m_uMaxID = 0;
}

CRoomList::~CRoomList()
{

}

CRoomItem* CRoomList::GetRoomItem(UINT64 uRoomID)
{
    return GetByKey(uRoomID);
}

CRoomItem* CRoomList::GetAvailableRoom()
{
    TNodeTypePtr pNode = MoveFirst();
    ERROR_RETURN_FALSE(pNode != NULL);

    UINT32 dwProxyID = 0;
    CRoomItem* pItem = NULL;
    for (; pNode != NULL; pNode = MoveNext(pNode))
    {
        pItem = pNode->GetValue();
        ERROR_RETURN_FALSE(pItem != NULL);

        if (pItem->GetCount() < 3)
        {
            return pItem;
        }
    }

    return NULL;
}

BOOL CRoomList::DestroyRoom(UINT64 uRoomID)
{
    return Delete(uRoomID);
}


CRoomItem* CRoomList::CreateEmptyRoom()
{
    m_uMaxID += 1;
    CRoomItem* pItem = InsertAlloc(m_uMaxID);
    ERROR_RETURN_NULL(pItem != NULL);
    pItem->m_uRoomID = m_uMaxID;
    return pItem;
}

BOOL CRoomList::NotifyRoomChange(UINT64 uRoomID)
{
    CRoomItem* pItem = GetRoomItem(uRoomID);
    ERROR_RETURN_FALSE(pItem != NULL);
    return NotifyRoomChange(pItem);
}

BOOL CRoomList::NotifyRoomChange(CRoomItem* pRoomItem)
{
    Msg_BroadRoomNotify Nty;
    for (int i = 0; i < pRoomItem->GetCount(); i++)
    {
        CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pRoomItem->m_uRoleID[i]);

        Msg_RoomPlayerInfo* pPlayerInfo = Nty.add_playerlist();

        CRoleModule* pRoleModule = (CRoleModule*)pPlayer->GetModuleByType(MT_ROLE);

        pPlayerInfo->set_actorid(pRoleModule->GetActorID());
        pPlayerInfo->set_roleid(pPlayer->GetRoleID());
        pPlayerInfo->set_level(pRoleModule->GetLevel());
        pPlayerInfo->set_name(pRoleModule->GetName());

        CEquipModule* pEquipModule = (CEquipModule*)pPlayer->GetModuleByType(MT_EQUIP);

        for (int i = 0; i < EQUIP_MAX_NUM; i++)
        {
            EquipDataObject* pEquip = pEquipModule->m_vtDressEquip[i];
            if (pEquip == NULL)
            {
                pPlayerInfo->add_equips(0);
            }
            else
            {
                pPlayerInfo->add_equips(pEquip->m_EquipID);
            }
        }
    }

    for (int i = 0; i < pRoomItem->GetCount(); i++)
    {
        CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pRoomItem->m_uRoleID[i]);

        pPlayer->SendMsgProtoBuf(MSG_BROAD_ROOM_NOTIFY, Nty);
    }

    return TRUE;
}
