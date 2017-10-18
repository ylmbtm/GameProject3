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
// 	switch(m_pBuffInfo->BuffType)
// 	{
// 		case BFT_NONE:
// 		{
// 			m_pSceneObject->m_dwObjState |= m_pBuffInfo->BuffEffect;
// 		}
// 		break;
//
// 		default:
// 		{
//
// 		}
// 	}

	for(int i = 0; i < PROPERTY_NUM; i++)
	{
		m_PtyChange[i] += m_pSceneObject->m_Propertys[i] * m_pBuffInfo->PtyPercent[i] / 10000;
		m_PtyChange[i] += m_pBuffInfo->PtyValue[i];

		m_pSceneObject->m_Propertys[i] += m_PtyChange[i];
	}


	//LuaManager::GetInstancePtr()->CallLua(m_pBuffInfo->AddLua, "pp=", m_pSceneObject, this);

	//TimerManager::GetInstancePtr()->AddDiffTimer(1, m_dwBuffID, &CBuffObject::OnUpdate, this);

	return TRUE;
}

BOOL CBuffObject::OnRemoveBuff()
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);
// 	switch(m_pBuffInfo->BuffType)
// 	{
// 		case BFT_NONE:
// 		{
//
//
// 		}
// 		break;
//
// 		default:
// 		{
//
// 		}
// 	}

	for(int i = 0; i < PROPERTY_NUM; i++)
	{
		m_pSceneObject->m_Propertys[i] -= m_PtyChange[i];
	}

	//LuaManager::GetInstancePtr()->CallLua(m_pBuffInfo->RemoveLua, "pp=", m_pSceneObject, this);

	return TRUE;
}

BOOL CBuffObject::OnEffect()
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);
// 	switch(m_pBuffInfo->BuffType)
// 	{
// 		case BFT_NONE:
// 		{
//
//
// 		}
// 		break;
//
// 		default:
// 		{
//
// 		}
// 	}
	//LuaManager::GetInstancePtr()->CallLua(m_pBuffInfo->TickLua, "pp=", m_pSceneObject, this);
	return TRUE;
}

BOOL CBuffObject::OnUpdate( UINT32 dwTick )
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


