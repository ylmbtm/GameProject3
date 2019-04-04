#ifndef __SKILL_OBJECT_H__
#define __SKILL_OBJECT_H__

#include "../StaticData/SkillStruct.h"
#include "XMath.h"
class CSceneObject;

class CSkillObject
{
public:
	CSkillObject();

	~CSkillObject();

public:
	BOOL OnUpdate(UINT64 uTick);

	BOOL OnSkillComplete();

	BOOL StartSkill(UINT32 dwSkillID, INT32 nLevel);

	BOOL StopSkill();

	BOOL ResetSkill();

	BOOL SetCastObject(CSceneObject* pObject);

	BOOL AddTargetObject(CSceneObject* pObject);

	BOOL SkillFight(StSkillEvent& SkillEvent, CSceneObject* pTarget);

	BOOL CalcTargetObjects(StSkillEvent& SkillEvent);

	BOOL ProcessEvent(StSkillEvent& SkillEvent);

	ESkillStatus GetSkillStatus();

	BOOL SetComboSkill(BOOL bCombo);

	UINT32 GetSkillID();

public:
	UINT64              m_dwStartTick; //开始时间
	UINT32              m_dwSkillID;   //技能ID
	UINT32              m_dwEventIndex;
	StSkillInfo*        m_pSkillInfo;
	StSkillEventInfo*   m_pSkillEventInfo;
	CSceneObject*       m_pCastObject;
	Vector3D            m_TargetPos;
	BOOL                m_bComboSkill;  //连击开启
	ESkillStatus        m_SkillStatus;

	std::vector<CSceneObject*> m_vtTargets;
};

#endif //__SKILL_OBJECT_H__
