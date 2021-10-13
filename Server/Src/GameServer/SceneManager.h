#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_
#include "Scene.h"
//#include "AVLTree.h"

//typedef AVLTree<UINT32, CScene*> SceneMap;
typedef std::map<UINT32, CScene*>  SceneMap;

class CSceneManager
{
public:
	CSceneManager();

	~CSceneManager();

	BOOL		Init(BOOL bMainLand);

	BOOL		Uninit();
public:
	BOOL		CreateScene(UINT32 dwCopyID, UINT32 dwCopyGuid, UINT32 dwCopyType, UINT32 dwPlayerNum, UINT64 uCreateKey);

	CScene*		GetSceneByCopyGuid(UINT32 dwCopyGuid);

	BOOL		LoadMainScene();

	BOOL        DispatchPacket(NetPacket* pNetPack);

	BOOL		OnUpdate( UINT64 uTick );

	UINT32      MakeCopyGUID();

	BOOL		SendCityReport();


public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgCreateSceneReq(NetPacket* pNetPack);

	//*********************消息处理定义结束******************************

protected:
	SceneMap	m_mapSceneList;
	UINT32      m_MaxCopyBaseID;
};

#endif //_SCENE_MANAGER_H_
