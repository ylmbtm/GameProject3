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

	BOOL   SendWebResult(INT32 nConnID, EWebResult eResult);

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgGmCommandReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************

public:
	//*********************WebAction处理定义开始******************************
	void OnGmReloadTable(HttpParameter& hParams, INT32 nConnID);
	void OnGmSealRole(HttpParameter& hParams, INT32 nConnID);
	void OnGmSingleMail(HttpParameter& hParams, INT32 nConnID); //单发邮件
	void OnGmGroupMail(HttpParameter& hParams, INT32 nConnID);  //群发邮件
	void OnGmDeleteMail(HttpParameter& hParams, INT32 nConnID); //删除邮件
	void OnGmCommand(HttpParameter& hParams, INT32 nConnID);
	//*********************WebAction处理定义开始******************************

};
#endif