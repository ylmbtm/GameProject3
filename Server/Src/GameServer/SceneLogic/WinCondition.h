#ifndef __BATTLE_RESULT_H__
#define __BATTLE_RESULT_H__

#include "XMath.h"

enum EWinCondition
{
	EWC_NONE,
	EWC_KILL_ALL,       //击杀全部怪物
	EWC_KILL_NUM,       //击杀指定数量怪物
	EWC_DESTINATION,    //达到目的地
	EWC_PLAYER_ALIVE,   //存活下来
	EWC_NPC_ALIVE,      //护送npc
	EWC_END
};

enum EBattleCamp
{
	EBC_NONE,     //中立阵营
	EBC_PLAYER,   //玩家阵营
	EBC_MONSTER,  //怪物阵营
};


class CWinCondition
{
public:
	CWinCondition(EWinCondition type = EWC_NONE);

	~CWinCondition();

	EWinCondition GetConditionType();

	VOID SetConditionType(EWinCondition type);

	BOOL SetDestination(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

	BOOL SetNpcID(UINT32 dwNpcID);

	BOOL SetKillMonster(UINT32 dwMonsterID, UINT32 dwKillNum);
private:
	EWinCondition   m_ConditionType = EWC_NONE;//判定类型
	Rect2D          m_DestRect;
	UINT32          m_dwNpcID = 0;
	UINT32          m_dwMonsterID = 0;
	UINT32          m_dwKillNum = 0;
};


#endif //__BATTLE_RESULT_H__