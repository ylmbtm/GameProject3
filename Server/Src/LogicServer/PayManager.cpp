#include "stdafx.h"
#include "PayManager.h"
#include "GameService.h"
#include "StaticData.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "PacketHeader.h"
#include "IBufferHandler.h"
#include "PayData.h"
#include "DataPool.h"

CPayManager::CPayManager()
{
}

CPayManager::~CPayManager()
{
    for (auto itor = m_mapPayData.begin(); itor != m_mapPayData.end(); ++itor)
    {
        PayDataObject* pObject = itor->second;
        pObject->Release();
    }

    m_mapPayData.clear();
}

CPayManager* CPayManager::GetInstancePtr()
{
    static CPayManager _StaticMgr;

    return &_StaticMgr;
}

BOOL CPayManager::LoadData(CppMySQL3DB& tDBConnection)
{
    CppMySQLQuery QueryResult = tDBConnection.querySQL("SELECT * FROM payment where status = 1");
    while (!QueryResult.eof())
    {
        PayDataObject* pPayDataObject = DataPool::CreateObject<PayDataObject>(ESD_PAYMENT, FALSE);
        pPayDataObject->m_uRoleID = QueryResult.getInt64Field("roleid");
        pPayDataObject->m_nChannel = QueryResult.getIntField("channel");
        pPayDataObject->m_nStatus = QueryResult.getIntField("status");
        pPayDataObject->m_fMoney = QueryResult.getFloatField("money");
        pPayDataObject->m_uFinishTime = QueryResult.getInt64Field("finish_time");
        pPayDataObject->m_uSendTime = QueryResult.getInt64Field("send_time");
        pPayDataObject->m_nProductID = QueryResult.getIntField("productid");
        pPayDataObject->m_nBuyID = QueryResult.getIntField("buyid");
        CommonConvert::StrCopy(pPayDataObject->m_szOrderID, QueryResult.getStringField("orderid"), PAY_ORDERID_LEN);
        CommonConvert::StrCopy(pPayDataObject->m_szThirdID, QueryResult.getStringField("thirdid"), PAY_ORDERID_LEN);
        m_mapPayData.insert(std::make_pair(pPayDataObject->m_szOrderID, pPayDataObject));
        QueryResult.nextRow();
    }

    RegisterMessageHanler();

    return TRUE;
}

VOID CPayManager::RegisterMessageHanler()
{
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_CREATE_PAYMENT_REQ, &CPayManager::OnMsgCreatePayRecord, this);
}

BOOL CPayManager::Update()
{
    return TRUE;
}

VOID CPayManager::WritePayRecordThread(CreatePaymentReq* pReq)
{
    ERROR_RETURN_NONE(pReq != NULL);
    std::unique_ptr<CreatePaymentReq> pCreatePayment(pReq); //用来自动释放内存
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_ip");
    INT32 nPort        = CConfigFile::GetInstancePtr()->GetIntValue("mysql_gm_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_user");
    std::string strPwd  = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_pwd");
    std::string strDb   = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_db_name");

    CppMySQL3DB tDBConnection;
    if (!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
    {
        CLog::GetInstancePtr()->LogError("CPayManager::WritePayRecordThread Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        return ;
    }

    CHAR szSql[SQL_BUFF_LEN] = { 0 };
    snprintf(szSql, SQL_BUFF_LEN, "insert into payment(orderid, serverid, roleid, accountid, buyid, status, channel, platform, productid, create_time) values('%s','%d','%lld','%lld','%d','%d','%d','%d', '%d','%s')",
             pReq->orderid().c_str(), pReq->serverid(), pReq->roleid(), pReq->accountid(), pReq->buyid(), 0, pReq->channel(), pReq->platform(), pReq->productid(), CommonFunc::TimeToString(CommonFunc::GetCurrTime()).c_str());

    if (tDBConnection.execSQL(szSql) < 0)
    {
        CLog::GetInstancePtr()->LogError("CPayManager::WritePayRecordThread Failed! Reason: %s", tDBConnection.GetErrorMsg());
        return;
    }

    CreatePaymentAck Ack;

    Ack.set_orderid(pReq->orderid());

    ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pReq->proxyid(), MSG_CREATE_PAYMENT_ACK, pReq->roleid(), pReq->clientid(), Ack);

    return;
}

PayDataObject* CPayManager::GetPayOrderByID(std::string strOrderID)
{
    auto itor = m_mapPayData.find(strOrderID);
    if (itor != m_mapPayData.end())
    {
        return itor->second;
    }

    return NULL;
}

BOOL CPayManager::ProcessPlayerLogin(UINT64 uRoleID)
{
    for (auto itor = m_mapPayData.begin(); itor != m_mapPayData.end(); ++itor)
    {
        PayDataObject* pPayData = itor->second;
        ERROR_CONTINUE_EX(pPayData != NULL);

        if (pPayData->m_nStatus == EOS_DONE)
        {
            continue;
        }

        if (pPayData->m_uRoleID != uRoleID)
        {
            continue;
        }

        ProcessSussessPayOrder(pPayData);
    }

    return TRUE;
}

BOOL CPayManager::OnMsgCreatePayRecord(NetPacket* pNetPacket)
{
    CreatePaymentReq* pReq = new CreatePaymentReq;
    pReq->ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pReq->roleid());
    ERROR_RETURN_TRUE(pPlayer != NULL);

    ERROR_RETURN_TRUE(pReq->roleid() > 0);
    ERROR_RETURN_TRUE(pReq->serverid() > 0);

    if (pReq->productid() <= 0)
    {
        CreatePaymentAck Ack;
        Ack.set_retcode(MRC_DUPLICATED_ORDER_ID);
        Ack.set_orderid(pReq->orderid());
        pPlayer->SendMsgProtoBuf(MSG_CREATE_PAYMENT_ACK, Ack);
        CLog::GetInstancePtr()->LogError("CPayManager::OnMsgCreatePayRecord Error! Invalid Product ID");
        return TRUE;
    }
    if (m_setCreatedOrder.find(pReq->orderid()) != m_setCreatedOrder.end())
    {
        CreatePaymentAck Ack;
        Ack.set_retcode(MRC_DUPLICATED_ORDER_ID);
        Ack.set_orderid(pReq->orderid());
        pPlayer->SendMsgProtoBuf(MSG_CREATE_PAYMENT_ACK, Ack);
        CLog::GetInstancePtr()->LogError("CPayManager::OnMsgCreatePayRecord Error! Duplicated strOrderID:%s", pReq->orderid().c_str());
        return TRUE;
    }

    pReq->set_proxyid(pNetPacket->m_nConnID);
    pReq->set_clientid(pPlayer->m_nClientConnID);
    std::thread tThread(&CPayManager::WritePayRecordThread, this, pReq);
    tThread.detach();

    return TRUE;
}

void CPayManager::OnGmPayCallBack(HttpParameter& hParams, INT32 nConnID)
{
    UINT64 uRoleID = hParams.GetLongValue("roleid");        //角色ID
    INT32 nProductID = hParams.GetIntValue("productid");  //充值档位
    INT32 nChannel = hParams.GetIntValue("channel");
    std::string strOrderID = hParams.GetStrValue("orderid");
    std::string strThirdID = hParams.GetStrValue("thirdid");
    UINT64 uFinishTime = hParams.GetLongValue("finishtime");
    FLOAT fMoney = hParams.GetFloatValue("money");
    INT32 buyid = hParams.GetIntValue("buyid");

    //通知支付服己经收到支付通知，己成功加入等待发货队列，支付服的任务己经完成
    std::string strResult = CommonConvert::IntToString((INT64)0);
    ServiceBase::GetInstancePtr()->SendMsgRawData(nConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, strResult.c_str(), (INT32)strResult.size());

    //避免重复通知
    if (GetPayOrderByID(strOrderID) != NULL)
    {
        CLog::GetInstancePtr()->LogError("CPayManager::OnGmPayCallBack Error! Duplicated strOrderID:%s", strOrderID.c_str());
        return;
    }

    PayDataObject* pPayDataObject = DataPool::CreateObject<PayDataObject>(ESD_PAYMENT, TRUE);
    pPayDataObject->Lock();
    pPayDataObject->m_uRoleID = uRoleID;
    pPayDataObject->m_nChannel = nChannel;
    pPayDataObject->m_nStatus = EOS_WAIT; //待发货
    pPayDataObject->m_fMoney = fMoney;
    pPayDataObject->m_uFinishTime = uFinishTime;
    pPayDataObject->m_nProductID = nProductID;
    pPayDataObject->m_nBuyID = buyid;
    CommonConvert::StrCopy(pPayDataObject->m_szOrderID, strOrderID.c_str(), PAY_ORDERID_LEN);
    CommonConvert::StrCopy(pPayDataObject->m_szThirdID, strThirdID.c_str(), PAY_ORDERID_LEN);
    pPayDataObject->Unlock();
    m_mapPayData.insert(std::make_pair(strOrderID, pPayDataObject));

    ProcessSussessPayOrder(pPayDataObject);

    return;
}

BOOL CPayManager::ProcessSussessPayOrder(PayDataObject* pOrderData)
{
    ERROR_RETURN_FALSE(pOrderData != NULL);
    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pOrderData->m_uRoleID);
    if (pPlayer == NULL)
    {
        CLog::GetInstancePtr()->LogError("CPayManager::OnGmPayCallBack Error! Invalid uRoleID:%lld", pOrderData->m_uRoleID);
        return FALSE;
    }
    pOrderData->Lock();
    pOrderData->m_uSendTime = CommonFunc::GetCurrTime();
    pOrderData->m_nStatus = EOS_DONE; //已发货
    pOrderData->Unlock();

    return TRUE;
}
