#include "stdafx.h"
#include "SceneLogic_City.h"
#include "../GameObject/SceneObject.h"
#include "../Scene.h"
#include "../ServerData/ServerDefine.h"
#include "../Message/Game_Define.pb.h"

SceneLogic_City::SceneLogic_City(CScene* pScene): SceneLogicBase(pScene)
{

}

SceneLogic_City::~SceneLogic_City()
{

}

BOOL SceneLogic_City::OnObjectCreate(CSceneObject* pObject)
{

	return TRUE;
}

BOOL SceneLogic_City::OnObjectDie(CSceneObject* pObject)
{

	return TRUE;
}


BOOL SceneLogic_City::OnPlayerEnter(CSceneObject* pPlayer)
{
	//玩家进入副本了。

	return TRUE;
}

BOOL SceneLogic_City::OnPlayerLeave(CSceneObject* pPlayer)
{
	return TRUE;
}



BOOL SceneLogic_City::Update(UINT32 dwTick)
{
	return TRUE;
}

BOOL SceneLogic_City::TimeUP()
{
	return TRUE;
}
