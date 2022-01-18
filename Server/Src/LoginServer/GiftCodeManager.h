#ifndef _GIFT_CODE_MANAGER_H_
#define _GIFT_CODE_MANAGER_H_

#include "DBInterface/CppMysql.h"
#include "LockFreeQueue.h"

#define GIFT_AWARD_ITEM_NUM 8

struct AwardNode
{
    AwardNode()
    {
        m_nAwardID = 0;   //礼包ID
    }

    UINT64      m_nAwardID = 0;    //礼包ID
    INT32       m_nItemID[GIFT_AWARD_ITEM_NUM] = { 0 };
    INT32       m_nItemNum[GIFT_AWARD_ITEM_NUM] = { 0 };
};

struct CodeReqNode
{
    CodeReqNode()
    {
        m_uAccountID    = 0;   //账号ID
        m_uRoleID       = 0;   //角色ID
        m_nAreaID      = 0;   //区服ID
        m_nChannel     = 0;   //渠道号
        m_nResult      = 0;
        m_nConnID      = 0;
    }

    std::string m_strCode;      //礼包码
    UINT64      m_uAccountID;   //账号ID
    UINT64      m_uRoleID;      //角色ID
    INT32       m_nAreaID;     //区服ID
    INT32       m_nChannel;    //渠道号
    INT32       m_nResult;     //返回码
    INT32       m_nConnID;     //连接ID
    INT32       m_nItemID[GIFT_AWARD_ITEM_NUM] = { 0 };
    INT32       m_nItemNum[GIFT_AWARD_ITEM_NUM] = { 0 };
};

class GiftCodeManager
{
public:
    GiftCodeManager(void);
    ~GiftCodeManager(void);

public:
    static GiftCodeManager* GetInstancePtr();

public:
    BOOL    Init();

    BOOL    Uninit();

    BOOL    ProceeGiftCodeThread();

    BOOL    Update();

    BOOL    DispatchPacket(NetPacket* pNetPacket);

public:
    //*********************消息处理定义开始******************************
    BOOL OnMsgRecvGiftCodeReq(NetPacket* pNetPacket);
    //*********************消息处理定义结束******************************

public:
    BOOL                m_IsRun;

    std::thread*        m_pThread;

    ArrayLockFreeQueue<CodeReqNode*, 1024>      m_ArrPrepareNode;
    ArrayLockFreeQueue<CodeReqNode*, 1024>      m_ArrFinishNode;


};

#endif //_PAY_ORDER_MANAGERH_