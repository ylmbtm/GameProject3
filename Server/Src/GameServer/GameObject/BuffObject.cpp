#include "stdafx.h"
#include "BuffObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "..\Message\Game_Define.pb.h"
#include "TimerManager.h"
#include "..\ConfigData\ConfigData.h"
#include "..\ConfigData\ConfigStruct.h"
#include "SceneObject.h"


CBuffObject::CBuffObject(CSceneObject* pObject, UINT32 dwBuffID)
{
	m_dwBuffID = dwBuffID;
	m_pSceneObject = pObject;
	m_dwBuffType = 0;
	m_dwEffTime = 0;
	m_pBuffInfo = NULL;
}

CBuffObject::~CBuffObject()
{
	m_dwBuffID = 0;
	m_pSceneObject = NULL;
	m_dwBuffType = 0;
	m_dwEffTime = 0;
	m_pBuffInfo = NULL;
}

BOOL CBuffObject::OnAddBuff()
{
	if(m_pBuffInfo == NULL)
	{
		m_pBuffInfo = CConfigData::GetInstancePtr()->GetBuffInfo(m_dwBuffID);
		ERROR_RETURN_FALSE(m_pBuffInfo != NULL);
	}

	switch(m_dwBuffType)
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

	TimerManager::GetInstancePtr()->AddDiffTimer(1, m_dwBuffID, &CBuffObject::OnUpdate, this);

	return TRUE;
}

BOOL CBuffObject::OnRemoveBuff()
{
	switch(m_dwBuffType)
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

BOOL CBuffObject::OnEffect()
{
	switch(m_dwBuffType)
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

BOOL CBuffObject::OnUpdate( UINT32 dwData )
{
	return TRUE;
}

