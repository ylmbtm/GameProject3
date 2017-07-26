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

CResultKillAll::CResultKillAll() : BattleResult(BRT_KILL_ALL)
{

}

CResultKillNum::CResultKillNum(UINT32 dwMonsterID, UINT32 dwKillNum) : BattleResult(BRT_KILL_NUM)
{
	m_dwMonsterID = dwMonsterID;
	m_dwKillNum = dwKillNum;
}

CResultPlayerAlive::CResultPlayerAlive() : BattleResult(BRT_PLAYER_ALIVE)
{

}

CResultDestination::CResultDestination(FLOAT left,FLOAT top,FLOAT right,FLOAT bottom) : BattleResult(BRT_DESTINATION)
{
	m_DestRect.m_Left = left;
	m_DestRect.m_Top = top;
	m_DestRect.m_Right = right;
	m_DestRect.m_Bottom = bottom;
}

CResultNpcAlive::CResultNpcAlive(UINT32 dwNpcID) : BattleResult(BRT_NPC_ALIVE)
{
	m_dwNpcID = dwNpcID;
}


BOOL CResultDestination::SetDestination(FLOAT left,FLOAT top,FLOAT right,FLOAT bottom)
{
	m_DestRect.m_Left = left;
	m_DestRect.m_Top = top;
	m_DestRect.m_Right = right;
	m_DestRect.m_Bottom = bottom;

	return TRUE;
}