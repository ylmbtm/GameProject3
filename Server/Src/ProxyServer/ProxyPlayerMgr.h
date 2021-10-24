#ifndef __PROXY_PLAYER_MGR_H__
#define __PROXY_PLAYER_MGR_H__
#include "AVLTree.h"

class  CProxyPlayer
{
public:
    CProxyPlayer();

    ~CProxyPlayer();

public:
    INT32   GetGameSvrID();

    VOID    SetGameSvrInfo(INT32 nSvrID, INT32 nCopyGuid);

    VOID    SetConnID(INT32 nConnID);

    UINT64  GetRoleID();

    INT32   GetConnID();

    INT32   GetCopyGuid();

public:
    UINT64  m_uRoleID;

    INT32   m_nGameSvrID;

    INT32   m_nConnID;

    INT32   m_nCopyGuid;
};

class CProxyPlayerMgr : public AVLTree<UINT64, CProxyPlayer>
{
private:
    CProxyPlayerMgr(void);
    ~CProxyPlayerMgr(void);

public:
    static CProxyPlayerMgr* GetInstancePtr();

    CProxyPlayer*   GetByRoleID(UINT64 uRoleID);

    CProxyPlayer*   CreateProxyPlayer(UINT64 uRoleID);

    BOOL            RemoveByRoleID(UINT64 uRoleID);

    CProxyPlayer*   GetByConnID(INT32 nConnID);
};

#endif /*__PROXY_PLAYER_MGR_H__*/