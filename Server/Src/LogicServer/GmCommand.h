#ifndef __GM_COMMAND_H__
#define __GM_COMMAND_H__


class CGmCommand
{
	CGmCommand();
	~CGmCommand();
public:
	static CGmCommand* GetInstancePtr();
public:
	BOOL DispatchPacket(NetPacket* pNetPacket);
	BOOL OnMsgGmCommandReq(NetPacket* pNetPacket);
public:





};
#endif