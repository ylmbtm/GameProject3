#ifndef __RANK_MANAGER_H__
#define __RANK_MANAGER_H__

#include "DBInterface/CppMysql.h"
#include "Rank.h"

class CRankManager
{
public:
	static CRankManager* GetInstancePtr();

private:
	CRankManager();
	~CRankManager();

public:
	BOOL    LoadData(CppMySQL3DB& tDBConnection);

	TRanker<UINT64, INT32> m_LevelRanker; //等级排行榜

	TRanker<UINT64, INT32> m_FightRanker;  //战力排行榜
};
#endif