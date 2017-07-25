#ifndef _SCENELOGIC_BASE_H_
#define _SCENELOGIC_BASE_H_

#include "Utility\RapidXml\rapidxml.h"
#include "..\Message\Game_Define.pb.h"
#include "Utility\XMath.h"

class CScene;
class CSceneObject;
class BattleResult;



class SceneLogicBase
{
public:
	SceneLogicBase(CScene *pScene);
	
	~SceneLogicBase();
public:
	virtual BOOL ReadFromXml(rapidxml::xml_node<char> *pNode);

	virtual BOOL OnObjectCreate(CSceneObject *pObject);

	virtual BOOL OnObjectDie(CSceneObject *pObject);

	virtual BOOL OnPlayerEnter(CSceneObject *pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject *pPlayer);

	virtual BOOL Update(UINT32 dwTick);

	virtual BOOL OnTimeUP();

    virtual BOOL SetFinished();
	virtual BOOL IsFinished();

	virtual BOOL SetLastTime(UINT32 dwTime);

	//TRUE:表示己经决出胜负, FALSE没有决出胜负
	virtual BOOL BattleResultCheck();

	CScene*  GetScene();
public:
	CScene *m_pScene;
	BOOL m_bFinished;

	UINT32 m_dwLastTime; 

	BattleResult *m_pBattleResult;

	CPoint2d m_vtBirthPos[CT_CMAP_END];
};

#endif //_SCENELOGIC_BASE_H_


