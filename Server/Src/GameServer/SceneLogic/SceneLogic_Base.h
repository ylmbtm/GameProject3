#ifndef _SCENELOGIC_BASE_H_
#define _SCENELOGIC_BASE_H_
class CScene;
class CSceneObject;


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

	virtual BOOL SetEndTime(UINT32 dwTime);

	CScene*  GetScene();

public:
	CScene *m_pScene;

public:
	BOOL m_bFinished;

	UINT32 m_dwEndTime;

};

#endif //_SCENELOGIC_BASE_H_


