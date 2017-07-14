#ifndef _GAME_MONSTER_CREATOR_H_
#define _GAME_MONSTER_CREATOR_H_

class CScene;


class MonsterCreator
{
public:
	MonsterCreator(CScene *pScene);
	~MonsterCreator();


	BOOL OnUpdate(UINT32 dwTick);


	CScene *m_pScene;
};

#endif