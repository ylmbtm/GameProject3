#ifndef __WEB_COMMAND_MGR_H__
#define __WEB_COMMAND_MGR_H__

#include "WebActionDef.h"
#include "HttpParameter.h"

class CWebCommandMgr
{
	CWebCommandMgr();
	~CWebCommandMgr();
public:
	static CWebCommandMgr* GetInstancePtr();

	BOOL   DispatchPacket(NetPacket* pNetPacket);

	BOOL   Init();

	BOOL   Uninit();

	BOOL SendWebResult(UINT32 nConnID, EWebResult eResult);
public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgGmCommandReq(NetPacket* pNetPacket);

	//*********************消息处理定义结束******************************

public:
	//*********************WebAction处理定义开始******************************
	void OnGmSealAccount(HttpParameter& hParams, UINT32 nConnID);
	void OnGmServerChange(HttpParameter& hParams, UINT32 nConnID);
	//*********************WebAction处理定义开始******************************
};
#endif