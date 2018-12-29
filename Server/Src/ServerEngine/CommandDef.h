#ifndef __MSG_DEFINE_H__
#define __MSG_DEFINE_H__

#define BEGIN_PROCESS_MESSAGE(ClassName) \
BOOL ClassName##::DispatchPacket(NetPacket *pNetPacket) \
{ \
	PacketHeader *pPacketHeader = (PacketHeader *)pNetPacket->m_pDataBuffer->GetBuffer();\
	ERROR_RETURN_TRUE(pPacketHeader != NULL);\
	switch(pPacketHeader->dwMsgID) \
	{

#define PROCESS_MESSAGE_ITEM(dwMsgID, Func) \
		case dwMsgID:{\
		CLog::GetInstancePtr()->LogInfo("---Receive Message:[%s]----", #dwMsgID);\
		if(Func(pNetPacket)){return TRUE;}}break;

#define PROCESS_MESSAGE_ITEMEX(dwMsgID, Func) \
		case dwMsgID:{\
		if(Func(pNetPacket)){return TRUE;}}break;

#define END_PROCESS_MESSAGE \
		default: \
			{ return FALSE;} \
			break;\
	}\
	return TRUE;\
}

#endif /* __MSG_DEFINE_H__ */


