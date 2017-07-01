#ifndef __GAME_SVR_MGR__
#define __GAME_SVR_MGR__

struct GameSvrInfo
{
	GameSvrInfo(UINT32 svrID, UINT32 conID)
	{
		dwSvrID = svrID;
		dwConnID = conID;
	}
	UINT32 dwSvrID;
	UINT32 dwConnID;
};

class CGameSvrMgr
{
private:
	CGameSvrMgr(void);
	~CGameSvrMgr(void);
public:
	static CGameSvrMgr* GetInstancePtr();
public:
	UINT32	GetServerIDByCopyID(UINT32 dwCopyID);

	UINT32	GetFreeGameServerID();

	BOOL	CreateScene(UINT32 dwCopyType, UINT64 CreateParam);

	BOOL	SendCreateSceneCmd(UINT32 dwServerID, UINT32 dwCopyType, UINT64 CreateParam);

	UINT32  GetConnIDBySvrID(UINT32 dwServerID);

	BOOL	GetMainScene(UINT32 &dwServerID, UINT32 &dwConnID, UINT32 &dwCopyID);
public:
	BOOL	OnMsgGameSvrRegister(NetPacket *pNetPacket);

	BOOL	OnCloseConnect(UINT32 dwConnID);

public:
	std::map<UINT32, GameSvrInfo> m_mapGameSvr; //服务器ID--信息
};

#endif
