#include "Utility\XMath.h"
#ifndef __BATTLE_RESULT_H__
#define __BATTLE_RESULT_H__

enum RESULTTYPE
{
	BRT_NONE,
	BRT_KILL_ALL,			//击杀全部怪物
	BRT_KILL_NUM,			//击杀指定数量怪物
	BRT_DESTINATION,			//达到目的地
	BRT_PLAYER_ALIVE,				//存活下来
	BRT_NPC_ALIVE,			//护送npc
	BRT_END
};


class BattleResult
{
public:
	BattleResult(RESULTTYPE type = BRT_NONE);
	~BattleResult();

	RESULTTYPE GetResultType();
	
private:
	RESULTTYPE	m_Type;//判定类型
};


class CResultKillAll: public BattleResult
{
public:
	CResultKillAll();
};

class CResultKillNum: public BattleResult
{
public:
	CResultKillNum(UINT32 dwMonsterID, UINT32 dwKillNum);

private:
	UINT32 m_dwMonsterID;
	UINT32 m_dwKillNum;
};

class CResultPlayerAlive: public BattleResult
{
public:
	CResultPlayerAlive();
};

class CResultDestination: public BattleResult
{
public:
	CResultDestination(FLOAT left,FLOAT top,FLOAT right,FLOAT bottom);

	BOOL SetDestination(FLOAT left,FLOAT top,FLOAT right,FLOAT bottom);
private:
	Rect2d  m_DestRect;
};

class CResultNpcAlive: public BattleResult
{
public:
	CResultNpcAlive(UINT32 dwNpcID);
private:

	UINT32 m_dwNpcID;
};

#endif //__BATTLE_RESULT_H__