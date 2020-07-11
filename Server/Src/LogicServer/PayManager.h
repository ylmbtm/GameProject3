#ifndef __PAY_MANAGER_H__
#define __PAY_MANAGER_H__

#include "HttpParameter.h"

class CPayManager
{
	CPayManager();
	~CPayManager();
public:
	static CPayManager* GetInstancePtr();

public:
	BOOL Init();

	BOOL Uninit();

	void OnGmPayCallBack(HttpParameter& hParams, UINT32 nConnID);

	std::set<std::string> m_setFinishedOrder;  //己完成支付的订单

	std::set<std::string> m_setCreatedOrder;

	VOID RegisterMessageHanler();

	BOOL Update();
};

#endif //__PAY_MANAGER_H__