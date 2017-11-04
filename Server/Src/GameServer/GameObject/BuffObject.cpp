#include "stdafx.h"
#include "BuffObject.h"
#include "CommandDef.h"
#include "Log.h"
#include "CommonFunc.h"
#include "CommonEvent.h"
#include "../Message/Game_Define.pb.h"
#include "TimerManager.h"
#include "../ConfigData/ConfigData.h"
#include "../ConfigData/ConfigStruct.h"
#include "SceneObject.h"
#include "LuaManager.h"

CBuffObject::CBuffObject(CSceneObject* pObject, UINT32 dwBuffID)
{
	m_dwBuffID = dwBuffID;
	m_pSceneObject = pObject;
	m_bOver = FALSE;
	m_pBuffInfo = CConfigData::GetInstancePtr()->GetBuffInfo(m_dwBuffID);
}

CBuffObject::~CBuffObject()
{
	m_dwBuffID = 0;
	m_pSceneObject = NULL;
	m_pBuffInfo = NULL;
	m_bOver = FALSE;
}

BOOL CBuffObject::OnAddBuff()
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);

	m_pSceneObject->m_dwObjState |= m_pBuffInfo->ChangeStaute;

	m_pSceneObject->m_dwBuffState |= m_pBuffInfo->BuffEffect;


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

	m_pSceneObject->m_dwObjState &= ~m_pBuffInfo->ChangeStaute;
	m_pSceneObject->m_dwBuffState &= ~m_pBuffInfo->BuffEffect;
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

BOOL CBuffObject::OnUpdate( UINT64 dwTick )
{
	if(m_bOver)
	{
		return TRUE;
	}

	if(m_pBuffInfo->Interval != 0)
	{
		if((dwTick - m_dwLastTime) > m_pBuffInfo->Interval)
		{
			OnEffect();
			m_dwLastTime += m_pBuffInfo->Interval;
		}
	}

	if((dwTick - m_dwStartTime) > m_pBuffInfo->TotalTime)
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


