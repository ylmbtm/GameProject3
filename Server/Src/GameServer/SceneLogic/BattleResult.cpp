#include "stdafx.h"
#include "BattleResult.h"


BattleResult::BattleResult(RESULTTYPE type /*= BRT_NONE*/)
{
	m_Type = type;
}

BattleResult::~BattleResult()
{

}

RESULTTYPE BattleResult::GetResultType()
{
	return m_Type;
}

VOID BattleResult::SetResultType(RESULTTYPE type)
{
	m_Type = type;
}

BOOL BattleResult::SetDestination(FLOAT left,FLOAT top,FLOAT right,FLOAT bottom)
{
	m_DestRect.m_Left = left;
	m_DestRect.m_Top = top;
	m_DestRect.m_Right = right;
	m_DestRect.m_Bottom = bottom;

	return TRUE;
}

BOOL BattleResult::SetNpcID(UINT32 dwNpcID)
{
	m_dwNpcID = dwNpcID;

	return TRUE;
}

BOOL BattleResult::SetKillMonster(UINT32 dwMonsterID, UINT32 dwKillNum)
{
	m_dwMonsterID = dwMonsterID;
	m_dwKillNum = dwKillNum;

	return TRUE;
}
