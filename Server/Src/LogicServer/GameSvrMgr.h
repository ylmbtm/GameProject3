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

	BOOL	DispatchPacket( NetPacket *pNetPacket);

	UINT32	GetServerIDByCopyID(UINT32 dwCopyID);

	UINT32	GetFreeGameServerID();

	BOOL	CreateScene(UINT32 dwCopyType, UINT64 CreateParam);

	BOOL	SendCreateSceneCmd(UINT32 dwServerID, UINT32 dwCopyType, UINT64 CreateParam);

	UINT32  GetConnIDBySvrID(UINT32 dwServerID);

	BOOL	GetMainScene(UINT32 &dwServerID, UINT32 &dwConnID, UINT32 &dwCopyID);

	BOOL    SendPlayerToScene(UINT64 uID, UINT32 dwCopyID, UINT32 dwSvrID);

public:
	BOOL	OnCloseConnect(UINT32 dwConnID);
	BOOL	OnMsgGameSvrRegister(NetPacket *pNetPacket);
	//BOOL	OnMsgCopyReportReq(NetPacket *pNetPacket);
	
	

public:
	std::map<UINT32, GameSvrInfo> m_mapGameSvr; //服务器ID-->信息
};

#endif
