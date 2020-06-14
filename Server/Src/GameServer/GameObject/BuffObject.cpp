#include "stdafx.h"
#include "BuffObject.h"
#include "../Message/Game_Define.pb.h"
#include "TimerManager.h"
#include "StaticData.h"
#include "StaticStruct.h"
#include "SceneObject.h"
#include "LuaManager.h"

CBuffObject::CBuffObject(CSceneObject* pObject, UINT32 dwBuffID)
{
	m_dwBuffID = dwBuffID;
	m_pSceneObject = pObject;
	m_bOver = FALSE;
	m_dwStartTime = 0;
	m_dwLastTime = 0;
	m_pBuffInfo = CStaticData::GetInstancePtr()->GetBuffInfo(m_dwBuffID);
}

CBuffObject::~CBuffObject()
{
	m_dwBuffID = 0;
	m_pSceneObject = NULL;
	m_pBuffInfo = NULL;
	m_bOver = FALSE;
	m_dwStartTime = 0;
	m_dwLastTime = 0;
}

BOOL CBuffObject::OnAddBuff()
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);

	m_pSceneObject->m_dwStatus |= m_pBuffInfo->ChangeStatus;

	OnEffect(TRUE);

	if (!m_pBuffInfo->LuaAdd.empty() && (m_pBuffInfo->LuaAdd.size() > 1))
	{
		//LuaManager::GetInstancePtr()->CallLua(m_pBuffInfo->LuaAdd, "pp=", m_pSceneObject, this);
	}

	return TRUE;
}

BOOL CBuffObject::OnRemoveBuff()
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);

	m_pSceneObject->m_dwStatus &= ~m_pBuffInfo->ChangeStatus;

	for(int i = 0; i < PROPERTY_NUM; i++)
	{
		m_pSceneObject->m_Propertys[i] -= m_PtyChange[i];
	}

	if (!m_pBuffInfo->LuaRemove.empty() && (m_pBuffInfo->LuaRemove.size() > 1))
	{
		//LuaManager::GetInstancePtr()->CallLua(m_pBuffInfo->LuaRemove, "pp=", m_pSceneObject, this);
	}

	return TRUE;
}

BOOL CBuffObject::OnEffect(BOOL bFirst)
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);

	for (int i = 0; i < PROPERTY_NUM; i++)
	{
		m_PtyChange[i] += m_pSceneObject->m_Propertys[i] * m_pBuffInfo->PtyPercent[i] / 10000;
		m_PtyChange[i] += m_pBuffInfo->PtyValue[i];

		m_pSceneObject->m_Propertys[i] += m_PtyChange[i];
	}

	if (!bFirst && !m_pBuffInfo->LuaTick.empty() && (m_pBuffInfo->LuaTick.size() > 1))
	{
		//LuaManager::GetInstancePtr()->CallLua(m_pBuffInfo->TickLua, "pp=", m_pSceneObject, this);
	}
	return TRUE;
}

BOOL CBuffObject::OnUpdate( UINT64 uTick )
{
	if(m_bOver)
	{
		return TRUE;
	}

	if(m_pBuffInfo->Interval != 0)
	{
		if((uTick - m_dwLastTime) > m_pBuffInfo->Interval)
		{
			OnEffect();
			m_dwLastTime += m_pBuffInfo->Interval;
		}
	}

	if((uTick - m_dwStartTime) > m_pBuffInfo->TotalTime)
	{
		OnRemoveBuff();
		m_bOver = TRUE;
	}


	return TRUE;
}

BOOL CBuffObject::IsOver()
{
	return m_bOver;
}

VOID CBuffObject::SetOver()
{
	m_bOver = TRUE;
}

