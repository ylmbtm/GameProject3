#ifndef __MSG_DEFINE_H__
#define __MSG_DEFINE_H__

#define BEGIN_PROCESS_MESSAGE(ClassName) \
BOOL ClassName##::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper) \
{ \
	CommandHeader *pCmdHeader = pBufferHelper->GetPacketHeader(); \
	if(pCmdHeader == NULL) \
	{ \
		ASSERT_FAIELD; \
		return FALSE; \
	} \
	ASSERT(u64ConnID != 0);\
	switch(wCommandID) \
	{ 

#define PROCESS_MESSAGE_ITEM_T(wCommandID, Func) case wCommandID:{Func(wCommandID, u64ConnID, pBufferHelper);}break;

#define PROCESS_MESSAGE_ITEM(wCommandID, Func) \
		case wCommandID:{\
		CLog::GetInstancePtr()->AddLog("---Receive Message:[%s]----", #wCommandID);\
		Func(pNetPacket);}break;

#define END_PROCESS_MESSAGE \
		default: \
			{ } \
			break;\
	}\
	return TRUE;\
}


#define PROCESS_MESSAGE_ITEM_CLIENT(wCommandID, Func) \
		case wCommandID:{\
		Func(dwMsgID, PacketBuf, BufLen);}break;

#endif /* __MSG_DEFINE_H__ */


