#include "stdafx.h"
#include "TeamCopyMgr.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Game_Define.pb.h"
#include "../ServerData/ServerDefine.h"
#include "../StaticData/StaticStruct.h"
#include "../StaticData/StaticData.h"
#include "../ServerData/RoleData.h"

#include "CopyModule.h"
#include "BagModule.h"
#include "RoleModule.h"
#include "TimerManager.h"
#include "GameSvrMgr.h"
#include "../GameServer/SceneLogic/WinCondition.h"

CTeamCopyMgr::CTeamCopyMgr(void)
{
	RegisterMessageHanler();

	TimerManager::GetInstancePtr()->AddDiffTimer(60, 1, &CTeamCopyMgr::OnTimer, this);
}

CTeamCopyMgr::~CTeamCopyMgr(void)
{
}

CTeamCopyMgr* CTeamCopyMgr::GetInstancePtr()
{
	static CTeamCopyMgr _TeamCopyManager;

	return &_TeamCopyManager;
}


BOOL CTeamCopyMgr::OnTimer(UINT32 nParam)
{


	return TRUE;
}

CRoomItem* CTeamCopyMgr::CreateRoom(UINT32 dwCopyID, UINT64 uRoleID)
{
	CRoomItem* pRoomItem = m_WaitRoomList.CreateEmptyRoom();
	ERROR_RETURN_NULL(pRoomItem != NULL);

	pRoomItem->dwCopyID = dwCopyID;

	pRoomItem->uID[0] = uRoleID;

	pRoomItem->dwCamp[0] = EBC_PLAYER;

	return pRoomItem;
}

VOID CTeamCopyMgr::RegisterMessageHanler()
{

}

BOOL CTeamCopyMgr::OnCloseConnect(UINT32 dwConnID)
{


	return TRUE;
}


BOOL CTeamCopyMgr::OnMsgCreateRoom(NetPacket* pNetPacket)
{
	//验证玩家是否合法
	//玩家是否在其它的房间内没有退出。
	//清理其它房间内的玩家
	//创建新的房间
	//返回房间的id给客户端
	//返回成功的消息
	return TRUE;
}

BOOL CTeamCopyMgr::OnMsgLeaveRoom(NetPacket* pNetPacket)
{
	//验证玩家是否合法
	//验证玩家是否在这个房间内
	//如果房间只有一个人，退出，房间销毁
	//如果房间内还有其它的人，转移房主的权力
	//房间状态通知其它的玩家
	//返回成功
	return TRUE;
}

BOOL CTeamCopyMgr::OnMsgJoinRoom(NetPacket* pNetPacket)
{
	//1.指定加入
	//验证玩家是否合法
	//验证房间是否存在
	//房间内是否还有位置
	//加入房间
	//房间状态通知其它的玩家
	//返回结果

	//2.搜索加入
	//验证玩家是否合法
	//是否能找到需要人的房间
	//加入房间
	//房间状态通知其它的玩家
	//返回结果
	return TRUE;
}

BOOL CTeamCopyMgr::OnMsgStartBattle(NetPacket* pNetPacket)
{
	//UINT64 uRoleID = 0;
	UINT64 uRoomID = 0;

	CRoomItem* pItem = m_WaitRoomList.GetRoomItem(uRoomID);
	ERROR_RETURN_TRUE(pItem != NULL);

	CGameSvrMgr::GetInstancePtr()->TakeCopyRequest(pItem->uID, pItem->dwCamp, 3, pItem->dwCopyID, pItem->dwCopyType);

	return TRUE;
}

CRoomList::CRoomList()
{
	m_uMaxID = 0;
}

CRoomList::~CRoomList()
{

}

CRoomItem* CRoomList::GetRoomItem(UINT64 uRoomID)
{
	return GetByKey(uRoomID);
}

CRoomItem* CRoomList::CreateEmptyRoom()
{
	CRoomItem* pItem = InsertAlloc(++m_uMaxID);
	ERROR_RETURN_NULL(pItem != NULL);
	pItem->dwRoomID = m_uMaxID - 1;
	return pItem;
}
