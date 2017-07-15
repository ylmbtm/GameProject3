#include "stdafx.h"
#include "SceneLogic_Base.h"


SceneLogicBase::SceneLogicBase(CScene *pScene)
{
	m_pScene = pScene;

	m_bFinished = FALSE;
}

SceneLogicBase::~SceneLogicBase()
{

}

BOOL SceneLogicBase::OnObjectCreate(CSceneObject *pObject)
{
	

	return TRUE;
}

BOOL SceneLogicBase::OnPlayerEnter(CSceneObject *pPlayer)
{
	return TRUE;
}

BOOL SceneLogicBase::OnPlayerLeave(CSceneObject *pPlayer)
{
	return TRUE;
}


BOOL SceneLogicBase::OnObjectDie(CSceneObject *pObject)
{
	return TRUE;
}

BOOL SceneLogicBase::Update(UINT32 dwTick)
{
	if(dwTick >= m_dwEndTime)
	{
		OnTimeUP();
	}

	return TRUE;
}

BOOL SceneLogicBase::OnTimeUP()
{
	return TRUE;
}

BOOL SceneLogicBase::IsFinished()
{
	return m_bFinished;
}

BOOL SceneLogicBase::SetEndTime(UINT32 dwTime)
{
	m_dwEndTime = dwTime;

	return TRUE;
}

CScene* SceneLogicBase::GetScene()
{
	return m_pScene;
}
