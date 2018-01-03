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

	BOOL DispatchPacket(NetPacket* pNetPacket);

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	BOOL OnUpdate(UINT64 uTick);

public:
	BOOL ProcessGameCommand(UINT64 u64ID, std::vector<std::string>& vtParam);




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
	BOOL OnMsgAbortCopyReq(NetPacket* pNetPacket);
	BOOL OnMsgBackToCityReq(NetPacket* pNetPacket);
	BOOL OnMsgRegToLoginAck(NetPacket* pNetPacket);
	BOOL OnMsgChatMessageReq(NetPacket* pNetPacket);
	BOOL OnMsgReconnectReq(NetPacket* pNetPacket);
	BOOL OnMsgTestAddItemReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif //_LOGIC_MSG_HANDLER_H_
