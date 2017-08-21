#ifndef __CLIENT_OBJECT_MGR_H__
#define __CLIENT_OBJECT_MGR_H__
#include "AVLTree.h"

class  CProxyPlayer
{
public:
	CProxyPlayer(): m_u64RoleID(0), m_dwGameSvrID(0) {}

	~CProxyPlayer();

public:
	UINT32	GetGameSvrID();

	VOID	SetGameSvrInfo(UINT32 dwSvrID, UINT32 dwCopyGuid);

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

	BOOL			GetPlayerOnline(UINT64 u64RoleID);
};

#endif /*__CLIENT_OBJECT_MGR_H__*/