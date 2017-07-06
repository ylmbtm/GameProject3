#ifndef _SCENE_H_
#define _SCENE_H_
#include "GameObject/SceneObject.h"
#include "GameObject/WorldObject.h"
#include "GridManager.h"
#include "SceneLogic/SceneLogic_Base.h"
#include "GameObject/SceneObjectMgr.h"

class CScene
{
public:
	CScene();

	~CScene();

	BOOL	DispatchPacket(NetPacket *pNetPack);

	BOOL	Init(UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwLogicType);

	BOOL	Uninit();

	BOOL	AddToMap(CWorldObject *pWorldObject);

	BOOL	AddToMapPos(CWorldObject *pWorldObject, FLOAT x, FLOAT z);

	BOOL	RemoveFromMap(CWorldObject *pWorldObject);

	BOOL	AddToUpdateList(CWorldObject *pWorldObject);

	BOOL	RemoveFromUpList(CWorldObject *pWorldObject);

	BOOL	HandleUpdateList();

	BOOL	HandleUpdateObject(CWorldObject *pWorldObject);
	
	UINT32	GetCopyID(){ return m_dwCopyID; }
	UINT32  GetCopyType() { return m_dwCopyType; }

	BOOL    OnUpdate( UINT32 dwTick );

	BOOL	CreateSceneLogic(UINT32 dwLogicType);

	BOOL	SendNewObjectToGrids(CWorldObject *pWorldObject, INT32 Grids[9]);

	BOOL	SendNewGridsToObject(INT32 Grids[9], CSceneObject *pSceneObj);

	BOOL	SendUpdateObjectToGrids(CWorldObject *pWorldObj, INT32 Grids[9]);

	BOOL	SendRemoveObjectToGrids(UINT64 u64RoleID, INT32 Grids[9]);

	BOOL	SendRemoveGridsToPlayer(INT32 Grids[9], CSceneObject *pSceneObj);

	BOOL	SendUpdateObjectToMyself(CWorldObject *pWorldObj);

public:
	BOOL	IsCopyOver();
	BOOL    IsFinished();
public:
	BOOL	SetSceneMapCoords( INT32 nLeft, INT32 nRight, INT32 nTop, INT32 nBottom );

public:
	BOOL							m_bOver;   //标示副本己经结束，结算己经完成,可以销毁
	BOOL							m_bFinished; //标标副本己经完成，还示结算

protected:
	UINT32							m_dwCopyID;		//当前副本实例ID
	UINT32							m_dwCopyType;   //当前副本TYPE

	SceneLogicBase					*m_pSceneLogic;

	CSceneObjectMgr					m_SceneObjectMgr;		//玩家管理器

	CGridManager					m_GridManager;			//地图格子管理器

	CUpdateObjectMgr				m_UpdateObjectMgr;		//更新的对象管理器
	
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgTransRoleDataReq(NetPacket *pNetPacket);
	BOOL OnMsgEnterSceneReq(NetPacket *pNetPacket);
	BOOL OnMsgLeaveSceneReq(NetPacket *pNetPacket);
	BOOL OnMsgRoleMoveReq(NetPacket *pNetPacket);
	BOOL OnMsgRoleAttack(NetPacket *pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif //_MAP_H_


