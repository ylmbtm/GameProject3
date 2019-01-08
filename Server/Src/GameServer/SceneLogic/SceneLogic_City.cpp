#include "stdafx.h"
#include "SceneLogic_City.h"
#include "../GameObject/SceneObject.h"
#include "../Scene.h"
#include "../Message/Game_Define.pb.h"

SceneLogic_City::SceneLogic_City(CScene* pScene): SceneLogicBase(pScene)
{

}

SceneLogic_City::~SceneLogic_City()
{

}

BOOL SceneLogic_City::OnObjectCreate(CSceneObject* pObject)
{
	if(pObject->GetObjType() == OT_PLAYER)
	{
		ERROR_RETURN_TRUE(pObject->m_dwCamp < m_vtBornPos.size());
		ERROR_RETURN_TRUE(pObject->m_dwCamp > 0);
		pObject->SetPos(m_vtBornPos[pObject->m_dwCamp].m_x, m_vtBornPos[pObject->m_dwCamp].m_y, m_vtBornPos[pObject->m_dwCamp].m_z);
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

BOOL SceneLogic_City::Update(UINT64 uTick)
{
	return TRUE;
}

BOOL SceneLogic_City::TimeUP()
{
	return TRUE;
}
