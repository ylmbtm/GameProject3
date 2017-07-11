#ifndef _SCENE_H_
#define _SCENE_H_
#include "GameObject/SceneObject.h"
#include "SceneLogic/SceneLogic_Base.h"

class CScene
{
public:
	CScene();

	~CScene();

	BOOL	DispatchPacket(NetPacket *pNetPack);

	BOOL	Init(UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwLogicType);

	BOOL	Uninit();
	
	UINT32	GetCopyID();

	UINT32  GetCopyType();

	BOOL    OnUpdate( UINT32 dwTick );

	BOOL	CreateSceneLogic(UINT32 dwLogicType);

    BOOL    BroadNewObject(CSceneObject *pSceneObject);

    BOOL    BroadRemoveObject(CSceneObject *pSceneObject);

	BOOL	SendAllNewObjectToPlayer(CSceneObject *pSceneObject);

    CSceneObject* GetPlayer(UINT64 uID);
    BOOL          AddPlayer(CSceneObject *pSceneObject);

	CSceneObject* GetSceneObject(UINT64 uID);



public:
	BOOL	IsCopyOver();
	BOOL    IsFinished();

public:
	BOOL							m_bOver;   //标示副本己经结束，结算己经完成,可以销毁
	BOOL							m_bFinished; //标标副本己经完成，还示结算
protected:
	UINT32							m_dwCopyID;		//当前副本实例ID
	UINT32							m_dwCopyType;   //当前副本TYPE

	SceneLogicBase					*m_pSceneLogic;

	std::map<UINT64, CSceneObject*>	 m_PlayerMap;		//玩家管理器
    std::map<UINT64, CSceneObject*>  m_MonsterMap;      //怪物管理器
	
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgTransRoleDataReq(NetPacket *pNetPacket);
	BOOL OnMsgEnterSceneReq(NetPacket *pNetPacket);
	BOOL OnMsgLeaveSceneReq(NetPacket *pNetPacket);
	BOOL OnMsgRoleMoveReq(NetPacket *pNetPacket);
	BOOL OnMsgRoleAttack(NetPacket *pNetPacket);
    BOOL OnMsgRoleDisconnect(NetPacket *pNetPacket);
    //*********************消息处理定义结束******************************
};

#endif //_MAP_H_


