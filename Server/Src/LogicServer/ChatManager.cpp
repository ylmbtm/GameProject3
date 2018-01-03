#include "stdafx.h"
#include "ChatManager.h"
#include "GameService.h"
#include "CommonFunc.h"
#include "Log.h"
#include "DataPool.h"
#include "GlobalDataMgr.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Game_Define.pb.h"
#include "CommandDef.h"
#include "PacketHeader.h"
#include "../GameServer/GameObject/SceneObject.h"
#include "PlayerManager.h"

CChatManager::CChatManager()
{

}

CChatManager::~CChatManager()
{

}

BOOL CChatManager::DispatchPacket(NetPacket* pNetPacket)
{
	switch (pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_CHAT_MESSAGE_REQ, OnMsgChatReq)
	}

	return FALSE;
}

BOOL CChatManager::OnMsgChatReq(NetPacket* pNetPacket)
{
	ChatMessageReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	ERROR_RETURN_TRUE(Req.srcid() != 0);

	return TRUE;
}

CChatManager* CChatManager::GetInstancePtr()
{
	static CChatManager _StaticMgr;

	return &_StaticMgr;
}

