#ifndef _SCENELOGIC_BASE_H_
#define _SCENELOGIC_BASE_H_

#include "RapidXml.h"
#include "../Message/Game_Define.pb.h"
#include "XMath.h"
#include "BattleResult.h"

class CScene;
class CSceneObject;

class SceneLogicBase
{
public:
	SceneLogicBase(CScene* pScene);

	~SceneLogicBase();
public:
	virtual BOOL ReadFromXml(rapidxml::xml_node<char>* pNode);

	virtual BOOL OnObjectCreate(CSceneObject* pObject);

	virtual BOOL OnObjectDie(CSceneObject* pObject);

	virtual BOOL OnPlayerEnter(CSceneObject* pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject* pPlayer);

	virtual BOOL Update(UINT64 uTick);

	virtual BOOL OnTimeUP();

	//TRUE:表示己经决出胜负, FALSE没有决出胜负
	virtual BOOL BattleResultCheck();

	CScene*  GetScene();
public:
	CScene* m_pScene;
	
	BattleResult m_BattleResult;

	CPoint3D m_vtBornPos[CT_CMAP_END];
};

#endif //_SCENELOGIC_BASE_H_


