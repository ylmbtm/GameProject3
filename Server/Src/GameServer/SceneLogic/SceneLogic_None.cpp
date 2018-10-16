#include "stdafx.h"
#include "SceneLogic_None.h"
#include "../Scene.h"




SceneLogic_None::SceneLogic_None(CScene* pScene): SceneLogicBase(pScene)
{

}

SceneLogic_None::~SceneLogic_None()
{

}

BOOL SceneLogic_None::OnObjectCreate(CSceneObject* pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnObjectDie(CSceneObject* pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnPlayerEnter(CSceneObject* pPlayer)
{
	return TRUE;
}

BOOL SceneLogic_None::OnPlayerLeave(CSceneObject* pPlayer)
{
	return FALSE;
}

BOOL SceneLogic_None::Update(UINT64 uTick)
{
	SceneLogicBase::Update(uTick);

	if(m_pScene->GetStartTime() == 0)
	{
		if(CommonFunc::GetCurrTime() - m_pScene->GetCreateTime() > 60)
		{
			m_pScene->SetFinished();
		}
	}

	return TRUE;
}
