#include "stdafx.h"
#include "BuffObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "..\Message\Game_Define.pb.h"
#include "TimerManager.h"


CBuffObject::CBuffObject(CSceneObject *pObject, UINT32 dwBuffID)
{
	m_dwBuffID = dwBuffID;
	m_pSceneObject = pObject;
    m_dwBuffType = 0;
    m_dwEffTime = 0;
}

CBuffObject::~CBuffObject()
{

}

BOOL CBuffObject::OnAddBuff()
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

