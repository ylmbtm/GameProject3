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
	m_pBuffInfo = CConfigData::GetInstancePtr()->GetBuffInfo(m_dwBuffID);
}

CBuffObject::~CBuffObject()
{
	m_dwBuffID = 0;
	m_pSceneObject = NULL;
	m_pBuffInfo = NULL;
}

BOOL CBuffObject::OnAddBuff()
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);
	switch(m_pBuffInfo->BuffType)
	{
		case BFT_NONE:
		{
			m_pSceneObject->m_dwObjState |= m_pBuffInfo->BuffEffect;
		}
		break;

		default:
		{

		}
	}

	for(int i = 0; i < PROPERTY_NUM; i++)
	{
		m_PtyChange[i] += m_pSceneObject->m_Propertys[i] * m_pBuffInfo->PtyPercent[i] / 10000;
		m_PtyChange[i] += m_pBuffInfo->PtyValue[i];

		m_pSceneObject->m_Propertys[i] += m_PtyChange[i];
	}


	TimerManager::GetInstancePtr()->AddDiffTimer(1, m_dwBuffID, &CBuffObject::OnUpdate, this);

	return TRUE;
}

BOOL CBuffObject::OnRemoveBuff()
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);
	switch(m_pBuffInfo->BuffType)
	{
		case BFT_NONE:
		{


		}
		break;

		default:
		{

		}
	}

	for(int i = 0; i < PROPERTY_NUM; i++)
	{
		m_pSceneObject->m_Propertys[i] -= m_PtyChange[i];
	}

	return TRUE;
}

BOOL CBuffObject::OnEffect()
{
	ERROR_RETURN_FALSE(m_pBuffInfo != NULL);
	switch(m_pBuffInfo->BuffType)
	{
		case BFT_NONE:
		{


		}
		break;

		default:
		{

		}
	}

	return TRUE;
}

BOOL CBuffObject::OnUpdate( UINT32 dwTick )
{
	return TRUE;
}

