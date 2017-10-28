#ifndef __SKILL_OBJECT_H__
#define __SKILL_OBJECT_H__

#include "../ServerData/ServerDefine.h"

class CSceneObject;

class CSkillObject
{
public:
	CSkillObject();

	~CSkillObject();

public:
	BOOL OnUpdate(UINT64 dwTick);

	BOOL Start();

	BOOL GetTargets();

	BOOL SetHostObject(CSceneObject* pObject);

	BOOL SkillFight(CSceneObject* pTarget);

public:
	UINT64  m_dwStartTick; //开始时间
	UINT32  m_dwSkillID;   //技能ID

	CSceneObject* m_pSceneObject;
};

#endif //__SKILL_OBJECT_H__
