#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "LogicMsgHandler.h"

class CGameService : public IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

	BOOL		Init();

	BOOL		Uninit();

	BOOL		Run();

	BOOL		OnNewConnect(INT32 nConnID);

	BOOL		OnCloseConnect(INT32 nConnID);

	BOOL		OnSecondTimer();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL        SendCmdToDBConnection(INT32 nMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata);

	BOOL	    ConnectToLogServer();

	BOOL		ConnectToLoginSvr();

	BOOL		ConnectToDBSvr();

	BOOL		ConnectToCenterSvr();

	BOOL		RegisterToLoginSvr();

	BOOL		RegisterToDBSvr();

	BOOL		RegisterToCenterSvr();

	UINT32      GetDBConnID();

	UINT32      GetLoginConnID();

	UINT32      GetServerID();

	UINT32      GetCenterConnID();

	BOOL		ReportServerStatus();

	UINT32      GetLogSvrConnID();

	VOID        RegisterMessageHanler();

	UINT64      GetSvrOpenTime();
public:
	CLogicMsgHandler m_LogicMsgHandler;

	UINT32			m_dwLogConnID;
	UINT32			m_dwLoginConnID;
	UINT32			m_dwDBConnID;
	UINT32          m_dwCenterConnID;   //中心服的连接ID
	UINT64          m_uSvrOpenTime;
	UINT32          m_dwDbErrorCount;
	BOOL            m_bRegSuccessed;
public:
	//*********************消息处理定义开始******************************
	BOOL        OnMsgRegToLoginAck(NetPacket* pNetPacket);
	BOOL        OnMsgRegToCenterAck(NetPacket* pNetPacket);
	BOOL        OnMsgUpdateInfoAck(NetPacket* pNetPacket);
	BOOL        OnMsgDBWriteErrorNty(NetPacket* pNetPacket);
	BOOL        OnMsgRegToDBSvrAck(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif
