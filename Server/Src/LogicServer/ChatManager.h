#ifndef __CHAT_MANAGER_H__
#define __CHAT_MANAGER_H__


class CChatManager
{
	CChatManager();
	~CChatManager();

public:
	BOOL DispatchPacket(NetPacket* pNetPacket);




	static CChatManager* GetInstancePtr();

public:

};
#endif