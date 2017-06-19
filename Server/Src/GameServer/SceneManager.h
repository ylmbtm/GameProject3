#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_
#include "Scene.h"
//#include "Utility/AVLTree.h"

//typedef AVLTree<UINT32, CScene*> SceneMap;
typedef Hash_Map<UINT32, CScene*>  SceneMap;

class CSceneManager
{
public:
	CSceneManager();

	~CSceneManager();

	BOOL		Init(UINT32 dwReserved);

	BOOL		Uninit();
public:
	BOOL		CreateScene(UINT32 dwSceneType);

	CScene*		GetSceneByID(UINT32 dwSceneID);

	BOOL		LoadDefaultScene();

	BOOL        DispatchPacket(NetPacket *pNetPack);

	BOOL		OnUpdate( UINT32 dwTick );

	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgCreateSceneReq(NetPacket *pNetPack);


	//*********************消息处理定义结束******************************

protected:
	SceneMap	m_mapSceneList;
};

#endif //_SCENE_MANAGER_H_
