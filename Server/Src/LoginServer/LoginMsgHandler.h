#ifndef _LOGIN_MSG_HANDLER_H_
#define _LOGIN_MSG_HANDLER_H_

#include "LogicSvrManager.h"

class CLoginMsgHandler
{
public:
    CLoginMsgHandler();

    ~CLoginMsgHandler();

    BOOL Init();

    BOOL Uninit();

    BOOL DispatchPacket(NetPacket* pNetPacket);

    BOOL OnCloseConnect(INT32 nConnID);

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgCheckVersionReq(NetPacket* pPacket);
	BOOL OnMsgAccountRegReq(NetPacket* pPacket);
	BOOL OnMsgAccountLoginReq(NetPacket* pPacket);
	BOOL OnMsgServerListReq(NetPacket* pPacket);
	BOOL OnMsgSelectServerReq(NetPacket* pPacket);

	BOOL OnMsgAccountRegAck(NetPacket* pPacket);
	BOOL OnMsgAccountLoginAck(NetPacket* pPacket);

	BOOL OnMsgLogicSvrRegReq(NetPacket* pPacket);
	BOOL OnMsgLogicUpdateReq(NetPacket* pPacket);
	BOOL OnMsgSelectServerAck(NetPacket* pPacket);
	BOOL OnMsgSealAccountAck(NetPacket* pPacket);
    BOOL OnMsgGameParamReq(NetPacket* pPacket);
	//*********************消息处理定义结束******************************

public:
	LogicSvrManager m_LogicSvrMgr;
};

#endif //_LOGIN_MSG_HANDLER_H_
