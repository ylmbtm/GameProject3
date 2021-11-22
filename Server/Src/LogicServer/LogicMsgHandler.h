#ifndef _LOGIC_MSG_HANDLER_H_
#define _LOGIC_MSG_HANDLER_H_

#include "GameSvrMgr.h"
#include "PlayerObject.h"
#include "PlayerManager.h"

class CLogicMsgHandler
{
public:
	CLogicMsgHandler();

	~CLogicMsgHandler();

	BOOL Init(INT32 nReserved);

	BOOL Uninit();

	BOOL OnUpdate(UINT64 uTick);

	BOOL OnSecondTimer();

	VOID RegisterMessageHanler();
public:
	BOOL ProcessGMCommand(UINT64 u64ID, std::vector<std::string>& vtParam);

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgSelectServerReq(NetPacket* pNetPacket);
	BOOL OnMsgRoleListReq(NetPacket* pNetPacket);
	BOOL OnMsgRoleListAck(NetPacket* pNetPacket);
	BOOL OnMsgRoleCreateReq(NetPacket* pNetPacket);
	BOOL OnMsgRoleDeleteReq(NetPacket* pNetPacket);
	BOOL OnMsgRoleDeleteAck(NetPacket* pNetPacket);
	BOOL OnMsgRoleLoginReq(NetPacket* pNetPacket);
	BOOL OnMsgRoleLoginAck(NetPacket* pNetPacket);
	BOOL OnMsgRoleLogoutReq(NetPacket* pNetPacket);
	BOOL OnMsgRoleDisconnect(NetPacket* pNetPacket);
	BOOL OnMsgMainCopyReq(NetPacket* pNetPacket);
	BOOL OnMsgAbortSceneNtf(NetPacket* pNetPacket);
	BOOL OnMsgBackToCityReq(NetPacket* pNetPacket);
	BOOL OnMsgChatMessageReq(NetPacket* pNetPacket);
	BOOL OnMsgReconnectReq(NetPacket* pNetPacket);
	BOOL OnMsgSealAccountNtf(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif //_LOGIC_MSG_HANDLER_H_
