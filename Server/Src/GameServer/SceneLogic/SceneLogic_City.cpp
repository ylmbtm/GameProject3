#include "stdafx.h"
#include "SceneLogic_City.h"
#include "../GameObject/SceneObject.h"
#include "../Scene.h"
#include "../ServerData/ServerDefine.h"
#include "../Message/Game_Define.pb.h"
#include "Log.h"

SceneLogic_City::SceneLogic_City(CScene* pScene): SceneLogicBase(pScene)
{

}

SceneLogic_City::~SceneLogic_City()
{

}

BOOL SceneLogic_City::OnObjectCreate(CSceneObject* pObject)
{
	ERROR_RETURN_TRUE(pObject->m_dwCamp > CT_NONE);
	ERROR_RETURN_TRUE(pObject->m_dwCamp < CT_CMAP_END);
	if(pObject->GetObjType() == OT_PLAYER)
	{
		pObject->SetPos(m_vtBornPos[pObject->m_dwCamp].m_x, 0, m_vtBornPos[pObject->m_dwCamp].m_y);
	}
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
