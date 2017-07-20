#ifndef __BATTLE_RESULT_H__
#define __BATTLE_RESULT_H__

enum RESULTTYPE
{
	BRT_NONE,
	BRT_KILLMONSTER,			//限定时间内击杀怪物
	BRT_DESTINATION,			//限定时间达到目的地
	BRT_ALIVE,					//限定时间存活下来
	BRT_NPC_ALIVE,				//限定时间护送npc
	BRT_KILL_ONLYMONSTERE,		//限定时间只允许击杀怪物
};

class BattleResult
{
public:
	BattleResult(RESULTTYPE type = BRT_NONE)
	{
		m_Type = type;
	}
	~BattleResult()
	{
	}

	RESULTTYPE GetResultType()
	{
		return m_Type;
	}
	
private:
	RESULTTYPE	m_Type;//判定类型
};

class BR_KillMonsterNum: public BattleResult
{
public:
	BR_KillMonsterNum(): BattleResult(BRT_KILLMONSTER)
	{
	}

private:
	UINT32 m_dwMonsterID;
	UINT32 m_dwKillNum;
};

class BR_RoleAlive: public BattleResult
{
public:
	BR_RoleAlive(): BattleResult(BRT_ALIVE)
	{
	}
};

class BR_KillOnlyMonster: public BattleResult
{
public:
	BR_KillOnlyMonster(): BattleResult(BRT_KILLMONSTER)
	{
	}
	UINT32 m_dwMonsterID;
};


class BR_ReachPostion: public BattleResult
{
public:
	BR_ReachPostion(): BattleResult(BRT_DESTINATION)
	{
	}

private:
	FLOAT left, top, right, down;
};

class BR_NpcAlive: public BattleResult
{
public:
	BR_NpcAlive(): BattleResult(BRT_NPC_ALIVE)
	{
	}
private:

	UINT32 m_dwNpcID;
};

#endif //__BATTLE_RESULT_H__