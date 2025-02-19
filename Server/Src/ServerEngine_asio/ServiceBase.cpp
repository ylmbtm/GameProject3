#include "stdafx.h"
#include "SpinLock.h"
#include "ServiceBase.h"
#include "NetManager.h"
#include "Connection.h"
#include "TimerManager.h"
#include "PacketHeader.h"
#include "Log.h"

#define NEW_CONNECTION 1
#define CLOSE_CONNECTION 2

ServiceBase::ServiceBase(void)
{
    m_pPacketDispatcher = NULL;
    m_nHeartInterval = 0;
    m_nHeartTime     = 0;
    m_pRecvDataQueue = new std::deque<NetPacket>();
    m_pDispathQueue = new std::deque<NetPacket>();
}

ServiceBase::~ServiceBase(void)
{
    delete m_pRecvDataQueue;
    delete m_pDispathQueue;

    m_pRecvDataQueue = NULL;
    m_pDispathQueue = NULL;

    m_nHeartInterval = 0;
    m_nHeartTime     = 0;
}

ServiceBase* ServiceBase::GetInstancePtr()
{
    static ServiceBase _ServiceBase;

    return &_ServiceBase;
}


BOOL ServiceBase::OnDataHandle(IDataBuffer* pDataBuffer, INT32 nConnID)
{
    PacketHeader* pHeader = (PacketHeader*)pDataBuffer->GetBuffer();

    m_QueueLock.Lock();
    m_pRecvDataQueue->emplace_back(NetPacket(nConnID, pDataBuffer, pHeader->nMsgID));
    m_QueueLock.Unlock();
    return TRUE;
}

BOOL ServiceBase::StartNetwork(UINT16 nPortNum, INT32 nMaxConn, IPacketDispatcher* pDispather, std::string strListenIp)
{
    ERROR_RETURN_FALSE(pDispather != NULL);
    ERROR_RETURN_FALSE(nPortNum > 0);
    ERROR_RETURN_FALSE(nMaxConn > 0);

    m_pPacketDispatcher = pDispather;

    if (!CNetManager::GetInstancePtr()->Start(nPortNum, nMaxConn, this, strListenIp))
    {
        CLog::GetInstancePtr()->LogError("启动网络层失败!");
        return FALSE;
    }

    m_uLastTick = 0;
    m_nRecvNum = 0;
    m_nFps = 0;
    m_nSendNum = 0;
    m_nLastMsgID = 0;
    return TRUE;
}

BOOL ServiceBase::StopNetwork()
{
    CNetManager::GetInstancePtr()->Stop();

    return TRUE;
}

template<typename T>
BOOL ServiceBase::SendMsgStruct(INT32 nConnID, INT32 nMsgID, UINT64 u64TargetID, UINT32 dwUserData, T& Data)
{
    if (nConnID <= 0)
    {
        return FALSE;
    }

    m_nSendNum++;

    return CNetManager::GetInstancePtr()->SendMessageData(nConnID, nMsgID, u64TargetID, dwUserData, &Data, sizeof(T));
}

BOOL ServiceBase::SendMsgProtoBuf(INT32 nConnID, INT32 nMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
{
    if (nConnID <= 0)
    {
        CLog::GetInstancePtr()->LogWarn("SendMsgProtoBuf Error nConnID is Zero MessageID:%d", nMsgID);
        return FALSE;
    }

    char szBuff[204800] = {0};

    ERROR_RETURN_FALSE(pdata.ByteSize() < 204800);

    pdata.SerializePartialToArray(szBuff, pdata.GetCachedSize());
    m_nSendNum++;
    return CNetManager::GetInstancePtr()->SendMessageData(nConnID, nMsgID, u64TargetID, dwUserData, szBuff, pdata.GetCachedSize());
}

BOOL ServiceBase::SendMsgRawData(INT32 nConnID, INT32 nMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pdata, INT32 nLen)
{
    if (nConnID <= 0)
    {
        return FALSE;
    }

    m_nSendNum++;

    return CNetManager::GetInstancePtr()->SendMessageData(nConnID, nMsgID, u64TargetID, dwUserData, pdata, nLen);
}

BOOL ServiceBase::SendMsgBuffer(INT32 nConnID, IDataBuffer* pDataBuffer)
{
    if (nConnID == 0)
    {
        return FALSE;
    }

    m_nSendNum++;
    return CNetManager::GetInstancePtr()->SendMessageBuff(nConnID, pDataBuffer);
}

CConnection* ServiceBase::ConnectTo( std::string strIpAddr, UINT16 sPort )
{
    ERROR_RETURN_NULL(!strIpAddr.empty() && sPort > 0);

    return CNetManager::GetInstancePtr()->ConnectTo_Async(strIpAddr, sPort);
}

BOOL ServiceBase::CloseConnect(INT32 nConnID)
{
    CConnection* pConnection = GetConnectionByID(nConnID);

    ERROR_RETURN_FALSE(pConnection != NULL);

    pConnection->Close();

    return TRUE;
}


BOOL ServiceBase::EnableCheck(BOOL bCheck)
{
    return CNetManager::GetInstancePtr()->EnableCheck(bCheck);
}

BOOL ServiceBase::SetHeartInterval(INT32 nInterval)
{
    m_nHeartInterval = nInterval;

    return TRUE;
}

BOOL ServiceBase::OnCloseConnect(INT32 nConnID)
{
    ERROR_RETURN_FALSE(nConnID != 0);
    m_QueueLock.Lock();
    m_pRecvDataQueue->emplace_back(NetPacket(nConnID, NULL, CLOSE_CONNECTION));
    m_QueueLock.Unlock();
    return TRUE;
}

BOOL ServiceBase::OnNewConnect(INT32 nConnID)
{
    ERROR_RETURN_FALSE(nConnID != 0);
    m_QueueLock.Lock();
    m_pRecvDataQueue->emplace_back(NetPacket(nConnID, NULL, NEW_CONNECTION));
    m_QueueLock.Unlock();
    return TRUE;
}

CConnection* ServiceBase::GetConnectionByID( INT32 nConnID )
{
    return CConnectionMgr::GetInstancePtr()->GetConnectionByID(nConnID);
}

BOOL ServiceBase::Update()
{
    if (m_uLastTick == 0)
    {
        m_uLastTick = CommonFunc::GetTickCount();
    }

    m_QueueLock.Lock();
    std::swap(m_pRecvDataQueue, m_pDispathQueue);
    m_QueueLock.Unlock();

    if (m_pDispathQueue->size() > 0)
    {
        for (std::deque<NetPacket>::iterator itor = m_pDispathQueue->begin(); itor != m_pDispathQueue->end(); ++itor)
        {
            NetPacket& item = *itor;
            if (item.m_nMsgID == NEW_CONNECTION)
            {
                m_pPacketDispatcher->OnNewConnect(item.m_nConnID);
            }
            else if (item.m_nMsgID == CLOSE_CONNECTION)
            {
                m_pPacketDispatcher->OnCloseConnect(item.m_nConnID);
                //发送通知
                CConnectionMgr::GetInstancePtr()->DeleteConnection(item.m_nConnID);
            }
            else
            {
                m_nLastMsgID = item.m_nMsgID;
                m_pPacketDispatcher->DispatchPacket(&item);

                item.m_pDataBuffer->Release();

                m_nRecvNum += 1;
            }
        }

        m_pDispathQueue->clear();
    }

    m_nFps += 1;

    if((CommonFunc::GetTickCount() - m_uLastTick) > 1000)
    {
        m_pPacketDispatcher->OnSecondTimer();

        CLog::GetInstancePtr()->SetTitle("[AreaID:%d]-[FPS:%d]-[RecvPack:%d]--[SendPack:%d]", CConfigFile::GetInstancePtr()->GetIntValue("areaid"), m_nFps, m_nRecvNum, m_nSendNum);
        m_nFps = 0;
        m_nRecvNum = 0;
        m_nSendNum = 0;
        m_uLastTick = CommonFunc::GetTickCount();
        m_nHeartTime++;
        if (m_nHeartInterval > 0 && m_nHeartTime >= 30)
        {
            CConnectionMgr::GetInstancePtr()->CheckConntionAvalible(m_nHeartInterval);
            m_nHeartTime = 0;
        }
    }

    TimerManager::GetInstancePtr()->UpdateTimer();

    return TRUE;
}

BOOL ServiceBase::FixFrameNum(INT32 nFrames)
{
    if (nFrames < 1)
    {
        nFrames = 1;
    }

    static UINT64 uNextTick = CommonFunc::GetTickCount();
    UINT64 uCurTick = CommonFunc::GetTickCount();

    if (uNextTick > uCurTick)
    {
        CommonFunc::Sleep(uNextTick - uCurTick);
    }

    uNextTick = uNextTick + 1000 / nFrames;

    return TRUE;
}
