#ifndef _GIFT_CODE_MANAGER_H_
#define _GIFT_CODE_MANAGER_H_

#include "DBInterface/CppMysql.h"
#include "LockFreeQueue.h"

#define GIFT_AWARD_ITEM_NUM 8

struct AwardNode
{
	AwardNode()
	{
		m_dwAwardID = 0;   //礼包ID
	}

	UINT64      m_dwAwardID = 0;    //礼包ID
	UINT32      m_dwItemID[GIFT_AWARD_ITEM_NUM] = { 0 };
	UINT32      m_dwItemNum[GIFT_AWARD_ITEM_NUM] = { 0 };
};

struct CodeReqNode
{
	CodeReqNode()
	{
		m_uAccountID    = 0;   //账号ID
		m_uRoleID       = 0;   //角色ID
		m_dwAreaID      = 0;   //区服ID
		m_dwChannel     = 0;   //渠道号
		m_dwResult      = 0;
		m_dwConnID      = 0;
	}

	std::string m_strCode;      //礼包码
	UINT64      m_uAccountID;   //账号ID
	UINT64      m_uRoleID;      //角色ID
	UINT32      m_dwAreaID;     //区服ID
	UINT32      m_dwChannel;    //渠道号
	UINT32      m_dwResult;     //返回码
	UINT32      m_dwConnID;     //连接ID
	UINT32      m_dwItemID[GIFT_AWARD_ITEM_NUM] = { 0 };
	UINT32      m_dwItemNum[GIFT_AWARD_ITEM_NUM] = { 0 };
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
	BOOL				m_IsRun;

	std::thread*  		m_pThread;

	ArrayLockFreeQueue<CodeReqNode*, 1024>		m_ArrPrepareNode;
	ArrayLockFreeQueue<CodeReqNode*, 1024>		m_ArrFinishNode;


};

#endif //_PAY_ORDER_MANAGERH_