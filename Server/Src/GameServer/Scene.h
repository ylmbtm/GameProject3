#ifndef _SCENE_H_
#define _SCENE_H_
#include "GameObject/PlayerObject.h"
#include "GameObject/WorldObject.h"
#include "GridManager.h"

class CScene
{
public:
	CScene();

	~CScene();

	BOOL	DispatchPacket(NetPacket *pNetPack);

	BOOL	Init(UINT32 dwSceneType, UINT32 dwSceneID, UINT32 dwLogicType);

	BOOL	Uninit();

	BOOL	AddToMap(CWorldObject *pWorldObject);

	BOOL	AddToMapPos(CWorldObject *pWorldObject, FLOAT x, FLOAT z);

	BOOL	RemoveFromMap(CWorldObject *pWorldObject);

	BOOL	AddToUpdateList(CWorldObject *pWorldObject);

	BOOL	RemoveFromUpList(CWorldObject *pWorldObject);

	BOOL	HandleUpdateList();

	BOOL	HandleUpdateObject(CWorldObject *pWorldObject);
	
	UINT32	GetSceneID(){ return m_dwSceneID; }

	BOOL    OnUpdate( UINT32 dwTick );

	BOOL	CreateSceneLogic(UINT32 dwLogicType);

	BOOL	SendNewObjectToGrids(CWorldObject *pWorldObject, INT32 Grids[9]);

	BOOL	SendNewGridsToObject(INT32 Grids[9], CPlayerObject *pPlayerObj);

	BOOL	SendUpdateObjectToGrids(CWorldObject *pWorldObj, INT32 Grids[9]);

	BOOL	SendRemoveObjectToGrids(UINT64 u64RoleID, INT32 Grids[9]);

	BOOL	SendRemoveGridsToPlayer(INT32 Grids[9], CPlayerObject *pPlayerObj);

	BOOL	SendUpdateObjectToMyself(CWorldObject *pWorldObj);

public:
	BOOL	SetSceneMapCoords( INT32 nLeft, INT32 nRight, INT32 nTop, INT32 nBottom );

public:
	

protected:
	UINT32							m_dwSceneID;
	UINT32							m_dwSceneType;

	SceneLogicBase					*m_pSceneLogic;

	CPlayerObjectMgr				m_PlayerObjectMgr;		//玩家管理器

	CGridManager					m_GridManager;			//地图格子管理器

	CUpdateObjectMgr				m_UpdateObjectMgr;		//更新的对象管理器
	
	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgTransRoleDataReq(NetPacket *pNetPacket);
	BOOL OnMsgLeaveGameReq(NetPacket *pNetPacket);
	BOOL OnMsgRoleAction(NetPacket *pNetPacket);
	BOOL OnMsgRoleAttack(NetPacket *pNetPacket);
	
	
	//*********************消息处理定义结束******************************



	
};

#endif //_MAP_H_


