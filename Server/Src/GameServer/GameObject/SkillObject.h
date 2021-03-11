#ifndef __SKILL_OBJECT_H__
#define __SKILL_OBJECT_H__

#include "SkillStruct.h"
#include "CommonMath.h"
#include "BulletObject.h"
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

	CSceneObject* GetCastObject();

	BOOL AddTargetObject(CSceneObject* pObject);

	INT32 GetTargetNum();

	BOOL AttackTarget(CSceneObject* pTarget, UINT32 HitActionID, UINT32 HitEffectID, FLOAT HitDistance);

	BOOL CalcTargetObjects(StSkillEvent& SkillEvent);

	BOOL ProcessSkillEvent(StSkillEvent& SkillEvent);

	ESkillStatus GetSkillStatus();

	BOOL SetComboSkill(BOOL bCombo);

	UINT32 GetSkillID();

	VOID SetCalcTargets(BOOL bCalc);

	CBulletObject* CreateBullet(StBulletObject& stBullet);

public:
	UINT64              m_dwStartTick; //开始时间
	UINT32              m_dwSkillID;   //技能ID
	UINT32              m_dwEventIndex;
	StSkillInfo*        m_pSkillInfo;
	StSkillEventInfo*   m_pSkillEventInfo;
	CSceneObject*       m_pCastObject;
	BOOL                m_bComboSkill;  //连击开启
	ESkillStatus        m_SkillStatus;
	BOOL                m_bCalcTargets;
	std::vector<CSceneObject*> m_vtTargets;
};

#endif //__SKILL_OBJECT_H__
