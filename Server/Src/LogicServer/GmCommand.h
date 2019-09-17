#ifndef __GM_COMMAND_H__
#define __GM_COMMAND_H__


class CGmCommand
{
	CGmCommand();
	~CGmCommand();
public:
	static CGmCommand* GetInstancePtr();

	VOID        RegisterMessageHanler();

public:
	BOOL OnMsgGmCommandReq(NetPacket* pNetPacket);

};
#endif