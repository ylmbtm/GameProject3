#ifndef __SKILL_OBJECT_H__
#define __SKILL_OBJECT_H__

#include "../ServerData/ServerDefine.h"
#include "../ConfigData/SkillStruct.h"
#include "XMath.h"
class CSceneObject;

class CSkillObject
{
public:
	CSkillObject();

	~CSkillObject();

public:
	BOOL OnUpdate(UINT64 uTick);

	BOOL StartSkill(UINT32 dwSkillID);

	BOOL SetCastObject(CSceneObject* pObject);

	BOOL AddTargetObject(CSceneObject* pObject);

	BOOL SkillFight(CSceneObject* pTarget);

	BOOL ProcessEvent(StSkillEvent& SkillEvent);

public:
	UINT64			m_dwStartTick; //开始时间
	UINT32			m_dwSkillID;   //技能ID
	UINT32			m_dwEventIndex;
	StSkillInfo*	m_pSkillInfo;
	CSceneObject*	m_pCastObject;
	Vector3D		m_TargetPos;
	std::vector<CSceneObject*> m_vtTargets;
};

#endif //__SKILL_OBJECT_H__
