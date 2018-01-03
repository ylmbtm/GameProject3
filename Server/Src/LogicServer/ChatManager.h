#ifndef __CHAT_MANAGER_H__
#define __CHAT_MANAGER_H__


class CChatManager
{
	CChatManager();
	~CChatManager();

public:
	BOOL DispatchPacket(NetPacket* pNetPacket);


	//*********************消息处理定义开始******************************
	BOOL OnMsgChatReq(NetPacket* pNetPacket);


	//*********************消息处理定义结束******************************



public:
	static CChatManager* GetInstancePtr();
};
#endif