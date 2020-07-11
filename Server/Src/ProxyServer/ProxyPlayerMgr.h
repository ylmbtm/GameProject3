#ifndef __PROXY_PLAYER_MGR_H__
#define __PROXY_PLAYER_MGR_H__
#include "AVLTree.h"

class  CProxyPlayer
{
public:
	CProxyPlayer();

	~CProxyPlayer();

public:
	UINT32	GetGameSvrID();

	VOID	SetGameSvrInfo(UINT32 dwSvrID, UINT32 dwCopyGuid);

	VOID	SetConnID(UINT32 dwConnID);

	UINT64	GetCharID();

	UINT32  GetConnID();

	UINT32  GetCopyGuid();

public:
	UINT64	m_u64RoleID;

	UINT32	m_dwGameSvrID;

	UINT32  m_dwConnID;

	UINT32  m_dwCopyGuid;
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

	CProxyPlayer*   GetByConnID(UINT32 dwConnID);
};

#endif /*__PROXY_PLAYER_MGR_H__*/