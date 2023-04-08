#include "stdafx.h"
#include "PlayerManager.h"
#include "RoleModule.h"
#include "../ServerData/RoleData.h"
#include "../Message/Msg_ID.pb.h"
#include "TimerManager.h"

CPlayerManager::CPlayerManager()
{
    m_nMaxCacheNum = CConfigFile::GetInstancePtr()->GetIntValue("max_cache_num");
    if (m_nMaxCacheNum <= 10)
    {
        m_nMaxCacheNum = 3000;
    }
    TimerManager::GetInstancePtr()->AddFixTimer(0, 1, &CPlayerManager::ZeroTimer, this);
}

CPlayerManager::~CPlayerManager()
{

}

CPlayerManager* CPlayerManager::GetInstancePtr()
{
    static CPlayerManager _PlayerManager;

    return &_PlayerManager;
}

CPlayerObject* CPlayerManager::CreatePlayer(UINT64 u64RoleID)
{
    CPlayerObject* pPlayer = CreatePlayerByID(u64RoleID);
    return pPlayer;
}

CPlayerObject* CPlayerManager::GetPlayer( UINT64 u64RoleID )
{
    return GetByKey(u64RoleID);
}

CPlayerObject* CPlayerManager::CreatePlayerByID( UINT64 u64RoleID )
{
    return InsertAlloc(u64RoleID);
}

INT32 CPlayerManager::GetOnlineCount()
{
    TNodeTypePtr pNode = MoveFirst();
    if (pNode == NULL)
    {
        return 0;
    }

    INT32 nCount = 0;
    CPlayerObject* pTempObj = NULL;
    for (; pNode != NULL; pNode = MoveNext(pNode))
    {
        pTempObj = pNode->GetValue();
        if (pTempObj == NULL)
        {
            continue;
        }

        if (!pTempObj->IsOnline())
        {
            continue;
        }

        nCount++;
    }

    return nCount;
}

BOOL CPlayerManager::ReleasePlayer( UINT64 u64RoleID )
{
    CPlayerObject* pPlayer = GetByKey(u64RoleID);

    ERROR_RETURN_FALSE(pPlayer != NULL);

    pPlayer->Uninit();

    return Delete(u64RoleID);
}

BOOL CPlayerManager::BroadMessageToAll(INT32 nMsgID, const google::protobuf::Message& pdata)
{
    char szBuff[10240] = { 0 };
    ERROR_RETURN_FALSE(pdata.ByteSize() < 10240);
    ERROR_RETURN_FALSE(pdata.SerializePartialToArray(szBuff, pdata.ByteSize()));

    BroadMessageNotify Nty;
    Nty.set_msgdata(szBuff, pdata.ByteSize());
    Nty.set_msgid(nMsgID);

    TNodeTypePtr pNode = MoveFirst();
    ERROR_RETURN_FALSE(pNode != NULL);

    UINT32 dwProxyID = 0;
    CPlayerObject* pTempObj = NULL;
    for (; pNode != NULL; pNode = MoveNext(pNode))
    {
        pTempObj = pNode->GetValue();
        ERROR_RETURN_FALSE(pTempObj != NULL);

        if (!pTempObj->IsOnline())
        {
            continue;
        }

        Nty.add_connid(pTempObj->m_nClientConnID);

        if(pTempObj->m_nProxyConnID != 0)
        {
            dwProxyID = pTempObj->m_nProxyConnID;
        }
    }

    //因为所有玩家是一个ProxyID
    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwProxyID, MSG_BROAD_MESSAGE_NOTIFY, 0, 0, Nty);

    return TRUE;
}

BOOL CPlayerManager::ZeroTimer(INT32 nParam)
{
    TNodeTypePtr pNode = MoveFirst();
    if (pNode == NULL) //一个人也没有
    {
        return TRUE;
    }

    CPlayerObject* pTempObj = NULL;
    for (; pNode != NULL; pNode = MoveNext(pNode))
    {
        pTempObj = pNode->GetValue();
        ERROR_RETURN_FALSE(pTempObj != NULL);

        if (!pTempObj->IsOnline())
        {
            continue;
        }

        pTempObj->OnNewDay();
    }

    return TRUE;
}

BOOL CPlayerManager::OnUpdate(UINT64 uTick)
{
    if (GetCount() <= 0)
    {
        return TRUE;
    }

    UINT64 uMinLeaveTime = 0x0fffffffff;
    UINT64 uReleaseRoleID = 0;

    TNodeTypePtr pNode = MoveFirst();
    ERROR_RETURN_FALSE(pNode != NULL);

    CPlayerObject* pTempObj = NULL;
    for (; pNode != NULL; pNode = MoveNext(pNode))
    {
        pTempObj = pNode->GetValue();
        ERROR_RETURN_FALSE(pTempObj != NULL);

        if (pTempObj->IsOnline())
        {
            pTempObj->NotifyChange();
        }
        else
        {
            CRoleModule* pRoleModule = (CRoleModule*)pTempObj->GetModuleByType(MT_ROLE);
            ERROR_RETURN_FALSE(pRoleModule != NULL);

            if (uMinLeaveTime > pRoleModule->m_pRoleDataObject->m_uLogoffTime)
            {
                uMinLeaveTime = pRoleModule->m_pRoleDataObject->m_uLogoffTime;
                uReleaseRoleID = pTempObj->GetRoleID();
            }
        }
    }

    if (uReleaseRoleID != 0 && GetCount() > m_nMaxCacheNum)
    {
        //当内存中的人数超过人，就清理一个离线时间最长的玩家
        ReleasePlayer(uReleaseRoleID);
    }

    return TRUE;
}

BOOL CPlayerManager::ReleaseAll()
{
    TNodeTypePtr pNode = MoveFirst();
    if (pNode == NULL)
    {
        return TRUE;
    }

    CPlayerObject* pPlayer = NULL;
    for (; pNode != NULL; pNode = MoveNext(pNode))
    {
        pPlayer = pNode->GetValue();
        ERROR_TO_CONTINUE(pPlayer != NULL);

        pPlayer->Uninit();
    }

    Clear();

    return TRUE;
}

