#ifndef __BATTLE_RESULT_H__
#define __BATTLE_RESULT_H__

enum RESULTTYPE
{
	BRT_NONE,
	BRT_KILL_ALL,			//击杀全部怪物
	BRT_KILL_NUM,			//击杀指定数量怪物
	BRT_REACH_POS,			//达到目的地
	BRT_ALIVE,				//存活下来
	BRT_NPC_ALIVE,			//护送npc
	BRT_END
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
	BR_KillMonsterNum(): BattleResult(BRT_KILL_NUM)
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
	BR_KillOnlyMonster(): BattleResult(BRT_KILL_NUM)
	{
	}
	UINT32 m_dwMonsterID;
};


class BR_ReachPostion: public BattleResult
{
public:
	BR_ReachPostion(): BattleResult(BRT_REACH_POS)
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