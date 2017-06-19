#ifndef __CLIENT_OBJECT_MGR_H__
#define __CLIENT_OBJECT_MGR_H__
#include "Utility/AVLTree.h"

enum LineState
{
	LS_NONE,
	LS_Online,
	LS_OffLine,
	LS_EnterGame,
	LS_LeaveGame,
	LS_Unknow
}; 


class  CProxyPlayer
{
public:
	CProxyPlayer():m_u64RoleID(0),m_GameSvrConnID(0){}

	~CProxyPlayer();

public:
	UINT64	GetGameSvrConnID();

	VOID	SetGameSvrConnID(UINT64 u64ConnID);

	UINT64	GetCharID();

	UINT32  GetClientConnID();

public:
	UINT64	m_u64RoleID;

	UINT64	m_GameSvrConnID;

	UINT32  m_dwClientConnID;

	UINT32  m_CharState;
};

class CProxyPlayerMgr : public AVLTree<UINT64, CProxyPlayer>
{
private:
	CProxyPlayerMgr(void);
	~CProxyPlayerMgr(void);

public:
	static CProxyPlayerMgr* GetInstancePtr();

	CProxyPlayer*	GetByCharID(UINT64 u64RoleID);

	CProxyPlayer*	CreateProxyPlayer(UINT64 u64RoleID);

	BOOL			RemoveByCharID(UINT64 u64RoleID);

	BOOL			GetPlayerOnline(UINT64 u64RoleID);
};

#endif /*__CLIENT_OBJECT_MGR_H__*/