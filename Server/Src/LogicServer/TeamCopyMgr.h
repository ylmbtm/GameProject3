#ifndef __TEAM_COPY_MGR__
#define __TEAM_COPY_MGR__

#include "../Message/Msg_Copy.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "AVLTree.h"

struct CRoomItem
{
    CRoomItem()
    {
        memset(m_uRoleID, 0, sizeof(m_uRoleID));
        memset(m_nCamp, 0, sizeof(m_nCamp));
        m_dwCopyID = 0;
        m_uRoomID = 0;
        m_nCount = 0;
    }

    ~CRoomItem()
    {
        memset(m_uRoleID, 0, sizeof(m_uRoleID));
        memset(m_nCamp, 0, sizeof(m_nCamp));
        m_dwCopyID = 0;
        m_uRoomID = 0;
        m_nCount = 0;
    }

    INT32 GetCount()
    {
        return m_nCount;
    }

    UINT64 GetRoomID()
    {
        return m_uRoomID;
    }

    INT32 GetPosByID(UINT64 uRoleID)
    {
        for (int i = 0; i < m_nCount; i++)
        {
            if (m_uRoleID[i] == uRoleID)
            {
                return i;
            }
        }

        return -1;
    }

    BOOL AddPlayer(UINT64 uRoleID, UINT32 dwCamp)
    {
        if (m_nCount >= 3)
        {
            return FALSE;
        }

        m_uRoleID[m_nCount] = uRoleID;
        m_nCamp[m_nCount] = dwCamp;
        m_nCount += 1;
        return TRUE;
    }

    BOOL DeletePlayer(UINT64 uRoleID)
    {
        for (int i = 0; i < m_nCount; i++)
        {
            if (m_uRoleID[i] == uRoleID)
            {
                for (int j = i; j < m_nCount; j++)
                {
                    m_uRoleID[j] = m_uRoleID[j + 1];
                    m_nCamp[j] = m_nCamp[j + 1];
                }
            }
        }

        m_uRoleID[m_nCount - 1] = 0;

        m_nCamp[m_nCount - 1] = 0;

        m_nCount -= 1;

        return TRUE;
    }

    UINT64 m_uRoomID = 0;
    UINT64 m_uRoleID[10] = {0};
    INT32  m_nCamp[10] = {0};
    INT32  m_nCount = 0;
    UINT32 m_dwCopyID = 0;
    UINT32 m_dwCopyType = 0;
};

class CRoomList : public AVLTree<UINT64, CRoomItem>
{
public:
    CRoomList();

    ~CRoomList();

public:
    CRoomItem*      GetRoomItem(UINT64 uRoomID);

    CRoomItem*      GetAvailableRoom();

    BOOL            DestroyRoom(UINT64 uRoomID);

    CRoomItem*      CreateEmptyRoom();

    BOOL            NotifyRoomChange(UINT64 uRoomID);

    BOOL            NotifyRoomChange(CRoomItem* pRoomItem);

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

    BOOL        Init();

    BOOL        OnTimer(INT32 nParam);

    CRoomItem*  CreateRoom(UINT32 dwCopyID, UINT64 uRoleID);

    VOID        RegisterMessageHanler();
public:
    //*********************消息处理定义开始******************************
    BOOL    OnMsgCreateRoom(NetPacket* pNetPacket);
    BOOL    OnMsgLeaveRoom(NetPacket* pNetPacket);
    BOOL    OnMsgKickRoom(NetPacket* pNetPacket);
    BOOL    OnMsgJoinRoom(NetPacket* pNetPacket);
    BOOL    OnMsgStartBattle(NetPacket* pNetPacket);
    //*********************消息处理定义结束******************************

public:

    CRoomList               m_WaitRoomList;
};

#endif //__TEAM_COPY_MGR__
