#include "stdafx.h"
#include "GiftCodeManager.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Game.pb.h"

GiftCodeManager::GiftCodeManager(void)
{
    m_IsRun = FALSE;
}

GiftCodeManager::~GiftCodeManager(void)
{
    m_IsRun = FALSE;
}

GiftCodeManager* GiftCodeManager::GetInstancePtr()
{
    static GiftCodeManager _GiftCodeManager;

    return &_GiftCodeManager;
}

BOOL GiftCodeManager::Init()
{
    m_IsRun = TRUE;

    m_pThread = new std::thread(&GiftCodeManager::ProceeGiftCodeThread, this);

    ERROR_RETURN_FALSE(m_pThread != NULL);

    return TRUE;
}

BOOL GiftCodeManager::Uninit()
{
    m_IsRun = FALSE;

    if (m_pThread != NULL)
    {
        m_pThread->join();

        delete m_pThread;

        m_pThread = NULL;
    }

    return TRUE;
}


BOOL GiftCodeManager::ProceeGiftCodeThread()
{
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_ip");
    INT32 nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_gm_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_user");
    std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_pwd");
    std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_gm_svr_db_name");

    CppMySQL3DB         tDBConnection;
    if (!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
    {
        CLog::GetInstancePtr()->LogError("GiftCodeManager::Init Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        CLog::GetInstancePtr()->LogError("GiftCodeManager::Init Error: Host:[%s]-User:[%s]-Pwd:[%s]-DBName:[%s]", strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str());
        return FALSE;
    }

    //先把礼包奖励全部加载出来
    std::map<INT32, AwardNode> mapAwardList;

    CodeReqNode* pTmpNode;

    CHAR szSql[SQL_BUFF_LEN] = { 0 };

    while (m_IsRun)
    {
        if (m_ArrPrepareNode.size() <= 0)
        {
            CommonFunc::Sleep(100);
            continue;
        }

        if (!tDBConnection.ping())
        {
            if (!tDBConnection.reconnect())
            {
                CommonFunc::Sleep(1000);
                continue;
            }
        }

        while (m_ArrPrepareNode.pop(pTmpNode))
        {
            if (pTmpNode == NULL)
            {
                continue;
            }

            CHAR szSql[SQL_BUFF_LEN] = { 0 };
            //////////////////////////////////////////////////////////////////////////
#if 0
            snprintf(szSql, SQL_BUFF_LEN, "call use_giftcode('%s',%d, %lld, %d, @retcode, @awardid)", pTmpNode->m_strCode.c_str(), pTmpNode->m_nAreaID, pTmpNode->m_uRoleID, pTmpNode->m_nChannel);
            tDBConnection.querySQL("call use_giftcode('%s',1, 12345, 1, @retcode, @awardid)");
            CppMySQLQuery result = tDBConnection.querySQL("select @retcode, @awardid;");
            INT32 nRetCode = result.getIntField("retcode");
            INT32 nAwardID = result.getIntField("awardid");
#else
            //////////////////////////////////////////////////////////////////////////

            if (CommonConvert::HasSymbol(pTmpNode->m_strCode.c_str(), (const char*)"\'\" \\\r\n%%"))
            {
                CLog::GetInstancePtr()->LogError("GiftCodeManager::Invalid Gift Code :%s", pTmpNode->m_strCode.c_str());
                pTmpNode->m_nResult = MRC_GIFTCODE_INVALIDE_CODE;
                m_ArrFinishNode.push(pTmpNode);
                continue;
            }

            std::string strNowTime = CommonFunc::TimeToString(CommonFunc::GetCurrTime());
            snprintf(szSql, SQL_BUFF_LEN, "select * from giftcode_base where giftcode ='%s' and '%s' > starttime and '%s' < endtime", pTmpNode->m_strCode.c_str(), strNowTime.c_str(), strNowTime.c_str());
            CppMySQLQuery codeResult = tDBConnection.querySQL(szSql);
            if (tDBConnection.GetErrorNo() != 0)
            {
                CLog::GetInstancePtr()->LogError("GiftCodeManager::get gift code failure Error :%s", tDBConnection.GetErrorMsg());
                pTmpNode->m_nResult = MRC_GIFTCODE_INVALIDE_CODE;
                m_ArrFinishNode.push(pTmpNode);
                continue;
            }

            if (codeResult.eof())
            {
                pTmpNode->m_nResult = MRC_GIFTCODE_INVALIDE_CODE;
                m_ArrFinishNode.push(pTmpNode);
                continue;
            }

            INT32 nAssign = codeResult.getIntField("assign");
            INT32 nUseType = codeResult.getIntField("usetype");
            UINT64 uRoleID = codeResult.getInt64Field("roleid");
            INT32 nUseNum = codeResult.getIntField("usenum");
            INT32 nTotalNum = codeResult.getIntField("totalnum");
            INT32 nChannel = codeResult.getIntField("channel");
            INT32 nAreaID = codeResult.getIntField("areaid");
            INT32 nAwardID = codeResult.getIntField("awardid");
            if (nAssign <= 0)
            {
                pTmpNode->m_nResult = MRC_GIFTCODE_UNASSIGNED;
                m_ArrFinishNode.push(pTmpNode);
                continue;
            }

            if (nChannel != pTmpNode->m_nChannel && nChannel > 0)
            {
                pTmpNode->m_nResult = MRC_GIFTCODE_WRONG_CHANNEL;
                m_ArrFinishNode.push(pTmpNode);
                continue;
            }

            if (nAreaID != pTmpNode->m_nAreaID && nAreaID > 0)
            {
                pTmpNode->m_nResult = MRC_GIFTCODE_WRONG_AREA;
                m_ArrFinishNode.push(pTmpNode);
                continue;
            }

            if (nUseType == 1)  //表示只能一人使用
            {
                if (uRoleID > 0)
                {
                    pTmpNode->m_nResult = MRC_GIFTCODE_AREADY_USED;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                snprintf(szSql, SQL_BUFF_LEN, "update giftcode_base set roleid=%lld, usenum = 1 where giftcode ='%s'", pTmpNode->m_uRoleID, pTmpNode->m_strCode.c_str());
                if (tDBConnection.execSQL(szSql) <= 0 || tDBConnection.GetErrorNo() != 0)
                {
                    pTmpNode->m_nResult = MRC_UNKNOW_ERROR;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }
            }
            else if (nUseType == 2)  //表示可以多人使用
            {
                if (nUseNum >= nTotalNum)
                {
                    pTmpNode->m_nResult = MRC_GIFTCODE_NO_MORE;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                snprintf(szSql, SQL_BUFF_LEN, "select * from giftcode_used where giftcode ='%s' and roleid = %lld", pTmpNode->m_strCode.c_str(), pTmpNode->m_uRoleID);
                CppMySQLQuery usedResult = tDBConnection.querySQL(szSql);
                if (tDBConnection.GetErrorNo() != 0)
                {
                    pTmpNode->m_nResult = MRC_UNKNOW_ERROR;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                if (usedResult.numRow() > 0)
                {
                    pTmpNode->m_nResult = MRC_GIFTCODE_AREADY_USED;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                snprintf(szSql, SQL_BUFF_LEN, "insert into giftcode_used(roleid, accountid,areaid, giftcode, usetime, awardid) values(%lld, %lld, %d, '%s', %lld, %d)", pTmpNode->m_uRoleID, pTmpNode->m_uAccountID, pTmpNode->m_nAreaID,
                         pTmpNode->m_strCode.c_str(), CommonFunc::GetCurrTime(), nAwardID);
                if (tDBConnection.execSQL(szSql) <= 0 || tDBConnection.GetErrorNo() != 0)
                {
                    pTmpNode->m_nResult = MRC_UNKNOW_ERROR;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                snprintf(szSql, SQL_BUFF_LEN, "update giftcode_base set usenum = usenum +1 where giftcode ='%s'", pTmpNode->m_strCode.c_str());
                if (tDBConnection.execSQL(szSql) <= 0 || tDBConnection.GetErrorNo() != 0)
                {
                    pTmpNode->m_nResult = MRC_UNKNOW_ERROR;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }
            }
            else if (nUseType == 3)  //表示可以多人使用
            {
                if (uRoleID > 0)
                {
                    pTmpNode->m_nResult = MRC_GIFTCODE_AREADY_USED;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                snprintf(szSql, SQL_BUFF_LEN, "select * from giftcode_used where awardid = %d and roleid = %lld", nAwardID, pTmpNode->m_uRoleID);
                CppMySQLQuery usedResult = tDBConnection.querySQL(szSql);
                if (tDBConnection.GetErrorNo() != 0)
                {
                    pTmpNode->m_nResult = MRC_UNKNOW_ERROR;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                if (usedResult.numRow() > 0)
                {
                    pTmpNode->m_nResult = MRC_GIFTCODE_AREADY_USED;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                snprintf(szSql, SQL_BUFF_LEN, "insert into giftcode_used(roleid, accountid,areaid, giftcode, usetime, awardid) values(%lld, %lld, %d, '%s', %lld, %d)", pTmpNode->m_uRoleID, pTmpNode->m_uAccountID, pTmpNode->m_nAreaID,
                         pTmpNode->m_strCode.c_str(), CommonFunc::GetCurrTime(), nAwardID);
                if (tDBConnection.execSQL(szSql) <= 0 || tDBConnection.GetErrorNo() != 0)
                {
                    pTmpNode->m_nResult = MRC_UNKNOW_ERROR;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }

                snprintf(szSql, SQL_BUFF_LEN, "update giftcode_base set roleid=%lld, usenum = 1 where giftcode ='%s'", pTmpNode->m_uRoleID, pTmpNode->m_strCode.c_str());
                if (tDBConnection.execSQL(szSql) <= 0 || tDBConnection.GetErrorNo() != 0)
                {
                    pTmpNode->m_nResult = MRC_UNKNOW_ERROR;
                    m_ArrFinishNode.push(pTmpNode);
                    continue;
                }
            }
#endif

            std::map<INT32, AwardNode>::iterator itor = mapAwardList.find(nAwardID);
            if (itor == mapAwardList.end())
            {
                AwardNode tAwardNode;
                INT32 nIndex = 0;
                snprintf(szSql, SQL_BUFF_LEN, "select * from giftcode_award_item where awardid=%d", nAwardID);
                CppMySQLQuery awardResult = tDBConnection.querySQL(szSql);
                while (!awardResult.eof())
                {
                    tAwardNode.m_nItemID[nIndex] = awardResult.getIntField("objectid");
                    tAwardNode.m_nItemNum[nIndex] = awardResult.getIntField("objectnum");
                    nIndex++;
                    if (nIndex >= GIFT_AWARD_ITEM_NUM)
                    {
                        break;
                    }
                    awardResult.nextRow();
                }

                mapAwardList.insert(std::make_pair(nAwardID, tAwardNode));
                for (int i = 0; i < GIFT_AWARD_ITEM_NUM; i++)
                {
                    pTmpNode->m_nItemID[i] = tAwardNode.m_nItemID[i];
                    pTmpNode->m_nItemNum[i] = tAwardNode.m_nItemNum[i];
                }
            }
            else
            {
                AwardNode& tAwardNode = itor->second;
                for (int i = 0; i < GIFT_AWARD_ITEM_NUM; i++)
                {
                    pTmpNode->m_nItemID[i] = tAwardNode.m_nItemID[i];
                    pTmpNode->m_nItemNum[i] = tAwardNode.m_nItemNum[i];
                }
            }

            pTmpNode->m_nResult = MRC_SUCCESSED;

            m_ArrFinishNode.push(pTmpNode);
        }
    }
    return TRUE;
}

BOOL GiftCodeManager::Update()
{
    if (m_ArrFinishNode.size() <= 0)
    {
        return TRUE;
    }

    CodeReqNode* pTmpNode;
    while (m_ArrFinishNode.pop(pTmpNode))
    {
        if (pTmpNode == NULL)
        {
            continue;
        }

        Msg_GiftCodeAck Ack;
        Ack.set_retcode(pTmpNode->m_nResult);
        Ack.set_roleid(pTmpNode->m_uRoleID);
        if (pTmpNode->m_nResult == MRC_SUCCESSED)
        {
            for (int i = 0; i < 5; i++)
            {
                if (pTmpNode->m_nItemID[i] != 0)
                {
                    ItemData* pData = Ack.add_items();
                    pData->set_itemid(pTmpNode->m_nItemID[i]);
                    pData->set_itemnum(pTmpNode->m_nItemNum[i]);
                }
            }
        }

        ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pTmpNode->m_nConnID, MSG_RECV_GIFTCODE_ACK, pTmpNode->m_uRoleID, 0, Ack);

        delete pTmpNode;
    }

    return TRUE;
}

BOOL GiftCodeManager::OnMsgRecvGiftCodeReq(NetPacket* pNetPacket)
{
    Msg_GiftCodeReq Req;

    Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());

    CodeReqNode* pNode = new CodeReqNode();
    pNode->m_nAreaID = Req.areaid();
    pNode->m_nChannel = Req.channel();
    pNode->m_nConnID = pNetPacket->m_nConnID;
    pNode->m_uAccountID = Req.accountid();
    pNode->m_uRoleID = Req.roleid();
    pNode->m_strCode = Req.giftcode();
    m_ArrPrepareNode.push(pNode);

    return TRUE;
}

BOOL GiftCodeManager::DispatchPacket(NetPacket* pNetPacket)
{
    switch (pNetPacket->m_nMsgID)
    {
            PROCESS_MESSAGE_ITEM(MSG_RECV_GIFTCODE_REQ, OnMsgRecvGiftCodeReq);
    }

    return FALSE;
}

