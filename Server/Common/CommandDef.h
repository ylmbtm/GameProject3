#ifndef __MSG_DEFINE_H__
#define __MSG_DEFINE_H__

#define BEGIN_PROCESS_MESSAGE(ClassName) \
BOOL ClassName##::DispatchPacket(NetPacket *pNetPacket) \
{ \
	PacketHeader *pPacketHeader = (PacketHeader *)pNetPacket->m_pDataBuffer->GetBuffer();\
	ERROR_RETURN_TRUE(pPacketHeader != NULL);\
	switch(pPacketHeader->dwMsgID) \
	{

#define PROCESS_MESSAGE_ITEM_ID(dwMsgID, Func) \
	case dwMsgID:{\
	CLog::GetInstancePtr()->AddLog("---Receive Message:[%s]----Targetid:[%lld]", #dwMsgID, ((PacketHeader *)pNetPacket->m_pDataBuffer->GetBuffer())->u64TargetID);\
	Func(pNetPacket);}break;

#define PROCESS_MESSAGE_ITEM(dwMsgID, Func) \
		case dwMsgID:{\
		CLog::GetInstancePtr()->AddLog("---Receive Message:[%s]----", #dwMsgID);\
		Func(pNetPacket);}break;

#define PROCESS_MESSAGE_ITEMEX(dwMsgID, Func) \
		case dwMsgID:{\
		Func(pNetPacket);}break;

#define END_PROCESS_MESSAGE \
		default: \
			{ return FALSE;} \
			break;\
	}\
	return TRUE;\
}


#define PROCESS_MESSAGE_ITEM_CLIENT(dwMsgID, Func) \
		case dwMsgID:{\
		printf("---Receive Message:[%s]---- \n", #dwMsgID); \
		Func(dwMsgID, PacketBuf, BufLen);}break;

#endif /* __MSG_DEFINE_H__ */


