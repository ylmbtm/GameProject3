#include "stdafx.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "StoreModule.h"
#include "../Message/Msg_Game.pb.h"
#include "../ServerEngine/PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "../StaticData/StaticStruct.h"
#include "../StaticData/StaticData.h"
#include "BagModule.h"
#include "PlayerObject.h"

CStoreModule::CStoreModule(CPlayerObject* pOwner): CModuleBase(pOwner)
{
    RegisterMessageHanler();
}

CStoreModule::~CStoreModule()
{

}

BOOL CStoreModule::OnCreate(UINT64 u64RoleID)
{

    return TRUE;
}


BOOL CStoreModule::OnDestroy()
{
    return TRUE;
}

BOOL CStoreModule::OnLogin()
{
    return TRUE;
}

BOOL CStoreModule::OnLogout()
{
    return TRUE;
}

BOOL CStoreModule::OnNewDay()
{
    return TRUE;
}

VOID CStoreModule::RegisterMessageHanler()
{
    m_pOwnPlayer->RegisterMessageHandle(MSG_STORE_BUY_REQ, &CStoreModule::OnMsgStoreBuyReq, this);
}

BOOL CStoreModule::ReadFromDBLoginData(DBRoleLoginAck& Ack)
{
    return TRUE;
}

BOOL CStoreModule::SaveToClientLoginData(RoleLoginAck& Ack)
{
    return TRUE;
}

BOOL CStoreModule::NotifyChange()
{
    return TRUE;
}

BOOL CStoreModule::OnMsgStoreBuyReq(NetPacket* pNetPacket)
{
    StoreBuyReq Req;
    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    StStoreItemInfo* pInfo = CStaticData::GetInstancePtr()->GetStoreItemInfo(Req.storeid());
    ERROR_RETURN_TRUE(pInfo != NULL);

    CBagModule* pMoudule = (CBagModule*)m_pOwnPlayer->GetModuleByType(MT_BAG);
    ERROR_RETURN_TRUE(pMoudule != NULL);

    pMoudule->AddItem(pInfo->ItemID, 1);

    return TRUE;
}

