#ifndef __PAY_MANAGER_H__
#define __PAY_MANAGER_H__


class CPayManager
{
	CPayManager();
	~CPayManager();
public:
	static CPayManager* GetInstancePtr();

public:
	BOOL Init();

	BOOL OnMsgRechargeMoney(INT32 nDiamond, int nProductID);
};

#endif //__PAY_MANAGER_H__