#include "stdafx.h"
#include "CommonMath.h"
#include "WinCondition.h"


CWinCondition::CWinCondition(EWinCondition type /*= BRT_NONE*/)
{
	m_ConditionType = type;
}

CWinCondition::~CWinCondition()
{

}

EWinCondition CWinCondition::GetConditionType()
{
	return m_ConditionType;
}

VOID CWinCondition::SetConditionType(EWinCondition type)
{
	m_ConditionType = type;
}

BOOL CWinCondition::SetDestination(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
	m_DestRect.m_fLeft = left;
	m_DestRect.m_fTop = top;
	m_DestRect.m_fRight = right;
	m_DestRect.m_fBottom = bottom;

	return TRUE;
}

BOOL CWinCondition::SetNpcID(UINT32 dwNpcID)
{
	m_dwNpcID = dwNpcID;

	return TRUE;
}

BOOL CWinCondition::SetKillMonster(UINT32 dwMonsterID, UINT32 dwKillNum)
{
	m_dwMonsterID = dwMonsterID;
	m_dwKillNum = dwKillNum;

	return TRUE;
}
