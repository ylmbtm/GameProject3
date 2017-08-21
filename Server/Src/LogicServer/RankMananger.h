#ifndef __RANK_MANAGER_H__
#define __RANK_MANAGER_H__

#include "Rank.h"

class CRankManager
{
	CRankManager();
	~CRankManager();
public:
	static CRankManager* GetInstancePtr();

	TRanker m_LevelRanker;
};
#endif