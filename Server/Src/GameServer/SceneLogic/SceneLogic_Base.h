#ifndef _SCENELOGIC_BASE_H_
#define _SCENELOGIC_BASE_H_
class CScene;
class CSceneObject;

enum BATTLERESULTYPE
{
	BRT_NONE,
	BRT_KILL_ALL,			//击杀全部怪物
	BRT_KILL_NUM,			//击杀指定数量怪物
	BRT_REACH_POS,			//达到目的地
	BRT_ALIVE,				//存活下来
	BRT_NPC_ALIVE,			//护送npc
	BRT_END
};


class SceneLogicBase
{
public:
	SceneLogicBase(CScene *pScene);
	
	~SceneLogicBase();
public:
	virtual BOOL OnObjectCreate(CSceneObject *pObject);

	virtual BOOL OnObjectDie(CSceneObject *pObject);

	virtual BOOL OnPlayerEnter(CSceneObject *pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject *pPlayer);

	virtual BOOL Update(UINT32 dwTick);

	virtual BOOL OnTimeUP();

	virtual BOOL IsFinished();

	virtual BOOL SetLastTime(UINT32 dwTime);

	//TRUE:表示己经决出胜负, FALSE没有决出胜负
	virtual BOOL BattleResultCheck();

	CScene*  GetScene();

public:
	CScene *m_pScene;

public:
	BOOL m_bFinished;

	UINT32 m_dwLastTime;  

	UINT32 m_dwResultType;

};

#endif //_SCENELOGIC_BASE_H_


