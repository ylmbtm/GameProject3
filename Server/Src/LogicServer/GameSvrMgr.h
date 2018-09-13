#ifndef __GAME_SVR_MGR__
#define __GAME_SVR_MGR__

#include "../Message/Msg_Copy.pb.h"
#include "../Message/Msg_Game.pb.h"
struct GameSvrInfo
{
	GameSvrInfo(UINT32 svrID, UINT32 conID)
	{
		dwSvrID = svrID;
		dwConnID = conID;
		dwLoad   = 0;
	}
	UINT32 dwSvrID;
	UINT32 dwConnID;
	UINT32 dwLoad;		//负载值
};

struct CityInfo
{
	CityInfo(UINT32 dwCopyID, UINT32 dwSvrID, UINT32 dwConID, UINT32 dwCopyGuid)
	{
		m_dwSvrID  = dwSvrID;
		m_dwConnID = dwConID;
		m_dwCopyID = dwCopyID;
		m_dwCopyGuid = dwCopyGuid;

	}
	UINT32 m_dwCopyID;
	UINT32 m_dwSvrID;
	UINT32 m_dwConnID;
	UINT32 m_dwCopyGuid;
};

class CGameSvrMgr
{
private:
	CGameSvrMgr(void);
	~CGameSvrMgr(void);
public:
	static CGameSvrMgr* GetInstancePtr();
public:

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	UINT32		GetServerIDByCopyID(UINT32 dwCopyGuid);

	UINT32		GetBestGameServerID();

	BOOL		SendCreateSceneCmd(UINT32 dwServerID, UINT32 dwCopyID, UINT32 dwCopyType, UINT64 CreateParam, UINT32 dwPlayerNum);

	UINT32		GetConnIDBySvrID(UINT32 dwServerID);

	BOOL		SendPlayerToMainCity(UINT64 u64ID, UINT32 dwCopyID);

	BOOL		SendPlayerToCopy(UINT64 u64ID, UINT32 dwServerID, UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCamp);

	BOOL		GetMainCityInfo(UINT32 dwCopyID, UINT32& dwServerID, UINT32& dwConnID, UINT32& dwCopyGuid);

	BOOL		CreateScene(UINT32 dwCopyID, UINT64 CreateParam, UINT32 dwPlayerNum, UINT32 dwCopyType );

public:
	//响应副本创建返回
	//////////////////////////////////////////////////////////////////////////
	BOOL		OnCreateMainCopy(CreateNewSceneAck& Ack);



	//响应副本结果返回
	//////////////////////////////////////////////////////////////////////////
	BOOL		OnMainCopyResult(BattleResultNty& Nty);

	//*********************消息处理定义开始******************************
public:
	BOOL	OnCloseConnect(UINT32 dwConnID);
	BOOL	OnMsgGameSvrRegister(NetPacket* pNetPacket); //响应副本服务器注册
	BOOL	OnMsgCreateSceneAck(NetPacket* pNetPacket);  //响应创建副本成功
	BOOL	OnMsgTransRoleDataAck(NetPacket* pNetPacket);//响应角色数据传输成功
	BOOL	OnMsgEnterSceneReq(NetPacket* pNetPacket);
	BOOL	OnMsgCopyReportReq(NetPacket* pNetPacket);
	BOOL	OnMsgBattleResultNty(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
public:
	std::map<UINT32, GameSvrInfo> m_mapGameSvr; //服务器ID-->副本服务器信息

	std::map<UINT32, CityInfo> m_mapCity;


};

#endif
