#ifndef __TEAM_COPY_MGR__
#define __TEAM_COPY_MGR__

#include "../Message/Msg_Copy.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "AVLTree.h"

struct CRoomItem
{
	CRoomItem()
	{
		memset(uID, 0, sizeof(uID));
		memset(dwCamp, 0, sizeof(dwCamp));
		dwCopyID = 0;
		dwRoomID = 0;
	}
	~CRoomItem()
	{
		memset(uID, 0, sizeof(uID));
		memset(dwCamp, 0, sizeof(dwCamp));
	}

	UINT64 uID[10] = {0};
	UINT32 dwCamp[10] = {0};
	UINT32 dwCopyID = 0;
	UINT32 dwCopyType = 0;
	UINT64 dwRoomID = 0;
};

class CRoomList : public AVLTree<UINT64, CRoomItem>
{
public:
	CRoomList();

	~CRoomList();

public:
	CRoomItem*      GetRoomItem(UINT64 uRoomID);

	CRoomItem*      CreateEmptyRoom();

public:
	UINT64 m_uMaxID;
};

class CTeamCopyMgr
{
private:
	CTeamCopyMgr(void);

	~CTeamCopyMgr(void);
public:
	static CTeamCopyMgr* GetInstancePtr();

	BOOL        OnTimer(UINT32 nParam);

	CRoomItem*  CreateRoom(UINT32 dwCopyID, UINT64 uRoleID);

	VOID        RegisterMessageHanler();
public:
	//*********************消息处理定义开始******************************
	BOOL    OnCloseConnect(UINT32 dwConnID);
	BOOL    OnMsgCreateRoom(NetPacket* pNetPacket);
	BOOL    OnMsgLeaveRoom(NetPacket* pNetPacket);
	BOOL    OnMsgJoinRoom(NetPacket* pNetPacket);
	BOOL    OnMsgStartBattle(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************

public:

	CRoomList               m_WaitRoomList;
};

#endif //__TEAM_COPY_MGR__
