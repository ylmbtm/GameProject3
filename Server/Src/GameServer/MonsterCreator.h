#include "Utility\RapidXml\rapidxml.h"
#ifndef _GAME_MONSTER_CREATOR_H_
#define _GAME_MONSTER_CREATOR_H_

class CScene;

class MonsterData
{
public:
    MonsterData()
    {

    }

    ~MonsterData()
    {

    }

    UINT32 m_dwTypeID; //怪物类型ID
    UINT32 m_dwType;   //怪物类别
};

class MonsterWave 
{
public:
    MonsterWave()
    {

    }

    ~MonsterWave()
    {

    }

public:
    std::vector<MonsterData>  m_vtMonsterList;

    BOOL IsFinished();

    BOOL SetFinished();

    BOOL IsReady();

    BOOL   m_bFinished;
    BOOL   m_bReady;
};


class MonsterCreator
{
public:
	MonsterCreator(CScene *pScene);
	~MonsterCreator();

	BOOL ReadFromXml(rapidxml::xml_node<char> *pNode);

	BOOL OnUpdate(UINT32 dwTick);

    BOOL GenMonsterWave(MonsterWave *pWave);

    BOOL IsAllFinished();

    std::vector<MonsterWave> m_MonsterVaveList;

	CScene *m_pScene;

    BOOL    m_bAllFinished;
};

#endif