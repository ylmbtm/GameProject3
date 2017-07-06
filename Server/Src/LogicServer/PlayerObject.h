#ifndef __WS_PLAYER_OBJECT_H__
#define __WS_PLAYER_OBJECT_H__
#include "GameDefine.h"
#include "Utility/AVLTree.h"
#include "Utility/Position.h"
#include "ModuleBase.h"

enum MouduleType
{
	MT_ROLE,
	MT_END
};


enum Copy_State
{
	CS_START,
	CS_FINISHED
};

class CPlayerObject
{
public:
	CPlayerObject();

	~CPlayerObject();
	
	//  初始化玩家对象
	BOOL		Init();
	//  反初始化玩家对家
	BOOL		Uninit();

	virtual BOOL OnCreate(UINT64 u64RoleID);

	virtual BOOL OnDestroy(UINT64 u64RoleID);

	virtual BOOL OnLogin(UINT64 u64RoleID);

	virtual BOOL OnLogout(UINT64 u64RoleID);

	virtual BOOL OnNewDay();

	virtual BOOL OnLoadData(UINT64 u64RoleID);

	virtual BOOL DispatchPacket(NetPacket *pNetPack);

	BOOL    SendProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata);

public: //全部是操作方法
	BOOL    SendToScene(UINT32 dwCopyID,UINT32 dwSvrID);
	BOOL	SendNotifyIntoScene(UINT32 dwCopyID,  UINT32 dwCopyType,UINT32 dwSvrID);
	BOOL	SendLeaveScene(UINT32 dwCopyID, UINT32 dwSvrID);

	BOOL	SetConnectID(UINT32 dwProxyID, UINT32 dwClientID);

	//模块函数
	BOOL			CreateAllModule();
	BOOL			DestroyAllModule();
	BOOL			OnModuleFnished();
	BOOL			IsAllModuleOK();
	BOOL			OnAllModuleOK();
	BOOL			SetAllModuleOK();
	CModuleBase*	GetModuleByType(MouduleType MType);

public:
	UINT64			GetObjectID();
public:
	UINT64			m_u64ID;
	UINT32			m_dwProxyConnID;
	UINT32			m_dwClientConnID;
	std::vector<CModuleBase*> m_MoudleList;

public:
	UINT32      m_dwCopyID;     //当前的副本ID
	UINT32      m_dwToCopyID;   //正在前往的副本ID
	UINT32      m_dwCopyType;   //当前的副本类型
	Copy_State  m_CopyState;    //副本状态
	UINT32      m_dwServerID;   //副本服务器的ID
};


#endif //__WS_PLAYER_OBJECT_H__
