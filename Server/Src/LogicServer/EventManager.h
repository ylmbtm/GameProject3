#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include "MsgHandlerManager.h"

struct CEventParam
{
	UINT32 dwEventID;
	UINT32 dwParam[2];
	UINT64 uParam[2];
};

class CEventHandlerManager  : public CHandlerManager
{
private:
	CEventHandlerManager(void) {};
	virtual ~CEventHandlerManager(void) {};

public:
	static CEventHandlerManager* GetInstancePtr()
	{
		static CEventHandlerManager _Instance;

		return &_Instance;
	}

	template<typename TClass>
	bool RegisterEventHandle(int nEventID, BOOL(TClass::*FuncPtr)(CEventParam*), TClass* pObj)
	{
		return RegisterMessageHandle(nEventID, FuncPtr, pObj);
	}

	template<typename T>
	bool UnregisterEventHandle(int nEventID, T* pObj)
	{
		return UnregisterMessageHandle(nEventID, pObj);
	}

	bool FireEvent(UINT32 dwEventID, UINT32 dwParam1 = 0, UINT32 dwParam2 = 0, UINT64 uParm1 = 0, UINT64 uParam2 = 0)
	{
		CEventParam tParam;
		tParam.dwEventID = dwEventID;
		tParam.dwParam[0] = dwParam1;
		tParam.dwParam[1] = dwParam2;
		tParam.uParam[0] = uParm1;
		tParam.uParam[1] = uParam2;

		return  FireMessage(dwEventID, &tParam);
	}
};
#endif