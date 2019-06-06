#ifndef __BATTLE_RESULT_H__
#define __BATTLE_RESULT_H__

#include "XMath.h"

enum RESULTTYPE
{
	BRT_NONE,
	BRT_KILL_ALL,       //击杀全部怪物
	BRT_KILL_NUM,       //击杀指定数量怪物
	BRT_DESTINATION,    //达到目的地
	BRT_PLAYER_ALIVE,   //存活下来
	BRT_NPC_ALIVE,      //护送npc
	BRT_END
};


class BattleResult
{
public:
	BattleResult(RESULTTYPE type = BRT_NONE);

	~BattleResult();

	RESULTTYPE GetResultType();

	VOID SetResultType(RESULTTYPE type);

	BOOL SetDestination(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

	BOOL SetNpcID(UINT32 dwNpcID);

	BOOL SetKillMonster(UINT32 dwMonsterID, UINT32 dwKillNum);
private:
	RESULTTYPE	m_Type;//判定类型
	Rect2D		m_DestRect;
	UINT32		m_dwNpcID;
	UINT32		m_dwMonsterID;
	UINT32		m_dwKillNum;
};


#endif //__BATTLE_RESULT_H__