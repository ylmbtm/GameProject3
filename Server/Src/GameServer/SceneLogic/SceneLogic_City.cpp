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
		SceneLogicBase::OnObjectCreate(pObject);
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

BOOL SceneLogic_City::OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect)
{
	m_pScene->BroadRemoveObject(pPlayer);
	m_pScene->DeletePlayer(pPlayer->GetObjectGUID());
	//在主城里下线，宠物和伙伴也应该清除

	CSceneObject* pPet = m_pScene->GetSceneObject(pPlayer->m_uPetGuid);
	if (pPet != NULL)
	{
		m_pScene->BroadRemoveObject(pPet);
		m_pScene->DeleteMonster(pPlayer->m_uPetGuid);
	}

	CSceneObject* pPartner = m_pScene->GetSceneObject(pPlayer->m_uPartnerGuid);
	if (pPartner != NULL)
	{
		m_pScene->BroadRemoveObject(pPartner);
		m_pScene->DeleteMonster(pPlayer->m_uPartnerGuid);
	}

	return TRUE;
}

BOOL SceneLogic_City::Update(UINT64 uTick)
{

	return TRUE;
}

BOOL SceneLogic_City::OnTimeUP()
{
	return TRUE;
}
