#ifndef __RANK_MANAGER_H__
#define __RANK_MANAGER_H__

#include "Rank.h"

class CRankManager
{
public:
	static CRankManager* GetInstancePtr();

private:
	CRankManager();
	~CRankManager();

public:
	BOOL    InitRank();

	TRanker m_LevelRanker;  //等级排行榜

	TRanker m_FightRanker;  //战力排行榜
};
#endif