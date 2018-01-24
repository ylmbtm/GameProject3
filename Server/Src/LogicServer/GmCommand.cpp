#include "stdafx.h"
#include "GmCommand.h"
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

CGmCommand::CGmCommand()
{

}

CGmCommand::~CGmCommand()
{

}

BOOL CGmCommand::DispatchPacket(NetPacket* pNetPacket)
{
	switch (pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_PHP_GM_COMMAND_REQ, OnMsgGmCommandReq)
	}

	return FALSE;
}

BOOL CGmCommand::OnMsgGmCommandReq(NetPacket* pNetPacket)
{
	ChatMessageReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pHeader->dwUserData != 0);
	ERROR_RETURN_TRUE(Req.srcid() != 0);

	return TRUE;
}

CGmCommand* CGmCommand::GetInstancePtr()
{
	static CGmCommand _StaticMgr;

	return &_StaticMgr;
}

