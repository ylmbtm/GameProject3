#ifndef __SKILL_OBJECT_H__
#define __SKILL_OBJECT_H__

#include "../ServerData/ServerDefine.h"
#include "../ConfigData/ConfigStruct.h"

class CSceneObject;

class CSkillObject
{
public:
	CSkillObject();

	~CSkillObject();

public:
	BOOL OnUpdate(UINT64 uTick);

	BOOL StartSkill(UINT32 dwSkillID);

	BOOL GetTargets();

	BOOL SetHostObject(CSceneObject* pObject);

	BOOL SkillFight(CSceneObject* pTarget);

public:
	UINT64			m_dwStartTick; //开始时间
	UINT32			m_dwSkillID;   //技能ID
	StSkillInfo*	m_pSkillInfo;
	CSceneObject*	m_pSceneObject;

	std::vector<CSceneObject*> m_vtTargets;
};

#endif //__SKILL_OBJECT_H__
