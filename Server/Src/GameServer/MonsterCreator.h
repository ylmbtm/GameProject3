#ifndef _GAME_MONSTER_CREATOR_H_
#define _GAME_MONSTER_CREATOR_H_

#include "RapidXml.h"

class CScene;
class CSceneObject;
class MonsterData
{
public:
	MonsterData()
	{
		m_dwActorID		= 0;	//怪物类型ID
		m_dwType		= 0;	//怪物类别
		m_dwCamp		= 0;	//阵营
		m_dwDropID		= 0;	//掉落ID
		m_bCheckCamp	= 0;    //是否影响结算
		m_bCheckDie		= 0;    //是否影响刷怪
		m_x				= 0;
		m_y				= 0;
		m_z				= 0;
		m_ft			= 0;	//怪物出生位置
	}

	~MonsterData()
	{

	}

	UINT32 m_dwActorID;			//怪物类型ID
	UINT32 m_dwType;			//怪物类别
	UINT32 m_dwCamp;			//阵营
	UINT32 m_dwDropID;			//掉落ID
	BOOL   m_bCheckCamp;		//是否影响结算
	BOOL   m_bCheckDie;			//是否影响刷怪
	FLOAT  m_x, m_y, m_z, m_ft;	//怪物出生位置
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
	UINT32 m_dwGenType;
};


class MonsterCreator
{
public:
	MonsterCreator(CScene* pScene);
	~MonsterCreator();

	BOOL		ReadFromXml(rapidxml::xml_node<char>* pNode);

	BOOL		OnUpdate(UINT64 uTick);

	BOOL		GenMonsterWave(INT32 dwWaveIndex);

	BOOL		IsAllFinished();

	BOOL		OnObjectDie(CSceneObject* pObject);

	BOOL        OnPlayerMove(FLOAT x, FLOAT z);

	CScene*		m_pScene;

	BOOL		m_bAllFinished;

	INT32		m_dwCurWave;

	std::vector<MonsterWave> m_MonsterVaveList;
};

#endif