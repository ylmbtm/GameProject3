#ifndef __PAY_MANAGER_H__
#define __PAY_MANAGER_H__

#include "HttpParameter.h"
#include "../Message/Msg_Payment.pb.h"
#include "DBInterface/CppMysql.h"
#include "PayData.h"

class CPayManager
{
	CPayManager();
	~CPayManager();
public:
	static CPayManager* GetInstancePtr();

public:
	BOOL LoadData(CppMySQL3DB& tDBConnection);

	void OnGmPayCallBack(HttpParameter& hParams, INT32 nConnID);

	VOID RegisterMessageHanler();

	BOOL Update();

	VOID WritePayRecordThread(CreatePaymentReq* pReq);

	PayDataObject* GetPayOrderByID(std::string strOrderID);

	BOOL ProcessPlayerLogin(UINT64 uRoleID);

	//处理订单并发货
	BOOL ProcessSussessPayOrder(PayDataObject* pOrderData);

public:
	//*********************消息处理定义开始******************************
	BOOL        OnMsgCreatePayRecord(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************

public:
	std::map<std::string, PayDataObject*> m_mapPayData;

	std::set<std::string> m_setCreatedOrder;
};

#endif //__PAY_MANAGER_H__