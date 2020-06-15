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

	void OnGmPayCallBack(HttpParameter& hParams, UINT32 nConnID);

	std::set<std::string> m_setFinishedPay;  //己完成支付的订单
};

#endif //__PAY_MANAGER_H__