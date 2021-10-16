#include "stdafx.h"
#include "GameService.h"
#include "Scene.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Game_Define.pb.h"
#include "../Message/Msg_Move.pb.h"
#include "../Message/Msg_Copy.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "SceneLogic/SceneLogic_Normal.h"
#include "SceneLogic/SceneLogic_None.h"
#include "SceneLogic/SceneLogic_City.h"
#include "MonsterCreator.h"
#include "RapidXml.h"
#include "SceneXmlMgr.h"
#include "StaticStruct.h"
#include "StaticData.h"
#include "GameObject/SkillObject.h"
#include "GameObject/BulletObject.h"
#include "../ServerData/ServerStruct.h"


CScene::CScene()
{
    Reset();
}

CScene::~CScene()
{
    Reset();
}

BOOL CScene::Init(UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCopyType, UINT32 dwPlayerNum, UINT64 uCreateKey)
{
    m_dwCopyGuid        = dwCopyGuid;
    m_dwCopyID          = dwCopyID;
    m_dwCopyType        = dwCopyType;
    m_dwPlayerNum       = dwPlayerNum;
    m_uCreateKey        = uCreateKey;
    m_dwLoginNum        = 0;
    m_uStartTime        = 0;
    m_uMaxGuid          = 0;
    m_uCreateTime       = CommonFunc::GetCurrTime();
    m_pMonsterCreator   = new MonsterCreator(this);

    //表示这是一个自由进出的副本, 所以创建即开始
    if (dwPlayerNum == 0)
    {
        m_uStartTime = CommonFunc::GetCurrTime();
    }

    ERROR_RETURN_FALSE(CreateSceneLogic(dwCopyType));

    ERROR_RETURN_FALSE(ReadSceneXml());
    return TRUE;
}

BOOL CScene::Uninit()
{
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        delete pObj;
    }

    m_mapPlayer.clear();
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        delete pObj;
    }
    m_mapMonster.clear();

    for (std::map<UINT64, CBulletObject*>::iterator itor = m_mapBullet.begin(); itor != m_mapBullet.end(); itor++)
    {
        CBulletObject* pObj = itor->second;
        delete pObj;
    }
    m_mapBullet.clear();

    m_HitEffectNtf.Clear();

    delete m_pMonsterCreator;

    m_pMonsterCreator = NULL;

    ERROR_RETURN_FALSE(DestroySceneLogic(m_dwCopyType));

    return TRUE;
}

BOOL CScene::Reset()
{
    m_dwCopyGuid = 0;
    m_dwCopyID = 0;
    m_dwCopyType = 0;
    m_dwPlayerNum = 0;
    m_uCreateKey = 0;
    m_dwLoginNum = 0;
    m_uStartTime = 0;
    m_uMaxGuid = 0;
    m_uCreateTime = 0;
    m_pMonsterCreator = NULL;
    m_bFinished = FALSE;

    return TRUE;
}

BOOL CScene::DispatchPacket(NetPacket* pNetPacket)
{
    switch(pNetPacket->m_nMsgID)
    {
            PROCESS_MESSAGE_ITEM(MSG_TRANSFER_DATA_REQ,     OnMsgTransRoleDataReq);
            PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,       OnMsgEnterSceneReq);
            PROCESS_MESSAGE_ITEM(MSG_LEAVE_SCENE_REQ,       OnMsgLeaveSceneReq);
            PROCESS_MESSAGE_ITEM(MSG_ABORT_SCENE_REQ,       OnMsgAbortSceneReq);
            PROCESS_MESSAGE_ITEM(MSG_DISCONNECT_NTY,        OnMsgRoleDisconnect);
            PROCESS_MESSAGE_ITEMEX(MSG_SKILL_CAST_REQ,      OnMsgSkillCastReq);
            PROCESS_MESSAGE_ITEMEX(MSG_OBJECT_ACTION_REQ,   OnMsgObjectActionReq);
            PROCESS_MESSAGE_ITEM(MSG_HEART_BEAT_REQ,        OnMsgHeartBeatReq);
            PROCESS_MESSAGE_ITEM(MSG_USE_HP_BOOTTLE_REQ,    OnMsgUseHpBottleReq);
            PROCESS_MESSAGE_ITEM(MSG_USE_MP_BOOTTLE_REQ,    OnMsgUseMpBottleReq);
            PROCESS_MESSAGE_ITEM(MSG_BATTLE_CHAT_REQ,       OnMsgBattleChatReq);
            PROCESS_MESSAGE_ITEM(MSG_PLAYER_CHAGE_NTF,      OnMsgObjectChangeNtf);
            PROCESS_MESSAGE_ITEM(MSG_MOUNT_RIDING_REQ,      OnMsgMountRidingReq);
            PROCESS_MESSAGE_ITEM(MSG_ROLE_REBORN_REQ,       OnMsgRoleRebornReq);
    }

    return FALSE;
}

BOOL CScene::AddHitEffect(UINT64 uAttackerID, UINT64 uTargetID, INT32 nHurtValue, BOOL bCritHit, INT32 nHitActionID, INT32 nHitEffectID, FLOAT fHitDistance)
{
    HitEffectItem* pItem = m_HitEffectNtf.add_itemlist();
    pItem->set_castguid(uAttackerID);
    pItem->set_targetguid(uTargetID);
    pItem->set_crit(bCritHit);
    pItem->set_hurtvalue(nHurtValue);
    pItem->set_hitactionid(nHitActionID);
    pItem->set_hiteffectid(nHitEffectID);
    pItem->set_hitdistance(fHitDistance);

    return TRUE;
}

BOOL CScene::BroadHitEffect()
{
    if (m_HitEffectNtf.itemlist_size() <= 0)
    {
        return TRUE;
    }

    for (auto itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); ++itor)
    {
        CSceneObject* pSceneObject = itor->second;
        ERROR_CONTINUE_EX(pSceneObject != NULL);
        ERROR_CONTINUE_EX(pSceneObject->IsRobot() == FALSE);
        if (!pSceneObject->IsEnterCopy())
        {
            continue;
        }

        pSceneObject->SendMsgProtoBuf(MSG_ACTOR_HITEFFECT_NTF, m_HitEffectNtf);
    }

    m_HitEffectNtf.Clear();

    return TRUE;
}

BOOL CScene::SetBattleResult(UINT32 dwCamp, ECopyResult nBattleResult)
{
    for (auto itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); ++itor)
    {
        CSceneObject* pObj = itor->second;
        ERROR_CONTINUE_EX(pObj != NULL);

        if (pObj->GetCamp() != dwCamp && dwCamp != 0)
        {
            continue;
        }

        pObj->SetBattleResult(nBattleResult);
    }

    return TRUE;
}

BOOL CScene::OnMsgObjectActionReq( NetPacket* pNetPacket )
{
    ObjectActionReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    for(int i = 0; i < Req.actionlist_size(); i++)
    {
        const ActionReqItem& Item = Req.actionlist(i);
        CSceneObject* pSceneObj = GetSceneObject(Item.objectguid());
        if (pSceneObj == NULL || pSceneObj->IsDead())
        {
            continue;
        }
        pSceneObj->ProcessAction(Item);
        m_pMonsterCreator->OnPlayerMove(pSceneObj->m_Pos.m_x, pSceneObj->m_Pos.m_z);
    }

    return TRUE;
}

BOOL CScene::OnMsgSkillCastReq(NetPacket* pNetPacket)
{
    SkillCastReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pSceneObj = GetSceneObject(Req.objectguid());
    ERROR_RETURN_TRUE(pSceneObj != NULL);

    UINT32 dwRetCode = pSceneObj->ProcessSkill(Req);
    if (dwRetCode != MRC_SUCCESSED)
    {
        SkillCastAck Ack;
        Ack.set_objectguid(Req.objectguid());
        Ack.set_retcode(dwRetCode);

        if (pSceneObj->IsRobot())
        {
            return TRUE;
        }

        //未登录的肯定不是玩家，不是玩家就不需要反馈
        if (!pSceneObj->IsEnterCopy())
        {
            return TRUE;
        }

        pSceneObj->SendMsgProtoBuf(MSG_SKILL_CAST_ACK, Ack);
    }

    return TRUE;
}

BOOL CScene::OnMsgObjectChangeNtf(NetPacket* pNetPacket)
{
    ObjectChangeNotify Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pPlayer = GetPlayer(Req.roleid());
    ERROR_RETURN_TRUE(pPlayer != NULL);

    if (Req.changetype() == ECT_EQUIP)
    {
        pPlayer->ChangeEquip((INT32)Req.intvalue1(), (UINT32)Req.intvalue2());
    }
    else if (Req.changetype() == ECT_MOUNT)
    {
        pPlayer->ChangeMount((UINT32)Req.intvalue2());
    }

    return TRUE;
}

BOOL CScene::OnMsgMountRidingReq(NetPacket* pNetPacket)
{
    Msg_RidingMountReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pSceneObj = GetSceneObject(Req.objectguid());

    ERROR_RETURN_TRUE(pSceneObj != NULL);

    pSceneObj->SetRiding(!pSceneObj->m_bRiding);

    Msg_RidingMountAck Ack;

    Ack.set_retcode(MRC_SUCCESSED);

    pSceneObj->SendMsgProtoBuf(MSG_MOUNT_RIDING_ACK, Ack);

    return TRUE;
}

BOOL CScene::OnMsgRoleRebornReq(NetPacket* pNetPacket)
{
    Msg_RoleRebornReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pPlayer = GetPlayer(Req.objectguid());
    if (pPlayer == NULL)
    {
        return TRUE;
    }

    if (!pPlayer->IsDead())
    {
        return TRUE;
    }

    //复活
    pPlayer->Revive();

    Msg_RoleRebornAck Ack;
    Ack.set_retcode(MRC_SUCCESSED);
    pPlayer->SendMsgProtoBuf(MSG_ROLE_REBORN_ACK, Ack);

    /*
    //先把玩家的完整包组装好
    ObjectNewNty Nty;
    pPlayer->SaveNewData(Nty);

    char szBuff[10240] = { 0 };
    ERROR_RETURN_FALSE(Nty.ByteSize() < 10240);
    Nty.SerializePartialToArray(szBuff, Nty.ByteSize());

    for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if (!pOther->IsConnected())
        {
            continue;
        }

        if (pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
        {
            continue;
        }

        pOther->SendMsgRawData(MSG_OBJECT_NEW_NTF, szBuff, Nty.ByteSize());
    }

    */

    return TRUE;
}

BOOL CScene::OnMsgRoleDisconnect(NetPacket* pNetPacket)
{
    RoleDisconnectReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pPlayer = GetPlayer(Req.roleid());
    if (pPlayer == NULL)
    {
        return TRUE;
    }

    pPlayer->SetConnectID(0, 0);

    UpdateAiController(pPlayer->GetObjectGUID());

    m_pSceneLogic->OnPlayerLeave(pPlayer, TRUE);

    //ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_DISCONNECT_NTY, pHeader->u64TargetID, 0, Req);

    return TRUE;
}

BOOL CScene::OnMsgHeartBeatReq(NetPacket* pNetPacket)
{
    HeartBeatReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pPlayer = GetPlayer(pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pPlayer != NULL);

    HeartBeatAck Ack;
    Ack.set_timestamp(Req.timestamp());
    Ack.set_servertime(0);

    pPlayer->SendMsgProtoBuf(MSG_HEART_BEAT_ACK, Ack);

    return TRUE;
}

BOOL CScene::OnMsgUseHpBottleReq(NetPacket* pNetPacket)
{
    UseHpBottleReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pObject = GetPlayer(pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pObject != NULL);



    return TRUE;
}

BOOL CScene::OnMsgUseMpBottleReq(NetPacket* pNetPacket)
{
    UseMpBottleReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pObject = GetPlayer(pHeader->u64TargetID);
    ERROR_RETURN_TRUE(pObject != NULL);

    return TRUE;
}

BOOL CScene::BroadNewObject(CSceneObject* pSceneObject)
{
    if(GetConnectCount() <= 0)
    {
        return TRUE;
    }

    //先把玩家的完整包组装好
    ObjectNewNty Nty;
    pSceneObject->SaveNewData(Nty);

    char szBuff[10240] = {0};
    ERROR_RETURN_FALSE(Nty.ByteSize() < 10240);
    Nty.SerializePartialToArray(szBuff, Nty.ByteSize());

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(!pOther->IsConnected())
        {
            continue;
        }

        if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
        {
            continue;
        }

        pOther->SendMsgRawData(MSG_OBJECT_NEW_NTF, szBuff, Nty.ByteSize());
    }

    return TRUE;
}

BOOL CScene::BroadMessage(INT32 nMsgID, const google::protobuf::Message& pdata)
{
    char szBuff[10240] = { 0 };
    ERROR_RETURN_FALSE(pdata.ByteSize() < 10240);
    ERROR_RETURN_FALSE(pdata.SerializePartialToArray(szBuff, pdata.ByteSize()));

    BroadMessageNotify Nty;
    Nty.set_msgdata(szBuff, pdata.ByteSize());
    Nty.set_msgid(nMsgID);

    for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        ERROR_RETURN_FALSE(pObj != NULL);

        if (!pObj->IsConnected())
        {
            continue;
        }

        Nty.add_connid(pObj->m_dwClientConnID);
    }

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetProxyConnID(), MSG_BROAD_MESSAGE_NOTIFY, 0, 0, Nty);

    return TRUE;
}

//玩家主动中止副本
BOOL CScene::OnMsgLeaveSceneReq(NetPacket* pNetPacket)
{
    LeaveSceneReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pPlayer = GetPlayer(Req.roleid());
    ERROR_RETURN_TRUE(pPlayer != NULL);

    BroadRemoveObject(pPlayer);

    DeletePlayer(pPlayer->GetObjectGUID());

    CSceneObject* pPet = GetSceneObject(pPlayer->m_uPetGuid);
    if (pPet != NULL)
    {
        BroadRemoveObject(pPet);
        DeleteMonster(pPlayer->m_uPetGuid);
    }

    CSceneObject* pPartner = GetSceneObject(pPlayer->m_uPartnerGuid);
    if (pPartner != NULL)
    {
        BroadRemoveObject(pPartner);
        DeleteMonster(pPlayer->m_uPartnerGuid);
    }

    return TRUE;
}

BOOL CScene::OnMsgAbortSceneReq(NetPacket* pNetPacket)
{
    //如果是组队，就退出, 不删除
    //如果是单人，就直接结算
    //如果是pvp 2人，就直接结算．
    //如果是多人pvp，就退出自己, 并且删掉
    AbortSceneReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    CSceneObject* pSceneObject = GetPlayer(Req.roleid());
    ERROR_RETURN_TRUE(pSceneObject != NULL);

    m_pSceneLogic->OnPlayerLeave(pSceneObject, FALSE);

    return TRUE;
}

BOOL CScene::OnUpdate( UINT64 uTick )
{
    if(IsFinished())
    {
        //己经结束不再处理
        return TRUE;
    }

    BroadHitEffect();

    for(auto itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); ++itor)
    {
        CSceneObject* pSceneObject = itor->second;
        ERROR_CONTINUE_EX(pSceneObject != NULL);
        pSceneObject->OnUpdate(uTick);
    }

    for (auto itor = m_mapMonster.begin(); itor != m_mapMonster.end(); ++itor)
    {
        CSceneObject* pSceneObject = itor->second;
        ERROR_CONTINUE_EX(pSceneObject != NULL);
        pSceneObject->OnUpdate(uTick);
    }

    //更新场景里所有子弹的状态
    UpdateBulletStatus(uTick);

    //同步所有对象的状态
    SyncObjectStatus();

    //把玩家死亡都同步一下
    for(auto itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); ++itor)
    {
        CSceneObject* pSceneObject = itor->second;
        ERROR_CONTINUE_EX(pSceneObject != NULL);
        if(pSceneObject->GetHp() <= 0 && !pSceneObject->IsDead())
        {
            pSceneObject->SetDead(TRUE);
            pSceneObject->SetActionID(AT_DEAD);
            m_pSceneLogic->OnObjectDie(pSceneObject);
            BroadDieNotify(pSceneObject->GetObjectGUID());
        }
    }

    //把怪物死亡同步一下
    for (auto itor = m_mapMonster.begin(); itor != m_mapMonster.end(); ++itor)
    {
        CSceneObject* pSceneObject = itor->second;
        ERROR_CONTINUE_EX(pSceneObject != NULL);
        if(pSceneObject->GetHp() <= 0 && !pSceneObject->IsDead())
        {
            pSceneObject->SetDead(TRUE);
            pSceneObject->SetActionID(AT_DEAD);
            m_pMonsterCreator->OnObjectDie(pSceneObject);
            m_pSceneLogic->OnObjectDie(pSceneObject);
            BroadDieNotify(pSceneObject->GetObjectGUID());
        }
    }

    m_pMonsterCreator->OnUpdate(uTick);

    m_pSceneLogic->Update(uTick);

    RemoveDeadObject();

    return TRUE;
}

BOOL CScene::CreateSceneLogic(UINT32 dwCopyType)
{
    ERROR_RETURN_FALSE(dwCopyType != 0);
    switch (dwCopyType)
    {
        case CPT_MAIN:
            m_pSceneLogic = new SceneLogic_Normal(this);
            break;

        case CPT_CITY:
            m_pSceneLogic = new SceneLogic_City(this);
            break;
        default:
        {
            m_pSceneLogic = new SceneLogic_None(this);
        }
    }

    return (m_pSceneLogic != NULL);
}

BOOL CScene::DestroySceneLogic(UINT32 dwCopyType)
{
    switch (dwCopyType)
    {
        case CPT_MAIN:
        {
            SceneLogic_Normal* pTemp = (SceneLogic_Normal*)m_pSceneLogic;
            delete pTemp;
        }
        break;
        case CPT_CITY:
        {
            SceneLogic_City* pTemp = (SceneLogic_City*)m_pSceneLogic;
            delete pTemp;
        }
        break;
        default:
        {
            SceneLogic_None* pTemp = (SceneLogic_None*)m_pSceneLogic;
            delete pTemp;
        }
    }

    return (m_pSceneLogic != NULL);
}

BOOL CScene::OnMsgTransRoleDataReq(NetPacket* pNetPacket)
{
    ERROR_RETURN_FALSE(m_dwCopyGuid != 0);
    ERROR_RETURN_FALSE(m_dwCopyID != 0);
    ERROR_RETURN_FALSE(m_dwCopyType != 0);
    ERROR_RETURN_FALSE(CGameService::GetInstancePtr()->GetServerID() != 0);

    TransferDataReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

    for (int i = 0; i < Req.transdatas_size(); i++)
    {
        const TransferDataItem& Item = Req.transdatas(i);
        CreatePlayer(Item.roledata(), pHeader->u64TargetID, Item.camp());

        //是否有宠物
        if(Item.has_petdata())
        {
            CreatePet(Item.petdata(), pHeader->u64TargetID, Item.camp());
        }

        //是否有伙伴
        if (Item.has_partnerdata())
        {
            CreatePartner(Item.partnerdata(), pHeader->u64TargetID, Item.camp());
        }
    }

    //检查人齐没齐，如果齐了，就全部发准备好了的消息
    //有的副本不需要等人齐，有人就可以进

    TransferDataAck Ack;
    Ack.set_copyguid(m_dwCopyGuid);
    Ack.set_copyid(m_dwCopyID);
    Ack.set_roleid(pHeader->u64TargetID);
    Ack.set_serverid(CGameService::GetInstancePtr()->GetServerID());
    Ack.set_retcode(MRC_SUCCESSED);
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_TRANSFER_DATA_ACK, pHeader->u64TargetID, 0, Ack);
    return TRUE;
}

BOOL CScene::OnMsgEnterSceneReq(NetPacket* pNetPacket)
{
    EnterSceneReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(pHeader->u64TargetID != 0);

    CSceneObject* pSceneObj = GetPlayer(Req.roleid());
    ERROR_RETURN_TRUE(pSceneObj != NULL);

    pSceneObj->SetConnectID(pNetPacket->m_nConnID, (UINT32)pHeader->u64TargetID);
    pSceneObj->SetEnterCopy();
    m_pSceneLogic->OnPlayerEnter(pSceneObj);

    m_dwLoginNum ++;

    if (m_uStartTime <= 0)
    {
        //对于普通的副本，有入进入就表示开始
        m_uStartTime = CommonFunc::GetCurrTime();
    }

    //发比较全的自己的信息
    EnterSceneAck Ack;
    Ack.set_copyguid(m_dwCopyGuid);
    Ack.set_copyid(m_dwCopyID);
    Ack.set_roleid(Req.roleid());
    Ack.set_rolename(pSceneObj->m_strName);
    Ack.set_actorid(pSceneObj->m_dwActorID);
    Ack.set_retcode(MRC_SUCCESSED);

    Ack.set_hp(pSceneObj->m_Propertys[EA_HP]);
    Ack.set_mp(pSceneObj->m_Propertys[EA_MP]);
    Ack.set_hpmax(pSceneObj->m_Propertys[EA_HP_MAX]);
    Ack.set_mpmax(pSceneObj->m_Propertys[EA_MP_MAX]);
    Ack.set_speed(pSceneObj->m_Propertys[EA_SPEED]);

    Ack.set_camp(pSceneObj->m_dwCamp);

    if (pSceneObj->m_bRiding)
    {
        Ack.set_mountid(pSceneObj->m_dwMountID);
    }
    else
    {
        Ack.set_mountid(0 - pSceneObj->m_dwMountID);
    }

    Ack.set_x(pSceneObj->m_Pos.m_x);
    Ack.set_y(pSceneObj->m_Pos.m_y);
    Ack.set_z(pSceneObj->m_Pos.m_z);
    Ack.set_ft(0);

    for (int i = 0; i < EQUIP_MAX_NUM; i++)
    {
        Ack.add_equips(pSceneObj->m_Equips[i]);
    }

    for (int i = 0; i < pSceneObj->m_vtNormals.size(); i++)
    {
        SkillItem* pSkillItem = Ack.add_skills();
        pSkillItem->set_keypos(pSceneObj->m_vtNormals[i].nKeyPos);
        pSkillItem->set_level(pSceneObj->m_vtNormals[i].nLevel);
        pSkillItem->set_skillid(pSceneObj->m_vtNormals[i].dwSkillID);
    }

    for (int i = 0; i < pSceneObj->m_vtSpecials.size(); i++)
    {
        SkillItem* pSkillItem = Ack.add_skills();
        pSkillItem->set_keypos(pSceneObj->m_vtSpecials[i].nKeyPos);
        pSkillItem->set_level(pSceneObj->m_vtSpecials[i].nLevel);
        pSkillItem->set_skillid(pSceneObj->m_vtSpecials[i].dwSkillID);
    }

    pSceneObj->SendMsgProtoBuf(MSG_ENTER_SCENE_ACK, Ack);

    UpdateAiController(0);

    SendAllNewObjectToPlayer(pSceneObj);

    return TRUE;
}

UINT32 CScene::GetCopyGuid()
{
    return m_dwCopyGuid;
}

UINT32 CScene::GetCopyID()
{
    return m_dwCopyID;
}

UINT32 CScene::GetCopyType()
{
    return m_dwCopyType;
}

BOOL CScene::SendAllNewObjectToPlayer( CSceneObject* pSceneObject )
{
    //先把玩家的完整包组装好
    ObjectNewNty Nty;

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
        {
            continue;
        }

        //如果是机器人，必须有实控人
        if (pOther->IsRobot())
        {
            ERROR_RETURN_FALSE(pOther->m_uControlerID != 0);
        }

        pOther->SaveNewData(Nty);
    }

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
        {
            continue;
        }

        //宠物必须有控制人
        ERROR_RETURN_FALSE(pOther->m_uControlerID != 0);

        pOther->SaveNewData(Nty);
    }

    if(Nty.newlist_size() <= 0)
    {
        return TRUE;
    }

    pSceneObject->SendMsgProtoBuf(MSG_OBJECT_NEW_NTF, Nty);

    return TRUE;
}

BOOL CScene::BroadDieNotify(UINT64 uObjectID)
{
    ObjectDieNotify Nty;
    Nty.set_objectguid(uObjectID);
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pSceneObject = itor->second;
        ERROR_RETURN_FALSE(pSceneObject != NULL);

        if(pSceneObject->IsConnected())
        {
            pSceneObject->SendMsgProtoBuf(MSG_OBJECT_DIE_NOTIFY, Nty);
        }
    }

    return TRUE;
}

BOOL CScene::UpdateBulletStatus(UINT64 uTick)
{
    for (auto itor = m_mapBullet.begin(); itor != m_mapBullet.end();)
    {
        CBulletObject* pBulletObject = itor->second;
        ERROR_CONTINUE_EX(pBulletObject != NULL);
        pBulletObject->OnUpdate(uTick);

        if (pBulletObject->IsFinished())
        {
            itor = m_mapBullet.erase(itor);
        }
        else
        {
            ++itor;
        }
    }

    return TRUE;
}

BOOL CScene::BackToMainCity(UINT64 uRoleID)
{
    AbortSceneNty Nty;
    Nty.set_serverid(0);
    Nty.set_roleid(uRoleID);
    Nty.set_copyguid(m_dwCopyGuid);
    Nty.set_copyid(m_dwCopyID);
    Nty.set_copytype(m_dwCopyType);
    Nty.set_param(0);
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_ABORT_SCENE_NTF, 0, 0, Nty);

    return TRUE;
}

INT32 CScene::GetPlayerCount()
{
    return (INT32)m_mapPlayer.size();
}

INT32 CScene::GetConnectCount()
{
    INT32 nCount = 0;
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->IsConnected())
        {
            nCount += 1;
        }
    }

    return nCount;
}

BOOL CScene::BroadRemoveObject( CSceneObject* pSceneObject )
{
    ObjectRemoveNty Nty;
    Nty.add_removelist(pSceneObject->GetObjectGUID());

    char szBuff[10240] = {0};
    ERROR_RETURN_FALSE(Nty.ByteSize() < 10240);
    Nty.SerializePartialToArray(szBuff, Nty.ByteSize());

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->GetObjectGUID() == pSceneObject->GetObjectGUID())
        {
            continue;
        }

        if(!pOther->IsConnected())
        {
            continue;
        }

        pOther->SendMsgRawData(MSG_OBJECT_REMOVE_NTF, szBuff, Nty.ByteSize());
    }

    return TRUE;
}

CSceneObject* CScene::GetPlayer( UINT64 uID )
{
    std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.find(uID);
    if(itor != m_mapPlayer.end())
    {
        return itor->second;
    }

    return NULL;
}

BOOL CScene::AddPlayer( CSceneObject* pSceneObject )
{
    ERROR_RETURN_FALSE(pSceneObject != NULL);

    ERROR_RETURN_FALSE(pSceneObject->GetObjectGUID() != 0);

    m_mapPlayer.insert(std::make_pair(pSceneObject->GetObjectGUID(), pSceneObject));

    return TRUE;
}

VOID CScene::DeletePlayer(UINT64 uID)
{
    std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.find(uID);
    if(itor != m_mapPlayer.end())
    {
        CSceneObject* pObject = itor->second;
        delete pObject;
        m_mapPlayer.erase(itor);
    }
    else
    {
        CLog::GetInstancePtr()->LogError("Error CScene::DeletePlayer cannot find player id:%d", uID);
    }

    return ;
}

BOOL CScene::AddMonster(CSceneObject* pSceneObject)
{
    ERROR_RETURN_FALSE(pSceneObject != NULL);

    ERROR_RETURN_FALSE(pSceneObject->GetObjectGUID() != 0);

    m_mapMonster.insert(std::make_pair(pSceneObject->GetObjectGUID(), pSceneObject));

    return TRUE;

}

VOID CScene::DeleteMonster(UINT64 uID)
{
    std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.find(uID);
    if(itor != m_mapMonster.end())
    {
        CSceneObject* pObject = itor->second;
        delete pObject;
        m_mapMonster.erase(itor);
    }
    else
    {
        CLog::GetInstancePtr()->LogError("Error CScene::DeleteMonster cannot find player id:%d", uID);
    }
}

CSceneObject* CScene::GetSceneObject(UINT64 uID)
{
    std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.find(uID);
    if(itor != m_mapPlayer.end())
    {
        return itor->second;
    }

    itor = m_mapMonster.find(uID);
    if(itor != m_mapMonster.end())
    {
        return itor->second;
    }

    return NULL;
}

BOOL CScene::RemoveDeadObject()
{
    //为了方便结算时，生成汇报的结果，有些角色一般不删除
    //如玩家角色, 伙伴，宠物之类的，
    //由于目前不考虑战斗数据统计，就只保留角色
    /*
    for (auto itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); )
    {
        CSceneObject* pSceneObject = itor->second;
        ERROR_CONTINUE_EX(pSceneObject != NULL);
        if (pSceneObject->IsDead())
        {
            delete pSceneObject;
            itor = m_mapPlayer.erase(itor);
        }
        else
        {
            itor++;
        }
    }
    */

    for (auto itor = m_mapMonster.begin(); itor != m_mapMonster.end(); )
    {
        CSceneObject* pSceneObject = itor->second;
        ERROR_CONTINUE_EX(pSceneObject != NULL);
        if (pSceneObject->IsDead())
        {
            delete pSceneObject;
            itor = m_mapMonster.erase(itor);
        }
        else
        {
            itor++;
        }
    }

    return TRUE;
}

//场景里有玩家进入和退出的时候，都要进行控制人检查
BOOL CScene::UpdateAiController(UINT64 uFilterID)
{
    UINT64 u64ControllerID = SelectController(uFilterID);
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->GetObjectGUID() == uFilterID)
        {
            continue;
        }

        if(pOther->IsRobot())
        {
            if(pOther->GetControllerID() == uFilterID)
            {
                pOther->SetControllerID(u64ControllerID);
            }
        }
    }

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);

        if(pOther->GetControllerID() == uFilterID)
        {
            pOther->SetControllerID(u64ControllerID);
        }
    }

    return TRUE;
}

UINT64 CScene::SelectController(UINT64 uFilterID)
{
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pOther = itor->second;
        ERROR_RETURN_FALSE(pOther != NULL);
        if(pOther->GetObjectGUID() == uFilterID)
        {
            continue;
        }

        if(pOther->IsRobot())
        {
            continue;
        }

        if(!pOther->IsConnected())
        {
            continue;
        }

        return pOther->GetObjectGUID();
    }

    return 0;
}

BOOL CScene::SelectTargets(std::vector<CSceneObject*>& vTargets, UINT64 uExcludeID, UINT32 dwCamp, EHitShipType hitType, Vector3D hitPos, FLOAT fHitDir, ERangeType rangeType, FLOAT RangeParams[5])
{
    switch (rangeType)
    {
        case ERT_OBJECTS:
        {
            //什么都不需要做，直接使用客户端传过来的目标列表
        }
        break;
        case ERT_CIRCLE:
        {
            FLOAT radius  = RangeParams[0];
            FLOAT hAngle  = RangeParams[1];
            FLOAT height  = RangeParams[2];
            FLOAT offsetX = RangeParams[3];
            FLOAT offsetZ = RangeParams[4];

            Vector3D hitPoint = hitPos;

            hitPoint = hitPoint + Vector3D(offsetX, 0, offsetZ);

            for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
            {
                CSceneObject* pObject = itor->second;
                ERROR_RETURN_FALSE(pObject != NULL);

                if (uExcludeID == pObject->GetObjectGUID())
                {
                    continue;
                }

                if (hitType == EHST_ENEMY && pObject->GetCamp() == dwCamp)
                {
                    continue;
                }

                if (hitType == EHST_FRIEND && pObject->GetCamp() != dwCamp)
                {
                    continue;
                }

                if (pObject->IsInCircle(hitPoint, radius, height))
                {
                    vTargets.emplace_back(pObject);
                }
            }

            for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
            {
                CSceneObject* pObject = itor->second;
                ERROR_RETURN_FALSE(pObject != NULL);

                if (uExcludeID == pObject->GetObjectGUID())
                {
                    continue;
                }

                if (hitType == EHST_ENEMY && pObject->GetCamp() == dwCamp)
                {
                    continue;
                }

                if (hitType == EHST_FRIEND && pObject->GetCamp() != dwCamp)
                {
                    continue;
                }

                if (pObject->IsInCircle(hitPoint, radius, height))
                {
                    vTargets.emplace_back(pObject);
                }
            }

        }
        break;
        case ERT_CYLINDER:
        {
            FLOAT radius  = RangeParams[0];
            FLOAT hAngle  = RangeParams[1];
            FLOAT height  = RangeParams[2];
            FLOAT offsetX = RangeParams[3];
            FLOAT offsetZ = RangeParams[4];

            Vector3D hitPoint = hitPos;
            hitPoint = hitPoint + Vector3D(offsetX, 0, offsetZ);

            for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
            {
                CSceneObject* pObject = itor->second;
                ERROR_RETURN_FALSE(pObject != NULL);
                if (uExcludeID == pObject->GetObjectGUID())
                {
                    continue;
                }

                if (hitType == EHST_ENEMY && pObject->GetCamp() == dwCamp)
                {
                    continue;
                }

                if (hitType == EHST_FRIEND && pObject->GetCamp() != dwCamp)
                {
                    continue;
                }

                if (pObject->IsInSector(hitPoint, fHitDir, radius, hAngle))
                {
                    vTargets.emplace_back(pObject);
                }
            }

            for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
            {
                CSceneObject* pObject = itor->second;
                ERROR_RETURN_FALSE(pObject != NULL);

                if (uExcludeID == pObject->GetObjectGUID())
                {
                    continue;
                }

                if (hitType == EHST_ENEMY && pObject->GetCamp() == dwCamp)
                {
                    continue;
                }

                if (hitType == EHST_FRIEND && pObject->GetCamp() != dwCamp)
                {
                    continue;
                }

                if (pObject->IsInSector(hitPoint, fHitDir, radius, hAngle))
                {
                    vTargets.emplace_back(pObject);
                }
            }
        }
        break;
        case ERT_BOX:
        {
            FLOAT length  = RangeParams[0];
            FLOAT width   = RangeParams[1];
            FLOAT height  = RangeParams[2];
            FLOAT offsetX = RangeParams[3];
            FLOAT offsetZ = RangeParams[4];

            Vector3D hitPoint = hitPos;
            hitPoint = hitPoint + Vector3D(offsetX, 0, offsetZ);

            for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
            {
                CSceneObject* pObject = itor->second;
                ERROR_RETURN_FALSE(pObject != NULL);

                if (uExcludeID == pObject->GetObjectGUID())
                {
                    continue;
                }

                if (hitType == EHST_ENEMY && pObject->GetCamp() == dwCamp)
                {
                    continue;
                }

                if (hitType == EHST_FRIEND && pObject->GetCamp() != dwCamp)
                {
                    continue;
                }

                if (pObject->IsInSquare(hitPoint, fHitDir, length, width))
                {
                    vTargets.emplace_back(pObject);
                }
            }

            for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
            {
                CSceneObject* pObject = itor->second;
                ERROR_RETURN_FALSE(pObject != NULL);

                if (uExcludeID == pObject->GetObjectGUID())
                {
                    continue;
                }

                if (hitType == EHST_ENEMY && pObject->GetCamp() == dwCamp)
                {
                    continue;
                }

                if (hitType == EHST_FRIEND && pObject->GetCamp() != dwCamp)
                {
                    continue;
                }

                if (pObject->IsInSquare(hitPoint, fHitDir, length, width))
                {
                    vTargets.emplace_back(pObject);
                }
            }
        }
    }

    if (vTargets.size() > 0)
    {
        return TRUE;
    }

    return FALSE;
}



BOOL CScene::IsFinished()
{
    return m_bFinished;
}

VOID CScene::SetFinished()
{
    m_bFinished = TRUE;
}

BOOL CScene::IsAllDataReady()
{
    if(m_mapPlayer.size() >= m_dwPlayerNum)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CScene::IsAllLoginReady()
{
    if(!IsAllDataReady())
    {
        return FALSE;
    }

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        if(!pObj->IsEnterCopy())
        {
            return FALSE;
        }
    }

    return TRUE;
}

UINT64 CScene::GetStartTime()
{
    return m_uStartTime;
}

UINT64 CScene::GetCreateTime()
{
    return m_uCreateTime;
}

UINT64 CScene::GetTotalTime()
{
    return m_uTotalTime;
}

UINT64 CScene::GetLastTick()
{
    return m_dwLastTick;
}

BOOL CScene::SetLastTick(UINT64 uTick)
{
    m_dwLastTick = uTick;
    return TRUE;
}

UINT64 CScene::GenNewGuid()
{
    m_uMaxGuid += 1;
    return m_uMaxGuid;
}


BOOL CScene::SyncObjectStatus()
{
    ObjectActionNty ActionNty;
    for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        ERROR_RETURN_FALSE(pObj != NULL);

        if (!pObj->IsChanged())
        {
            continue;
        }

        pObj->SaveUpdateData(ActionNty);
    }


    for (std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        ERROR_RETURN_FALSE(pObj != NULL);

        if (!pObj->IsChanged())
        {
            continue;
        }

        pObj->SaveUpdateData(ActionNty);
    }

    if (ActionNty.actionlist_size() <= 0)
    {
        return TRUE;
    }

    char szBuff[10240] = {0};
    ERROR_RETURN_FALSE(ActionNty.ByteSize() < 10240);
    ERROR_RETURN_FALSE(ActionNty.SerializePartialToArray(szBuff, ActionNty.ByteSize()));

    BroadMessageNotify Nty;
    Nty.set_msgdata(szBuff, ActionNty.ByteSize());
    Nty.set_msgid(MSG_OBJECT_CHANGE_NTF);
    ActionNty.Clear();

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        ERROR_RETURN_FALSE(pObj != NULL);
        if(!pObj->IsConnected())
        {
            continue;
        }

        Nty.add_connid(pObj->m_dwClientConnID);
    }

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetProxyConnID(), MSG_BROAD_MESSAGE_NOTIFY, 0, 0, Nty);

    return TRUE;
}


CSceneObject* CScene::CreateMonster(UINT32 dwActorID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft)
{
    StActorInfo* pActorInfo = CStaticData::GetInstancePtr()->GetActorInfo(dwActorID);
    ERROR_RETURN_NULL(pActorInfo != NULL);
    CSceneObject* pObject = new CSceneObject(GenNewGuid(), this);

    pObject->m_dwCamp = dwCamp;
    pObject->m_dwObjType = OT_MONSTER;
    pObject->m_dwActorID = dwActorID;
    pObject->m_strName = pActorInfo->strName;
    pObject->m_dwLevel =  pActorInfo->InitLevel;
    for(int i = 0; i < PROPERTY_NUM; i++)
    {
        pObject->m_Propertys[i] = pActorInfo->Propertys[i];
    }

    pObject->InitSkills();
    //指定位置
    pObject->SetPos(x, y, z, ft);

    m_pSceneLogic->OnObjectCreate(pObject);

    AddMonster(pObject);

    pObject->m_uControlerID = SelectController(0);

    BroadNewObject(pObject);

    return pObject;
}

CSceneObject* CScene::CreatePlayer(const TransRoleData& roleData, UINT64 uHostID, UINT32 dwCamp )
{
    CSceneObject* pObject = GetPlayer(roleData.roleid());
    if(pObject == NULL)
    {
        pObject = new CSceneObject(roleData.roleid(), this);
        AddPlayer(pObject);
    }
    //根据数据创建宠物，英雄
    pObject->m_dwCamp = dwCamp;
    pObject->m_dwObjType = OT_PLAYER;
    pObject->m_dwActorID = roleData.actorid();
    pObject->m_strName = roleData.name();
    pObject->m_dwLevel = roleData.level();
    pObject->m_dwMountID = roleData.mountid();
    pObject->m_bRobot = roleData.robot();

    for (int i = 0; i < roleData.equips_size(); i++)
    {
        pObject->m_Equips[i] = roleData.equips(i);
    }

    for(int i = 0; i < roleData.propertys_size(); i++)
    {
        pObject->m_Propertys[i] = roleData.propertys(i);
    }

    pObject->InitSkills(roleData.skills());

    m_pSceneLogic->OnObjectCreate(pObject);

    BroadNewObject(pObject);

    return pObject;
}

CSceneObject* CScene::CreatePet(const TransPetData& petData, UINT64 uHostID, UINT32 dwCamp )
{
    StActorInfo* pActorInfo = CStaticData::GetInstancePtr()->GetActorInfo(petData.actorid());
    ERROR_RETURN_NULL(pActorInfo != NULL);
    CSceneObject* pObject = new CSceneObject(petData.petguid(), this);
    ERROR_RETURN_NULL(pObject != NULL);

    //根据数据创建宠物，英雄
    pObject->m_dwCamp = dwCamp;
    pObject->m_uHostGuid = uHostID;
    pObject->m_dwObjType = OT_PET;
    pObject->m_dwActorID = petData.actorid();
    pObject->m_strName = pActorInfo->strName;
    pObject->m_dwLevel = petData.level();
    pObject->m_uControlerID = uHostID;
    for (int i = 0; i < petData.propertys_size(); i++)
    {
        pObject->m_Propertys[i] = petData.propertys(i);
    }

    //设置技能
    pObject->InitSkills(petData.skills());

    //指定坐标 在主人附近
    CSceneObject* pHostObject = GetPlayer(uHostID);
    ERROR_RETURN_NULL(pHostObject != NULL);

    pHostObject->m_uPetGuid = petData.petguid();
    pObject->SetPos(pHostObject->m_Pos.m_x + 1, pHostObject->m_Pos.m_y, pHostObject->m_Pos.m_z - 1);
    m_pSceneLogic->OnObjectCreate(pObject);

    AddMonster(pObject);

    BroadNewObject(pObject);

    return pObject;
}

CSceneObject* CScene::CreatePartner(const TransPartnerData& partnerData, UINT64 uHostID, UINT32 dwCamp  )
{
    StActorInfo* pActorInfo = CStaticData::GetInstancePtr()->GetActorInfo(partnerData.actorid());
    ERROR_RETURN_NULL(pActorInfo != NULL);
    CSceneObject* pObject = new CSceneObject(partnerData.partnerguid(), this);
    ERROR_RETURN_NULL(pObject != NULL);

    //根据数据创建宠物，英雄
    pObject->m_dwCamp = dwCamp;
    pObject->m_uHostGuid = uHostID;
    pObject->m_dwObjType = OT_PARTNER;
    pObject->m_dwActorID = partnerData.actorid();
    pObject->m_strName = pActorInfo->strName;
    pObject->m_dwLevel = partnerData.level();
    pObject->m_uControlerID = uHostID;
    for (int i = 0; i < partnerData.propertys_size(); i++)
    {
        pObject->m_Propertys[i] = partnerData.propertys(i);
    }

    //设置技能
    pObject->InitSkills(partnerData.skills());

    //指定坐标 在主人附近
    CSceneObject* pHostObject = GetPlayer(uHostID);
    ERROR_RETURN_NULL(pHostObject != NULL);

    pHostObject->m_uPartnerGuid = partnerData.partnerguid();
    pObject->SetPos(pHostObject->m_Pos.m_x - 1, pHostObject->m_Pos.m_y, pHostObject->m_Pos.m_z + 1);

    m_pSceneLogic->OnObjectCreate(pObject);

    AddMonster(pObject);

    BroadNewObject(pObject);

    return pObject;
}

CSceneObject* CScene::CreateSummon(UINT32 dwActorID, UINT64 uSummonerID, UINT32 dwCamp, FLOAT x, FLOAT y, FLOAT z, FLOAT ft)
{
    StActorInfo* pActorInfo = CStaticData::GetInstancePtr()->GetActorInfo(dwActorID);
    ERROR_RETURN_NULL(pActorInfo != NULL);
    CSceneObject* pObject = new CSceneObject(GenNewGuid(), this);

    //根据数据创建宠物，英雄
    pObject->m_dwCamp = dwCamp;
    pObject->m_uSummonerID = uSummonerID;
    pObject->m_dwObjType = OT_SUMMON;
    pObject->m_dwActorID = dwActorID;
    pObject->m_strName = pActorInfo->strName;
    pObject->m_dwLevel = pActorInfo->InitLevel;
    for (int i = 0; i < PROPERTY_NUM; i++)
    {
        pObject->m_Propertys[i] = pActorInfo->Propertys[i];
    }

    pObject->InitSkills();

    //指定位置
    pObject->SetPos(x, y, z, ft);

    m_pSceneLogic->OnObjectCreate(pObject);

    AddMonster(pObject);

    BroadNewObject(pObject);

    return pObject;
}

CBulletObject* CScene::CreateBullet(UINT32 dwBulletID, StBulletInfo* pBulletInfo, CSkillObject* pSkillObject, Vector3D startPos)
{
    CBulletObject* pBullet = new CBulletObject(GenNewGuid(), pBulletInfo, pSkillObject, startPos);

    m_mapBullet.insert(std::make_pair(pBullet->m_uGuid, pBullet));

    return pBullet;
}

BOOL CScene::IsCampAllDie(UINT32 dwCamp)
{
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        if((pObj != NULL) && (pObj->GetCamp() == dwCamp))
        {
            if(!pObj->IsDead() && pObj->m_bIsCampCheck)
            {
                return FALSE;
            }
        }
    }

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        if((pObj != NULL) && (pObj->GetCamp() == dwCamp))
        {
            if(!pObj->IsDead() && pObj->m_bIsCampCheck)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL CScene::IsMonsterAllDie()
{
    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapMonster.begin(); itor != m_mapMonster.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        if (pObj == NULL)
        {
            continue;
        }

        if (pObj->GetObjType() != OT_MONSTER)
        {
            continue;
        }

        if (!pObj->m_bIsCampCheck)
        {
            continue;
        }

        if(!pObj->IsDead())
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CScene::IsMonsterAllGen()
{
    return m_pMonsterCreator->IsAllFinished();
}

BOOL CScene::ReadSceneXml()
{
    StCopyInfo* pCopyInfo = CStaticData::GetInstancePtr()->GetCopyInfo(m_dwCopyID);
    ERROR_RETURN_FALSE(pCopyInfo != NULL);
    pCopyInfo->strXml = CommonConvert::IntToString(m_dwCopyID) + ".xml";
    rapidxml::xml_document<char>* pXmlDoc = CSceneXmlManager::GetInstancePtr()->GetXmlDocument(pCopyInfo->strXml);
    ERROR_RETURN_FALSE(pXmlDoc != NULL);

    rapidxml::xml_node<char>* pXmlRoot = pXmlDoc->first_node("Root");
    ERROR_RETURN_FALSE(pXmlRoot != NULL);

    ERROR_RETURN_FALSE(m_pSceneLogic->ReadFromXml(pXmlRoot));

    auto pCreatorNode = pXmlRoot->first_node("MapWaves");
    ERROR_RETURN_FALSE(pCreatorNode != NULL);
    ERROR_RETURN_FALSE(m_pMonsterCreator != NULL);
    ERROR_RETURN_FALSE(m_pMonsterCreator->ReadFromXml(pCreatorNode));

    return TRUE;
}

CSceneObject* CScene::GetOwnPlayer()
{
    std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin();
    if(itor == m_mapPlayer.end())
    {
        return NULL;
    }

    return itor->second;
}

BOOL CScene::SendBattleResult()
{
    BattleResultNty Nty;

    Nty.set_copyguid(m_dwCopyGuid);
    Nty.set_copyid(m_dwCopyID);
    Nty.set_copytype(m_dwCopyType);
    Nty.set_lasttime(CommonFunc::GetCurrTime() - m_uStartTime);
    Nty.set_serverid(CGameService::GetInstancePtr()->GetServerID());

    for(std::map<UINT64, CSceneObject*>::iterator itor = m_mapPlayer.begin(); itor != m_mapPlayer.end(); itor++)
    {
        CSceneObject* pObj = itor->second;
        ERROR_RETURN_FALSE(pObj != NULL);
        ResultPlayer* pResultItem = Nty.add_playerlist();
        pObj->SaveBattleRecord(pResultItem);
    }

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_BATTLE_RESULT_NTY, 0, 0, Nty);

    return TRUE;
}

BOOL CScene::OnMsgBattleChatReq( NetPacket* pNetPacket )
{
    return TRUE;
}



