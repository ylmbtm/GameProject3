#ifndef _SCENELOGIC_BASE_H_
#define _SCENELOGIC_BASE_H_

#include "RapidXml.h"
#include "../Message/Game_Define.pb.h"
#include "WinCondition.h"
#include "CommonMath.h"

class CScene;
class CSceneObject;

class SceneLogicBase
{
public:
	SceneLogicBase(CScene* pScene);

	virtual ~SceneLogicBase();
public:
	virtual BOOL ReadFromXml(rapidxml::xml_node<char>* pNode);

	virtual BOOL OnObjectCreate(CSceneObject* pObject);

	virtual BOOL OnObjectDie(CSceneObject* pObject);

	virtual BOOL OnPlayerEnter(CSceneObject* pPlayer);

	virtual BOOL OnPlayerLeave(CSceneObject* pPlayer, BOOL bDisConnect);

	virtual BOOL Update(UINT64 uTick);

	virtual BOOL OnTimeUP() = 0;

	CScene*  GetScene();

protected:
	BOOL ReadBornFromXml(rapidxml::xml_node<char>* pNode);

	BOOL ReadConditionFromXml(rapidxml::xml_node<char>* pNode);

	CScene* m_pScene;

	CWinCondition m_BattleCondition;

	std::vector<CPoint3D> m_vtBornPos;
};

#endif //_SCENELOGIC_BASE_H_


